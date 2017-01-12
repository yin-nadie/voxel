
#include "flowsim.h"

#include <stdio.h>

#include "context.h"

void merge_volumes(struct fs_volume *v1, struct fs_volume *v2)
{
	printf("%s %llu %llu\n", __func__, v1->id, v2->id);
	list_merge(&v1->layers, &v2->layers);
	v1->v += v2->v;
	list_unlink(&v2->volumes);
	fs_volume_destroy(v2);
}

void merge_layers(struct fs_layer *l1, struct fs_layer *l2)
{
	printf("%s %llu %llu\n", __func__, l1->id, l2->id);
	struct v3ll p;

	list_unlink(&l2->layers);
	stack_foreach(p, l2->cells)
		stack_push(l1->cells, &p);
	if (l1->v != l2->v)
		merge_volumes(l1->v, l2->v);
	fs_layer_destroy(l2);
}

static inline void check_boundary(struct fs_layer *l, struct fs_layer *l2,
    struct stack *s, struct v3ll p)
{
	if (WORLD_AT(l->v->w, shape, p.x, p.y, p.z) != SHAPE_NONE)
		return;
	if (WORLD_AT(l->v->w, data, p.x, p.y, p.z) == l)
		return;
	if (WORLD_AT(l->v->w, data, p.x, p.y - 1, p.z) != l2)
		return;
	WORLD_AT(l->v->w, data, p.x, p.y, p.z) = l;
	stack_push(s, &p);
	stack_push(l->cells, &p);
}

static void layer_from_layer(struct fs_layer *l, struct fs_layer *l2,
    struct v3ll p)
{
	printf("%s %llu\n", __func__, l->id);
	struct stack *s = stack(sizeof(struct v3ll));
	check_boundary(l, l2, s, p);
	while (stack_size(s)) {
		stack_pop(s, &p);
		check_boundary(l, l2, s, v3ll(p.x - 1, p.y, p.z));
		check_boundary(l, l2, s, v3ll(p.x + 1, p.y, p.z));
		check_boundary(l, l2, s, v3ll(p.x, p.y, p.z - 1));
		check_boundary(l, l2, s, v3ll(p.x, p.y, p.z + 1));
	}
}

static inline void test_fall(struct fs_layer *l, struct v3ll p)
{
	if (WORLD_AT(l->v->w, shape, p.x, p.y, p.z) != SHAPE_NONE)
		return;
	if (WORLD_AT(l->v->w, data, p.x, p.y, p.z) == l)
		return;
	if (WORLD_AT(l->v->w, shape, p.x, p.y - 1, p.z) != SHAPE_NONE)
		return;
	stack_push(l->falls, &p);
}

static void calculate_falls(struct fs_layer *l)
{
//      printf("%s %d\n", __func__, l->id);
	struct v3ll p;
	stack_clear(l->falls);
	stack_foreach(p, l->cells) {
		test_fall(l, v3ll(p.x - 1, p.y, p.z));
		test_fall(l, v3ll(p.x + 1, p.y, p.z));
		test_fall(l, v3ll(p.x, p.y, p.z - 1));
		test_fall(l, v3ll(p.x, p.y, p.z + 1));
	}
//      printf("#falls=%u\n", stack_size(l->falls));
}

static inline void test_cell(struct fs_layer *l, struct v3ll p)
{
	struct fs_layer *l2;
	if (WORLD_AT(l->v->w, shape, p.x, p.y, p.z) != SHAPE_NONE)
		return;
	l2 = WORLD_AT(l->v->w, data, p.x, p.y, p.z);
	if (l2 == l)
		return;
	if (l2 != NULL) {
		merge_layers(l, l2);
		return;
	}
	if (WORLD_AT(l->v->w, shape, p.x, p.y - 1, p.z) == SHAPE_NONE)
		return;
	WORLD_AT(l->v->w, data, p.x, p.y, p.z) = l;
	stack_push(l->cells, &p);
}

static void expand_layer(struct fs_layer *l)
{
//      printf("%s %d\n", __func__, l->id);
	struct v3ll p;
	struct stack *tmp = l->cells;
	l->cells = stack(sizeof(struct v3ll));
	stack_foreach(p, tmp) {
		stack_push(l->cells, &p);
		test_cell(l, v3ll(p.x - 1, p.y, p.z));
		test_cell(l, v3ll(p.x + 1, p.y, p.z));
		test_cell(l, v3ll(p.x, p.y, p.z - 1));
		test_cell(l, v3ll(p.x, p.y, p.z + 1));
	}
	stack_destroy(tmp);
//      printf("#cells=%u\n", stack_size(l->cells));
}

struct fs_layer *fs_layer(struct fs_volume *v, int y)
{
	struct fs_layer *l = calloc(1, sizeof(*l));
	l->v = v;
	l->cells = stack(sizeof(struct v3ll));
	l->falls = stack(sizeof(struct v3ll));
	l->y = y;
	l->id = next_id();
	printf("%s %llu\n", __func__, l->id);
	return l;
}

void fs_layer_destroy(struct fs_layer *l)
{
	printf("%s %llu\n", __func__, l->id);
	struct v3ll p;
	stack_foreach(p, l->cells)
		WORLD_AT(l->v->w, data, p.x, p.y, p.z) = NULL;
	stack_destroy(l->cells);
	stack_destroy(l->falls);
	free(l);
}

struct fs_volume *fs_volume(struct world *w)
{
	struct fs_volume *v = calloc(1, sizeof(*v));
	v->w = w;
	list_init(&v->layers);
	v->id = next_id();
	printf("%s %llu\n", __func__, v->id);
	return v;
}

void fs_volume_destroy(struct fs_volume *v)
{
	struct fs_layer *l;
	printf("%s %llu\n", __func__, v->id);
	list_foreach(l, &v->layers, layers)
		fs_layer_destroy(l);
	free(v);
}

/* unlink this top layers search new ones over it */
static void push_layer(struct fs_volume *v, struct fs_layer *l)
{
	struct v3ll p;
	struct fs_layer *l2;
	printf("%s %llu\n", __func__, v->id);
	stack_foreach(p, l->cells) {
		++p.y;
		if (WORLD_AT(v->w, shape, p.x, p.y, p.z) == SHAPE_NONE) {
			l2 = WORLD_AT(v->w, data, p.x, p.y, p.z);
			if (l2 != NULL) {
				if (l->v != l2->v)
					merge_volumes(l->v, l2->v);
			} else {
				l2 = fs_layer(v, p.y);
				layer_from_layer(l2, l, p);
				l2->is_top = 1;
				list_prepend(&v->layers, &l2->layers);
				printf("%d.is_top=1\n", l2->id);
			}
		}
	}
	l->is_top = 0;
}

/* unlink and destroy this top layer and search for new ones below it */
static void pop_layer(struct fs_volume *v, struct fs_layer *l)
{
	struct v3ll p;
	struct fs_layer *l2;
	printf("%s %llu\n", __func__, l->id);
	stack_foreach(p, l->cells) {
		--p.y;
		if (WORLD_AT(v->w, shape, p.x, p.y, p.z) == SHAPE_NONE) {
			l2 = WORLD_AT(v->w, data, p.x, p.y, p.z);
			if (l2 != NULL && !l2->is_top) {
				l2->is_top = 1;
				list_prepend(&v->layers, &l2->layers);
				printf("%d.is_top=1\n", l2->id);
			}
		}
	}
	list_unlink(&l->layers);
	fs_layer_destroy(l);
}

void fs_volume_step(struct fs_volume *v)
{
	struct fs_layer *l;
	float top, bottom;
	top = 0;
	bottom = 0;
	list_foreach(l, &v->layers, layers) {
		assert(l->id < max_id());
		if (l->is_top) {
			top += stack_size(l->cells);
		} else {
			bottom += stack_size(l->cells);
		}
	}
	v->top = (v->v - bottom) / top;
	list_foreach_safe(l, &v->layers, layers) {
		assert(l->id < max_id());
		if (!l->is_top || v->top > (1. / 64.))
			expand_layer(l);
		calculate_falls(l);
	}
	if (v->top > 1) {
		list_foreach_safe(l, &v->layers, layers) {
		assert(l->id < max_id());
			if (l->is_top)
				push_layer(v, l);
		}
		--v->top;
	} else if (v->top < 0) {
		list_foreach_safe(l, &v->layers, layers) {
		assert(l->id < max_id());
			if (l->is_top)
				pop_layer(v, l);
		}
		++v->top;
	}
}

struct flowsim *flowsim(struct world *w)
{
	struct flowsim *f = calloc(1, sizeof(*f));
	f->w = w;
	list_init(&f->volumes);
	return f;
}

void flowsim_destroy(struct flowsim *f)
{
	free(f);
}

void flowsim_step(struct flowsim *f)
{
	struct fs_volume *v;
	struct fs_layer *l;
	struct v3ll p;
	list_foreach(v, &f->volumes, volumes) {
		list_foreach(l, &v->layers, layers) {
			float a = stack_size(l->falls) / 16.;
			float b = stack_size(l->cells) * l->v->top;
			if (a > b)
				a = b;
			b = a / stack_size(l->falls);
			stack_foreach(p, l->falls)
				flowsim_add(f, p, b);
			l->v->v -= a;
		}
	}
	list_foreach_safe(v, &f->volumes, volumes) {
		if (v->v <= 0) {
			list_unlink(&v->volumes);
			fs_volume_destroy(v);
		}
	}
	list_foreach(v, &f->volumes, volumes)
		fs_volume_step(v);
}

int flowsim_add(struct flowsim *f, struct v3ll p, float k)
{
	struct fs_layer *l;
	struct fs_volume *v;
	if (k == 0)
		return 0;
	while (WORLD_AT(f->w, shape, p.x, p.y, p.z) == SHAPE_NONE) {
		l = WORLD_AT(f->w, data, p.x, p.y, p.z);
		if (l != NULL) {
			l->v->v += k;
			return 1;
		}
		--p.y;
	}
	++p.y;
	v = fs_volume(f->w);
	list_append(&f->volumes, &v->volumes);
	l = fs_layer(v, p.y);
	stack_push(l->cells, &p);
	WORLD_AT(f->w, data, p.x, p.y, p.z) = l;
	calculate_falls(l);
	l->is_top = 1;
	list_prepend(&v->layers, &l->layers);
	v->v += k;
	return 1;
}