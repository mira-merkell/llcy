#include <stdatomic.h>

#include "llcy/slot.h"

bool slot_insert(slot_t *sl, void *data)
{
	intptr_t slot = atomic_load_explicit(sl, memory_order_acquire);
	if ((slot & SLOT_MASK) != SLOT_FREE)
		return false;

	return atomic_compare_exchange_strong_explicit(sl, &slot,
		(intptr_t)data | SLOT_WAIT, memory_order_acq_rel,
		memory_order_relaxed);
}

bool slot_remove(slot_t *sl, void **data)
{
	intptr_t slot = atomic_load_explicit(sl, memory_order_acquire);
	if ((slot & SLOT_MASK) != SLOT_WAIT)
		return false;

	if (!atomic_compare_exchange_strong_explicit(sl, &slot,
		    (slot & ~SLOT_MASK) | SLOT_FREE, memory_order_acq_rel,
		    memory_order_relaxed))
		return false;
	*data = (void *)(slot & ~SLOT_MASK);

	return true;
}
