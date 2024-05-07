/*
 * Copyright (c) 2024, Meta
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/posix/sys/features.h>
#include <zephyr/posix/sys/sysconf.h>
#include <zephyr/posix/unistd.h>

#ifdef CONFIG_POSIX_SYSCONF_IMPL_FULL

#define z_sysconf(x) (long)CONCAT(__z_posix_sysconf, x)

long sysconf(int x)
{
	switch (x) {
	case _SC_ADVISORY_INFO:
		return z_sysconf(_SC_ADVISORY_INFO);
	case _SC_ASYNCHRONOUS_IO:
		return z_sysconf(_SC_ASYNCHRONOUS_IO);
	case _SC_BARRIERS:
		return z_sysconf(_SC_BARRIERS);
	case _SC_CLOCK_SELECTION:
		return z_sysconf(_SC_CLOCK_SELECTION);
	case _SC_CPUTIME:
		return z_sysconf(_SC_CPUTIME);
	case _SC_FSYNC:
		return z_sysconf(_SC_FSYNC);
	case _SC_IPV6:
		return z_sysconf(_SC_IPV6);
	case _SC_JOB_CONTROL:
		return -1;
	case _SC_MAPPED_FILES:
		return z_sysconf(_SC_MAPPED_FILES);
	case _SC_MEMLOCK:
		return z_sysconf(_SC_MEMLOCK);
	case _SC_MEMLOCK_RANGE:
		return z_sysconf(_SC_MEMLOCK_RANGE);
	case _SC_MEMORY_PROTECTION:
		return z_sysconf(_SC_MEMORY_PROTECTION);
	case _SC_MESSAGE_PASSING:
		return z_sysconf(_SC_MESSAGE_PASSING);
	case _SC_MONOTONIC_CLOCK:
		return z_sysconf(_SC_MONOTONIC_CLOCK);
	case _SC_PRIORITIZED_IO:
		return z_sysconf(_SC_PRIORITIZED_IO);
	case _SC_PRIORITY_SCHEDULING:
		return z_sysconf(_SC_PRIORITY_SCHEDULING);
	case _SC_RAW_SOCKETS:
		return z_sysconf(_SC_RAW_SOCKETS);
	case _SC_RE_DUP_MAX:
		return -1;
	case _SC_READER_WRITER_LOCKS:
		return z_sysconf(_SC_READER_WRITER_LOCKS);
	case _SC_REALTIME_SIGNALS:
		return z_sysconf(_SC_REALTIME_SIGNALS);
	case _SC_REGEXP:
		return z_sysconf(_SC_REGEXP);
	case _SC_SAVED_IDS:
		return -1;
	case _SC_SEMAPHORES:
		return z_sysconf(_SC_SEMAPHORES);
	case _SC_SHARED_MEMORY_OBJECTS:
		return z_sysconf(_SC_SHARED_MEMORY_OBJECTS);
	case _SC_SHELL:
		return z_sysconf(_SC_SHELL);
	case _SC_SPAWN:
		return z_sysconf(_SC_SPAWN);
	case _SC_SPIN_LOCKS:
		return z_sysconf(_SC_SPIN_LOCKS);
	case _SC_SPORADIC_SERVER:
		return z_sysconf(_SC_SPORADIC_SERVER);
	case _SC_SS_REPL_MAX:
		return -1;
	case _SC_SYNCHRONIZED_IO:
		return z_sysconf(_SC_SYNCHRONIZED_IO);
	case _SC_THREAD_ATTR_STACKADDR:
		return z_sysconf(_SC_THREAD_ATTR_STACKADDR);
	case _SC_THREAD_ATTR_STACKSIZE:
		return z_sysconf(_SC_THREAD_ATTR_STACKSIZE);
	case _SC_THREAD_CPUTIME:
		return z_sysconf(_SC_THREAD_CPUTIME);
	case _SC_THREAD_PRIO_INHERIT:
		return z_sysconf(_SC_THREAD_PRIO_INHERIT);
	case _SC_THREAD_PRIO_PROTECT:
		return z_sysconf(_SC_THREAD_PRIO_PROTECT);
	case _SC_THREAD_PRIORITY_SCHEDULING:
		return z_sysconf(_SC_THREAD_PRIORITY_SCHEDULING);
	case _SC_THREAD_PROCESS_SHARED:
		return z_sysconf(_SC_THREAD_PROCESS_SHARED);
	case _SC_THREAD_ROBUST_PRIO_INHERIT:
		return z_sysconf(_SC_THREAD_ROBUST_PRIO_INHERIT);
	case _SC_THREAD_ROBUST_PRIO_PROTECT:
		return z_sysconf(_SC_THREAD_ROBUST_PRIO_PROTECT);
	case _SC_THREAD_SAFE_FUNCTIONS:
		return z_sysconf(_SC_THREAD_SAFE_FUNCTIONS);
	case _SC_THREAD_SPORADIC_SERVER:
		return z_sysconf(_SC_THREAD_SPORADIC_SERVER);
	case _SC_THREADS:
		return z_sysconf(_SC_THREADS);
	case _SC_TIMEOUTS:
		return z_sysconf(_SC_TIMEOUTS);
	case _SC_TIMERS:
		return z_sysconf(_SC_TIMERS);
	case _SC_TRACE:
		return z_sysconf(_SC_TRACE);
	case _SC_TRACE_EVENT_FILTER:
		return z_sysconf(_SC_TRACE_EVENT_FILTER);
	case _SC_TRACE_EVENT_NAME_MAX:
		return z_sysconf(_SC_TRACE_EVENT_NAME_MAX);
	case _SC_TRACE_INHERIT:
		return z_sysconf(_SC_TRACE_INHERIT);
	case _SC_TRACE_LOG:
		return z_sysconf(_SC_TRACE_LOG);
	case _SC_TRACE_NAME_MAX:
		return z_sysconf(_SC_TRACE_NAME_MAX);
	case _SC_TRACE_SYS_MAX:
		return z_sysconf(_SC_TRACE_SYS_MAX);
	case _SC_TRACE_USER_EVENT_MAX:
		return z_sysconf(_SC_TRACE_USER_EVENT_MAX);
	case _SC_TYPED_MEMORY_OBJECTS:
		return z_sysconf(_SC_TYPED_MEMORY_OBJECTS);
	case _SC_VERSION:
		return z_sysconf(_SC_VERSION);
	case _SC_V7_ILP32_OFF32:
		return -1;
	case _SC_V7_ILP32_OFFBIG:
		return -1;
	case _SC_V7_LP64_OFF64:
		return -1;
	case _SC_V7_LPBIG_OFFBIG:
		return -1;
	case _SC_V6_ILP32_OFF32:
		return -1;
	case _SC_V6_ILP32_OFFBIG:
		return -1;
	case _SC_V6_LP64_OFF64:
		return -1;
	case _SC_V6_LPBIG_OFFBIG:
		return -1;
	case _SC_BC_BASE_MAX:
		return -1;
	case _SC_BC_DIM_MAX:
		return -1;
	case _SC_BC_SCALE_MAX:
		return -1;
	case _SC_BC_STRING_MAX:
		return -1;
	case _SC_2_C_BIND:
		return _POSIX2_VERSION;
	case _SC_2_C_DEV:
		return _POSIX2_VERSION;
	case _SC_2_CHAR_TERM:
		return -1;
	case _SC_COLL_WEIGHTS_MAX:
		return -1;
	case _SC_DELAYTIMER_MAX:
		return -1;
	case _SC_EXPR_NEST_MAX:
		return -1;
	case _SC_2_FORT_DEV:
		return -1;
	case _SC_2_FORT_RUN:
		return -1;
	case _SC_LINE_MAX:
		return -1;
	case _SC_2_LOCALEDEF:
		return -1;
	case _SC_2_PBS:
		return -1;
	case _SC_2_PBS_ACCOUNTING:
		return -1;
	case _SC_2_PBS_CHECKPOINT:
		return -1;
	case _SC_2_PBS_LOCATE:
		return -1;
	case _SC_2_PBS_MESSAGE:
		return -1;
	case _SC_2_PBS_TRACK:
		return -1;
	case _SC_2_SW_DEV:
		return -1;
	case _SC_2_UPE:
		return -1;
	case _SC_2_VERSION:
		return _POSIX2_VERSION;
	case _SC_XOPEN_CRYPT:
		return -1;
	case _SC_XOPEN_ENH_I18N:
		return -1;
	case _SC_XOPEN_REALTIME:
		return z_sysconf(_SC_XOPEN_REALTIME);
	case _SC_XOPEN_REALTIME_THREADS:
		return z_sysconf(_SC_XOPEN_REALTIME_THREADS);
		/* shmctl(), shmget(), shmat(), shmdt() */
	case _SC_XOPEN_SHM:
		return -1;
	case _SC_XOPEN_STREAMS:
		return z_sysconf(_SC_XOPEN_STREAMS);
	case _SC_XOPEN_UNIX:
		return z_sysconf(_SC_XOPEN_UNIX);
	case _SC_XOPEN_UUCP:
		return z_sysconf(_SC_XOPEN_UUCP);
	case _SC_XOPEN_VERSION:
		return z_sysconf(_SC_VERSION);
	case _SC_CLK_TCK:
		return (100L);
	case _SC_GETGR_R_SIZE_MAX:
		return (0L);
	case _SC_GETPW_R_SIZE_MAX:
		return (0L);
	case _SC_AIO_LISTIO_MAX:
		return AIO_LISTIO_MAX;
	case _SC_AIO_MAX:
		return AIO_MAX;
	case _SC_AIO_PRIO_DELTA_MAX:
		return AIO_PRIO_DELTA_MAX;
	case _SC_ARG_MAX:
		return ARG_MAX;
	case _SC_ATEXIT_MAX:
		return ATEXIT_MAX;
	case _SC_CHILD_MAX:
		return CHILD_MAX;
	case _SC_HOST_NAME_MAX:
		return _POSIX_HOST_NAME_MAX;
	case _SC_IOV_MAX:
		return IOV_MAX;
	case _SC_LOGIN_NAME_MAX:
		return LOGIN_NAME_MAX;
	case _SC_NGROUPS_MAX:
		return _POSIX_NGROUPS_MAX;
	case _SC_MQ_OPEN_MAX:
		return -1; /* z_sysconf(_SC_MQ_OPEN_MAX); */
	case _SC_MQ_PRIO_MAX:
		return -1; /* z_sysconf(_SC_MQ_PRIO_MAX); */
	case _SC_OPEN_MAX:
		return z_sysconf(_SC_OPEN_MAX);
	case _SC_PAGE_SIZE:
		return PAGE_SIZE;
	case _SC_PAGESIZE:
		return PAGESIZE;
	case _SC_THREAD_DESTRUCTOR_ITERATIONS:
		return PTHREAD_DESTRUCTOR_ITERATIONS;
	case _SC_THREAD_KEYS_MAX:
		return PTHREAD_KEYS_MAX;
	case _SC_THREAD_STACK_MIN:
		return PAGE_SIZE;
	case _SC_THREAD_THREADS_MAX:
		return z_sysconf(_SC_THREAD_THREADS_MAX);
	case _SC_RTSIG_MAX:
		return RTSIG_MAX;
	case _SC_SEM_NSEMS_MAX:
		return SEM_NSEMS_MAX;
	case _SC_SEM_VALUE_MAX:
		return SEM_VALUE_MAX;
	case _SC_SIGQUEUE_MAX:
		return SIGQUEUE_MAX;
	case _SC_STREAM_MAX:
		return STREAM_MAX;
	case _SC_SYMLOOP_MAX:
		return SYMLOOP_MAX;
	case _SC_TIMER_MAX:
		return z_sysconf(_SC_TIMER_MAX);
	case _SC_TTY_NAME_MAX:
		return TTY_NAME_MAX;
	case _SC_TZNAME_MAX:
		return TZNAME_MAX;
	default:
		errno = EINVAL;
		return -1;
	}
}

#endif /* CONFIG_POSIX_SYSCONF_IMPL_FULL */
