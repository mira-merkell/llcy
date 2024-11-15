/* -------------------------------------------------------------------------- *
 * test.h: Test macros.                                                       *
 *                                                                            *
 * Copyright 2024 Marek Miller                                                *
 *                                                                            *
 * This program is free software: you can redistribute it and/or modify it    *
 * under the terms of the GNU General Public License as published by the      *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * This program is distributed in the hope that it will be useful, but        *
 * WITHOUT ANY WARRANTY* without even the implied warranty of MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   *
 * for more details.                                                          *
 *                                                                            *
 * You should have received a copy of the GNU General Public License along    *
 * with this program.  If not, see <https://www.gnu.org/licenses/>.           *
 * -------------------------------------------------------------------------- */
#ifndef TEST_H
#define TEST_H

#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>

static _Atomic size_t TEST_RT = 0;

#define TEST_FAIL(fmt, ...)                                                    \
	({                                                                     \
		fprintf(stderr, "%s:%d %s() FAIL \"" fmt "\"\n", __FILE__,     \
			__LINE__, __func__ __VA_OPT__(, ) __VA_ARGS__);        \
		atomic_fetch_add(&TEST_RT, 1);                                 \
	})

#define TEST_STR(a) #a
#define TEST_XSTR(a) TEST_STR(a)

#define TEST_ASSERT(expr)                                                      \
	({                                                                     \
		if (!(expr))                                                   \
			TEST_FAIL("assert: %s", TEST_XSTR(expr));              \
	})

#define TEST_EQ(a, b) TEST_ASSERT((a) == (b)

#define TEST_RET() (!!TEST_RT)

#endif /* TEST_H */
