/* SPDX-License-Identifier: Apache-2.0 */

#ifdef CONFIG_STATIC_INIT_GNU_SUPPORTED
	SECTION_PROLOGUE(init_array,,)
	{
#ifdef CONFIG_64BIT
		. = ALIGN(8);
#else
		. = ALIGN(4);
#endif
		/*
		 * Add all of the GNU-style constructors in priority order. Note
		 * that this doesn't build the ctors in the "usual" fashion with
		 * a length value first and NULL terminator, but as we run this
		 * list ourselves, that doesn't matter.
		 */
		__zephyr_init_array_start = .;
		KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*)
			SORT_BY_INIT_PRIORITY(.ctors.*)))
		KEEP (*(.init_array .ctors))
		__zephyr_init_array_end = .;
#ifdef CONFIG_NATIVE_LIBC
		/*
		 * The __CTOR_LIST__ and __CTOR_END__ symbols are always defined
		 * to result in an empty list. This is necessary to fix an issue
		 * where the glibc process initialization code on native_posix
		 * platforms calls constructors before Zephyr loads (issue #39347).
		 */
		__CTOR_LIST__ = .;
#ifdef CONFIG_64BIT
		QUAD(0)
		QUAD(0)
#else
		__CTOR_LIST__ = .;
		LONG(0)
		LONG(0)
#endif
		__CTOR_END__ = .;
		/*
		* Similar to the schenanigans required for the __CTOR_LIST__ and
		* __CTOR_END__ symbols we define __init_array_start and __init_array_end
		* to the same address to define an empty list. This prevents the glibc
		* startup code from calling any global constructors before Zephyr loads.
		*/
		__init_array_start = .;
		__init_array_end = .;
#endif
	} GROUP_ROM_LINK_IN(RAMABLE_REGION, ROMABLE_REGION)

#ifndef CONFIG_STATIC_INIT_GNU
	ASSERT(__zephyr_init_array_start == __zephyr_init_array_end,
	       "GNU-style constructors required but STATIC_INIT_GNU not enabled")
#endif

#endif
