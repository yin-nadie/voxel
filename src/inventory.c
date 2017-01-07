

#include "inventory.h"

#include <string.h>

struct inventory *inventory(size_t size)
{
	struct inventory *i = calloc(1, sizeof(*i));
	inventory_resize(i, size);
	return i;
}

void inventory_destroy(struct inventory *i)
{
	if (i->slots != NULL)
		free(i);
	free(i);
}

int inventory_resize(struct inventory *i, size_t size)
{
	i->slots = realloc(i->slots, sizeof(*i->slots) * size);
	if (size > i->size)
		memset(i->slots + i->size, 0, sizeof(*i->slots) * (size - i->size));
	i->size = size;
	return 0;
}

int inventory_add(struct inventory *i, int obj, int mat, int num)
{
	int slot = 0, acc = 0;
	while (slot < i->size && acc < num) {
		acc += inventory_add_to_slot(i, slot, obj, mat, num - acc);
		++slot;
	}
	slot = 0;
	while (slot < i->size && acc < num) {
		if (i->slots[slot].num == 0) {
			i->slots[slot].obj = obj;
			i->slots[slot].mat = mat;
			i->slots[slot].num = num - acc;
			acc = num;
		}
		++slot;
	}
	return acc;
}

int inventory_add_to_slot(struct inventory *i, int slot, int obj, int mat, int num)
{
	if (i->slots[slot].obj == obj && i->slots[slot].mat == mat) {
		if (num <= 64 - i->slots[slot].num) {
			i->slots[slot].num += num;
			return num;
		}
		num -= 64 - i->slots[slot].num;
		i->slots[slot].num = 64;
		return num;
	}
	return 0;
}

int inventory_remove(struct inventory *i, int obj, int mat, int num)
{
	int slot = 0, acc = 0;
	while (slot < i->size && acc < num) {
		acc += inventory_remove_from_slot(i, slot, obj, mat, num - acc);
		++slot;
	}
	return acc;
}

int inventory_remove_from_slot(struct inventory  *i, int slot, int obj, int mat, int num)
{
	if (i->slots[slot].obj == obj && i->slots[slot].mat == mat) {
		if (i->slots[slot].num >= num) {
			i->slots[slot].num -= num;
			return num;
		}
		num += i->slots[slot].num;
		i->slots[slot].num = 0;
		return num;
	}
	return 0;
}

