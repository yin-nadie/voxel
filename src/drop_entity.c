
#include "drop_entity.h"

#include "render.h"
#include "block_entity.h"

void *drop_entity_create(struct context *ctx);
void drop_entity_destroy(void *data);
void drop_entity_update(void *data);
void drop_entity_render(void *data);
int drop_entity_save(void *data, union sz_tag *root);
int drop_entity_load(void *data, char *key, union sz_tag *val);

struct entity_traits drop_traits = {
	.name = "drop_entity",
	.create_func = drop_entity_create,
	.update_func = drop_entity_update,
	.render_func = drop_entity_render,
	.destroy_func = drop_entity_destroy,
	.save_func = drop_entity_save,
	.load_func = drop_entity_load,
};

void drop_entity_callback(struct body *b, void *udata, struct v3ll p, int face)
{
	struct drop_entity *d = udata;
	if (face != FACE_UP)
		return;
	if (d->item.num) {
		struct block_entity *e = world_get_data(d->roaming.entity.ctx->w, p);
		if (e != NULL && e->entity.items != NULL)
			d->item.num -= inventory_add(e->entity.items, d->item);
	}
	b->v.x *= .8;
	b->v.z *= .8;
}

struct drop_entity *drop_entity(struct context *ctx, struct item item)
{
	struct drop_entity *d = drop_entity_create(ctx);
	d->item = item;
	return d;
}

void *drop_entity_create(struct context *ctx)
{
	struct drop_entity *d = calloc(1, sizeof(*d));
	d->roaming.entity.traits = &drop_traits;
	d->roaming.body = body(ctx->space);
	list_link(&ctx->entities, &d->roaming.entities);
	body_set_size(d->roaming.body, v2f(.0625, .0625));
	body_set_callback(d->roaming.body, drop_entity_callback, d);
	d->roaming.entity.ctx = ctx;
	return d;
}

void drop_entity_destroy(void *data)
{
	struct drop_entity *d = data;
	list_unlink(&d->roaming.entities);
	body_destroy(d->roaming.body);
	free(d);
}

void drop_entity_update(void *data)
{
	struct drop_entity *d = data;
	++d->ticks;
	if (d->ticks > 10 && box3_overlap(box3_grow(d->roaming.body->bb, 1), d->roaming.entity.ctx->player->bb))
		d->item.num -= inventory_add(d->roaming.entity.ctx->inv, d->item);
	if (d->ticks > 1800 || d->item.num == 0)
		d->roaming.die = 1;
}

void drop_entity_render(void *data)
{
	struct drop_entity *d = data;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(d->roaming.body->p.x, d->roaming.body->p.y, d->roaming.body->p.z);
	glScalef(.25, .25, .25);
	render_obj(d->roaming.entity.ctx, d->item.obj, d->item.mat, 255);
	glPopMatrix();
}

int drop_entity_save(void *data, union sz_tag *root)
{/*
	sz_dict_add(root, "y", sz_i64(s->y));
	sz_dict_add(root, "mat", sz_raw(s->mat, SHARD_VOLUME));
	sz_dict_add(root, "shape", sz_raw(s->shape, SHARD_VOLUME));
	sz_dict_add(root, "light", sz_raw(s->light, SHARD_VOLUME));*/
	return 0;
}

int drop_entity_load(void *data, char *key, union sz_tag *val)
{/*
	char *key;
	union sz_tag *val;
	sz_dict_foreach(key, val, root) {
		if (strcmp(key, "y") == 0) {
			s->y = val->i64.data;
		} else if (strcmp(key, "mat") == 0) {
			memcpy(s->mat, val->raw.data, val->raw.size);
		} else if (strcmp(key, "shape") == 0) {
			memcpy(s->shape, val->raw.data, val->raw.size);
		} else if (strcmp(key, "light") == 0) {
			memcpy(s->light, val->raw.data, val->raw.size);
		} else {
			log_error("bad tag: %s", key);
		}
	}*/
	return 0;
}