/* -------------------------------------------------------------------------- *
 * slot.h: Memory slot.                                                       *
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
#ifndef SLOT_H
#define SLOT_H

#include <stdatomic.h>

#define SLOT_MASK 0b1
#define SLOT_FREE 0
#define SLOT_WAIT 1

typedef atomic_intptr_t slot_t;

#define slot_init(sl, type)                                                    \
	({                                                                     \
		static_assert(alignof(type) > SLOT_MASK);                      \
		atomic_init(sl, 0);                                            \
	})

#define SLOT_NEW(name, type)                                                   \
	slot_t name;                                                           \
	slot_init(&name, type);

bool slot_insert(slot_t *sl, void *data);

bool slot_remove(slot_t *sl, void **data);

#endif /* SLOT_H */
