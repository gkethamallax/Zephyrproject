# Copyright (c) 2018 Open Source Foundries Limited.
#
# SPDX-License-Identifier: Apache-2.0

'''Common code used by commands which execute runners.
'''

import argparse
import logging
from os import close, getcwd, path
from subprocess import CalledProcessError
import tempfile
import textwrap
import traceback

from west import cmake
from west import log
from west import util
from build_helpers import find_build_dir, is_zephyr_build, \
    FIND_BUILD_DIR_DESCRIPTION
from west.commands import CommandError
from west.configuration import config

from runners import get_runner_cls, ZephyrBinaryRunner, MissingProgram

from zephyr_ext_common import cached_runner_config

# Context-sensitive help indentation.
# Don't change this, or output from argparse won't match up.
INDENT = ' ' * 2

if log.VERBOSE >= log.VERBOSE_NORMAL:
    # Using level 1 allows sub-DEBUG levels of verbosity. The
    # west.log module decides whether or not to actually print the
    # message.
    #
    # https://docs.python.org/3.7/library/logging.html#logging-levels.
    LOG_LEVEL = 1
else:
    LOG_LEVEL = logging.INFO

def _banner(msg):
    log.inf('-- ' + msg, colorize=True)

class WestLogFormatter(logging.Formatter):

    def __init__(self):
        super().__init__(fmt='%(name)s: %(message)s')

class WestLogHandler(logging.Handler):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setFormatter(WestLogFormatter())
        self.setLevel(LOG_LEVEL)

    def emit(self, record):
        fmt = self.format(record)
        lvl = record.levelno
        if lvl > logging.CRITICAL:
            log.die(fmt)
        elif lvl >= logging.ERROR:
            log.err(fmt)
        elif lvl >= logging.WARNING:
            log.wrn(fmt)
        elif lvl >= logging.INFO:
            _banner(fmt)
        elif lvl >= logging.DEBUG:
            log.dbg(fmt)
        else:
            log.dbg(fmt, level=log.VERBOSE_EXTREME)

def add_parser_common(parser_adder, command):
    parser = parser_adder.add_parser(
        command.name,
        formatter_class=argparse.RawDescriptionHelpFormatter,
        help=command.help,
        description=command.description)

    # Remember to update scripts/west-completion.bash if you add or remove
    # flags

    parser.add_argument('-H', '--context', action='store_true',
                        help='''Rebuild application and print context-sensitive
                        help; this may be combined with --runner to restrict
                        output to a given runner.''')

    group = parser.add_argument_group(title='General Options')

    group.add_argument('-d', '--build-dir',
                       help='Build directory to obtain runner information ' +
                       'from. ' + FIND_BUILD_DIR_DESCRIPTION)
    group.add_argument('-c', '--cmake-cache',
                       help='''Path to CMake cache file containing runner
                       configuration (this is generated by the Zephyr
                       build system when compiling binaries);
                       default: {}.

                       If this is a relative path, it is assumed relative to
                       the build directory. An absolute path can also be
                       given instead.'''.format(cmake.DEFAULT_CACHE))
    group.add_argument('-r', '--runner',
                       help='''If given, overrides any cached {}
                       runner.'''.format(command.name))
    group.add_argument('--skip-rebuild', action='store_true',
                       help='''If given, do not rebuild the application
                       before running {} commands.'''.format(command.name))

    group = parser.add_argument_group(
        title='Configuration overrides',
        description=textwrap.dedent('''\
        These values usually come from the Zephyr build system itself
        as stored in the CMake cache; providing these options
        overrides those settings.'''))

    # Important:
    #
    # 1. The destination variables of these options must match
    #    the RunnerConfig slots.
    # 2. The default values for all of these must be None.
    #
    # This is how we detect if the user provided them or not when
    # overriding values from the cached configuration.

    command_verb = "flash" if command == "flash" else "debug"

    group.add_argument('--board-dir',
                       help='Zephyr board directory')
    group.add_argument('--elf-file',
                       help='Path to elf file to {0}'.format(command_verb))
    group.add_argument('--hex-file',
                       help='Path to hex file to {0}'.format(command_verb))
    group.add_argument('--bin-file',
                       help='Path to binary file to {0}'.format(command_verb))
    #TODO: Don't care about the file type. back-compatibility, eh?
    group.add_argument(
        '-f', '--file',
        help='Path to elf/hex/bin file to {0}. '.format(command_verb)
           + 'Infers type from extension if --file-type is not provided.',
        dest='fname',
        metavar='FILE')
    group.add_argument(
        '--file-type',
        help='Override geussing of file type from extension. '
             'Meaningless without -f.',
        choices=['elf','hex','bin'])
    group.add_argument('--gdb',
                       help='Path to GDB, if applicable')
    group.add_argument('--openocd',
                       help='Path to OpenOCD, if applicable')
    group.add_argument(
        '--openocd-search',
        help='Path to add to OpenOCD search path, if applicable')

    return parser


def desc_common(command_name):
    return textwrap.dedent('''\
    Any options not recognized by this command are passed to the
    back-end {command} runner (run "west {command} --context"
    for help on available runner-specific options).

    If you need to pass an option to a runner which has the
    same name as one recognized by this command, you can
    end argument parsing with a '--', like so:

    west {command} --{command}-arg=value -- --runner-arg=value2
    '''.format(**{'command': command_name}))


def _override_config_from_namespace(cfg, namespace):
    '''Override a RunnerConfig's contents with command-line values.'''
    for var in cfg.__slots__:
        if var in namespace:
            val = getattr(namespace, var)
            if val is not None:
                setattr(cfg, var, val)


def _build_dir(args, die_if_none=True):
    # Get the build directory for the given argument list and environment.
    if args.build_dir:
        return args.build_dir

    guess = config.get('build', 'guess-dir', fallback='never')
    guess = True if guess == 'runners' else False
    dir = find_build_dir(None, guess)

    if dir and is_zephyr_build(dir):
        return dir
    elif die_if_none:
        msg = '--build-dir was not given, '
        if dir:
            msg = msg + 'and neither {} nor {} are zephyr build directories.'
        else:
            msg = msg + ('{} is not a build directory and the default build '
                         'directory cannot be determined. Check your '
                         'build.dir-fmt configuration option')
        log.die(msg.format(getcwd(), dir))
    else:
        return None

def dump_traceback():
    # Save the current exception to a file and return its path.
    fd, name = tempfile.mkstemp(prefix='west-exc-', suffix='.txt')
    close(fd)        # traceback has no use for the fd
    with open(name, 'w') as f:
        traceback.print_exc(file=f)
    log.inf("An exception trace has been saved in", name)

def do_run_common(command, args, runner_args, cached_runner_var):
    if args.context:
        _dump_context(command, args, runner_args, cached_runner_var)
        return

    command_name = command.name
    build_dir = _build_dir(args)

    if not args.skip_rebuild:
        _banner('west {}: rebuilding'.format(command_name))
        try:
            cmake.run_build(build_dir)
        except CalledProcessError:
            if args.build_dir:
                log.die('cannot run {}, build in {} failed'.format(
                    command_name, args.build_dir))
            else:
                log.die('cannot run {}; no --build-dir given and build in '
                        'current directory {} failed'.format(command_name,
                                                             build_dir))

    # Runner creation, phase 1.
    #
    # Get the default runner name from the cache, allowing a command
    # line override. Get the ZephyrBinaryRunner class by name, and
    # make sure it supports the command.

    cache_file = path.join(build_dir, args.cmake_cache or cmake.DEFAULT_CACHE)
    try:
        cache = cmake.CMakeCache(cache_file)
    except FileNotFoundError:
        log.die('no CMake cache found (expected one at {})'.format(cache_file))
    board = cache['CACHED_BOARD']
    available = cache.get_list('ZEPHYR_RUNNERS')
    if not available:
        log.wrn('No cached runners are available in', cache_file)
    runner = args.runner or cache.get(cached_runner_var)

    if runner is None:
        log.die('No', command_name, 'runner available for board', board,
                '({} is not in the cache).'.format(cached_runner_var),
                "Check your board's documentation for instructions.")

    _banner('west {}: using runner {}'.format(command_name, runner))
    if runner not in available:
        log.wrn('Runner {} is not configured for use with {}, '
                'this may not work'.format(runner, board))
    runner_cls = get_runner_cls(runner)
    if command_name not in runner_cls.capabilities().commands:
        log.die('Runner {} does not support command {}'.format(
            runner, command_name))

    # Runner creation, phase 2.
    #
    # At this point, the common options above are already parsed in
    # 'args', and unrecognized arguments are in 'runner_args'.
    #
    # - Set up runner logging to delegate to west.
    # - Pull the RunnerConfig out of the cache
    # - Override cached values with applicable command-line options

    logger = logging.getLogger('runners')
    logger.setLevel(LOG_LEVEL)
    logger.addHandler(WestLogHandler())
    cfg = cached_runner_config(build_dir, cache)
    _override_config_from_namespace(cfg, args)

    # Runner creation, phase 3.
    #
    # - Pull out cached runner arguments, and append command-line
    #   values (which should override the cache)
    # - Construct a runner-specific argument parser to handle cached
    #   values plus overrides given in runner_args
    # - Parse arguments and create runner instance from final
    #   RunnerConfig and parsed arguments.

    cached_runner_args = cache.get_list(
        'ZEPHYR_RUNNER_ARGS_{}'.format(cmake.make_c_identifier(runner)))
    assert isinstance(runner_args, list), runner_args
    # If the user passed -- to force the parent argument parser to stop
    # parsing, it will show up here, and needs to be filtered out.
    runner_args = [arg for arg in runner_args if arg != '--']
    final_runner_args = cached_runner_args + runner_args
    parser = argparse.ArgumentParser(prog=runner)
    runner_cls.add_parser(parser)
    parsed_args, unknown = parser.parse_known_args(args=final_runner_args)
    if unknown:
        log.die('Runner', runner, 'received unknown arguments:', unknown)
    runner = runner_cls.create(cfg, parsed_args)
    try:
        runner.run(command_name)
    except ValueError as ve:
        log.err(str(ve), fatal=True)
        dump_traceback()
        raise CommandError(1)
    except MissingProgram as e:
        log.die('required program', e.filename,
                'not found; install it or add its location to PATH')


#
# Context-specific help
#

def _dump_context(command, args, runner_args, cached_runner_var):
    build_dir = _build_dir(args, die_if_none=False)

    # Try to figure out the CMake cache file based on the build
    # directory or an explicit argument.
    if build_dir is not None:
        cache_file = path.abspath(
            path.join(build_dir, args.cmake_cache or cmake.DEFAULT_CACHE))
    elif args.cmake_cache:
        cache_file = path.abspath(args.cmake_cache)
    else:
        cache_file = None

    # Load the cache itself, if possible.
    if cache_file is None:
        log.wrn('No build directory (--build-dir) or CMake cache '
                '(--cmake-cache) given or found; output will be limited')
        cache = None
    else:
        try:
            cache = cmake.CMakeCache(cache_file)
        except Exception:
            log.die('Cannot load cache {}.'.format(cache_file))

    # If we have a build directory, try to ensure build artifacts are
    # up to date. If that doesn't work, still try to print information
    # on a best-effort basis.
    if build_dir and not args.skip_rebuild:
        try:
            cmake.run_build(build_dir)
        except CalledProcessError:
            msg = 'Failed re-building application; cannot load context. '
            if args.build_dir:
                msg += 'Is {} the right --build-dir?'.format(args.build_dir)
            else:
                msg += textwrap.dedent('''\
                Use --build-dir (-d) to specify a build directory; the one
                used was {}.'''.format(build_dir))
            log.die('\n'.join(textwrap.wrap(msg, initial_indent='',
                                            subsequent_indent=INDENT,
                                            break_on_hyphens=False)))

    if cache is None:
        _dump_no_context_info(command, args)
        if not args.runner:
            return

    if args.runner:
        # Just information on one runner was requested.
        _dump_one_runner_info(cache, args, build_dir, INDENT)
        return

    board = cache['CACHED_BOARD']

    all_cls = {cls.name(): cls for cls in ZephyrBinaryRunner.get_runners() if
               command.name in cls.capabilities().commands}
    available = [r for r in cache.get_list('ZEPHYR_RUNNERS') if r in all_cls]
    available_cls = {r: all_cls[r] for r in available if r in all_cls}

    default_runner = cache.get(cached_runner_var)
    cfg = cached_runner_config(build_dir, cache)

    log.inf('All Zephyr runners which support {}:'.format(command.name),
            colorize=True)
    for line in util.wrap(', '.join(all_cls.keys()), INDENT):
        log.inf(line)
    log.inf('(Not all may work with this build, see available runners below.)',
            colorize=True)

    if cache is None:
        log.warn('Missing or invalid CMake cache {}; there is no context.',
                 'Use --build-dir to specify the build directory.')
        return

    log.inf('Build directory:', colorize=True)
    log.inf(INDENT + build_dir)
    log.inf('Board:', colorize=True)
    log.inf(INDENT + board)
    log.inf('CMake cache:', colorize=True)
    log.inf(INDENT + cache_file)

    if not available:
        # Bail with a message if no runners are available.
        msg = ('No runners available for {}. '
               'Consult the documentation for instructions on how to run '
               'binaries on this target.').format(board)
        for line in util.wrap(msg, ''):
            log.inf(line, colorize=True)
        return

    log.inf('Available {} runners:'.format(command.name), colorize=True)
    log.inf(INDENT + ', '.join(available))
    log.inf('Additional options for available', command.name, 'runners:',
            colorize=True)
    for runner in available:
        _dump_runner_opt_help(runner, all_cls[runner])
    log.inf('Default {} runner:'.format(command.name), colorize=True)
    log.inf(INDENT + default_runner)
    _dump_runner_config(cfg, '', INDENT)
    log.inf('Runner-specific information:', colorize=True)
    for runner in available:
        log.inf('{}{}:'.format(INDENT, runner), colorize=True)
        _dump_runner_cached_opts(cache, runner, INDENT * 2, INDENT * 3)
        _dump_runner_caps(available_cls[runner], INDENT * 2)

    if len(available) > 1:
        log.inf('(Add -r RUNNER to just print information about one runner.)',
                colorize=True)


def _dump_no_context_info(command, args):
    all_cls = {cls.name(): cls for cls in ZephyrBinaryRunner.get_runners() if
               command.name in cls.capabilities().commands}
    log.inf('All Zephyr runners which support {}:'.format(command.name),
            colorize=True)
    for line in util.wrap(', '.join(all_cls.keys()), INDENT):
        log.inf(line)
    if not args.runner:
        log.inf('Add -r RUNNER to print more information about any runner.',
                colorize=True)


def _dump_one_runner_info(cache, args, build_dir, indent):
    runner = args.runner
    cls = get_runner_cls(runner)

    if cache is None:
        _dump_runner_opt_help(runner, cls)
        _dump_runner_caps(cls, '')
        return

    available = runner in cache.get_list('ZEPHYR_RUNNERS')
    cfg = cached_runner_config(build_dir, cache)

    log.inf('Build directory:', colorize=True)
    log.inf(INDENT + build_dir)
    log.inf('Board:', colorize=True)
    log.inf(INDENT + cache['CACHED_BOARD'])
    log.inf('CMake cache:', colorize=True)
    log.inf(INDENT + cache.cache_file)
    log.inf(runner, 'is available:', 'yes' if available else 'no',
            colorize=True)
    _dump_runner_opt_help(runner, cls)
    _dump_runner_config(cfg, '', indent)
    if available:
        _dump_runner_cached_opts(cache, runner, '', indent)
    _dump_runner_caps(cls, '')
    if not available:
        log.wrn('Runner', runner, 'is not configured in this build.')


def _dump_runner_caps(cls, base_indent):
    log.inf('{}Capabilities:'.format(base_indent), colorize=True)
    log.inf('{}{}'.format(base_indent + INDENT, cls.capabilities()))


def _dump_runner_opt_help(runner, cls):
    # Construct and print the usage text
    dummy_parser = argparse.ArgumentParser(prog='', add_help=False)
    cls.add_parser(dummy_parser)
    formatter = dummy_parser._get_formatter()
    for group in dummy_parser._action_groups:
        # Break the abstraction to filter out the 'flash', 'debug', etc.
        # TODO: come up with something cleaner (may require changes
        # in the runner core).
        actions = group._group_actions
        if len(actions) == 1 and actions[0].dest == 'command':
            # This is the lone positional argument. Skip it.
            continue
        formatter.start_section('REMOVE ME')
        formatter.add_text(group.description)
        formatter.add_arguments(actions)
        formatter.end_section()
    # Get the runner help, with the "REMOVE ME" string gone
    runner_help = '\n'.join(formatter.format_help().splitlines()[1:])

    log.inf('{} options:'.format(runner), colorize=True)
    log.inf(runner_help)


def _dump_runner_config(cfg, initial_indent, subsequent_indent):
    log.inf('{}Cached common runner configuration:'.format(initial_indent),
            colorize=True)
    for var in cfg.__slots__:
        log.inf('{}--{}={}'.format(subsequent_indent, var, getattr(cfg, var)))


def _dump_runner_cached_opts(cache, runner, initial_indent, subsequent_indent):
    runner_args = _get_runner_args(cache, runner)
    if not runner_args:
        return

    log.inf('{}Cached runner-specific options:'.format(initial_indent),
            colorize=True)
    for arg in runner_args:
        log.inf('{}{}'.format(subsequent_indent, arg))


def _get_runner_args(cache, runner):
    runner_ident = cmake.make_c_identifier(runner)
    args_var = 'ZEPHYR_RUNNER_ARGS_{}'.format(runner_ident)
    return cache.get_list(args_var)
