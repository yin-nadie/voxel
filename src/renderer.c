
#include "renderer.h"

#include <stdlib.h>
#include <assert.h>

void vertex2_update_vbo(const void *data, size_t size);
void vertex2_enable_client_states(void);
void vertex2_disable_client_states(void);

void vertex3_update_vbo(const void *data, size_t size);
void vertex3_enable_client_states(void);
void vertex3_disable_client_states(void);

const struct vertex_traits vertex2_traits = {
	vertex2_update_vbo,
	vertex2_enable_client_states,
	vertex2_disable_client_states
};

const struct vertex_traits vertex3_traits = {
	vertex3_update_vbo,
	vertex3_enable_client_states,
	vertex3_disable_client_states
};

void vertex2_update_vbo(const void *data, size_t size)
{
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(struct vertex2), data, GL_STATIC_DRAW);
}

void vertex2_enable_client_states(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex2), (char *)0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex2), (char *)offsetof(struct vertex2, u));
}

void vertex2_disable_client_states(void)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void vertex3_update_vbo(const void *data, size_t size)
{
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(struct vertex3), data, GL_STATIC_DRAW);
}

void vertex3_enable_client_states(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex3), (char *)0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex3), (char *)offsetof(struct vertex3, u));
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(struct vertex3), (char *)offsetof(struct vertex3, r));
}

void vertex3_disable_client_states(void)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

struct vertex3_buf *vertex3_buf(void)
{
	struct vertex3_buf *b = calloc(1, sizeof(*b));
	return b;
}

void vertex3_buf_destroy(struct vertex3_buf *b)
{
	if (b->data != NULL)
		free(b->data);
	free(b);
}

void vertex3_buf_push(struct vertex3_buf *buf, struct v3f p, struct v2f t, struct v4c c)
{
	if (buf->size == buf->alloc) {
		if (buf->alloc == 0)
			buf->alloc = 2;
		else
			buf->alloc *= 2;
		buf->data = realloc(buf->data, buf->alloc * sizeof(*buf->data));
	}
	buf->data[buf->size].x = p.x;
	buf->data[buf->size].y = p.y;
	buf->data[buf->size].z = p.z;
	buf->data[buf->size].u = t.x;
	buf->data[buf->size].v = t.y;
	buf->data[buf->size].r = c.x;
	buf->data[buf->size].g = c.y;
	buf->data[buf->size].b = c.z;
	buf->data[buf->size].a = c.w;
	++buf->size;
}

void vertex3_buf_left(struct vertex3_buf *buf, struct v3f p, GLfloat h, GLfloat d, struct aab2f t, int l)
{
	struct v4c c = v4c(l << 3, l << 3, l << 3, 255);
	struct v3f p0 = v3_add(p, v3f(0, 0, 0));
	struct v3f p1 = v3_add(p, v3f(0, 0, d));
	struct v3f p2 = v3_add(p, v3f(0, h, 0));
	struct v3f p3 = v3_add(p, v3f(0, h, d));
	struct v2f t0 = v2f(t.x1, t.y1);
	struct v2f t1 = v2f(t.x0, t.y1);
	struct v2f t2 = v2f(t.x1, t.y0);
	struct v2f t3 = v2f(t.x0, t.y0);
	vertex3_buf_push(buf, p0, t0, c);
	vertex3_buf_push(buf, p1, t1, c);
	vertex3_buf_push(buf, p2, t2, c);
	vertex3_buf_push(buf, p2, t2, c);
	vertex3_buf_push(buf, p1, t1, c);
	vertex3_buf_push(buf, p3, t3, c);
}

void vertex3_buf_right(struct vertex3_buf *buf, struct v3f p, GLfloat h, GLfloat d, struct aab2f t, int l)
{
	struct v4c c = v4c(l << 3, l << 3, l << 3, 255);
	struct v3f p0 = v3_add(p, v3f(0, h, d));
	struct v3f p1 = v3_add(p, v3f(0, 0, d));
	struct v3f p2 = v3_add(p, v3f(0, h, 0));
	struct v3f p3 = v3_add(p, v3f(0, 0, 0));
	struct v2f t0 = v2f(t.x0, t.y0);
	struct v2f t1 = v2f(t.x0, t.y1);
	struct v2f t2 = v2f(t.x1, t.y0);
	struct v2f t3 = v2f(t.x1, t.y1);
	vertex3_buf_push(buf, p0, t0, c);
	vertex3_buf_push(buf, p1, t1, c);
	vertex3_buf_push(buf, p2, t2, c);
	vertex3_buf_push(buf, p2, t2, c);
	vertex3_buf_push(buf, p1, t1, c);
	vertex3_buf_push(buf, p3, t3, c);
}

void vertex3_buf_down(struct vertex3_buf *buf, struct v3f p, GLfloat w, GLfloat d, struct aab2f t, int l)
{
	struct v4c c = v4c(l << 1, l << 1, l << 1, 255);
	struct v3f p0 = v3_add(p, v3f(0, 0, 0));
	struct v3f p1 = v3_add(p, v3f(w, 0, 0));
	struct v3f p2 = v3_add(p, v3f(0, 0, d));
	struct v3f p3 = v3_add(p, v3f(w, 0, d));
	struct v2f t0 = v2f(t.x1, t.y1);
	struct v2f t1 = v2f(t.x0, t.y1);
	struct v2f t2 = v2f(t.x1, t.y0);
	struct v2f t3 = v2f(t.x0, t.y0);
	vertex3_buf_push(buf, p0, t0, c);
	vertex3_buf_push(buf, p1, t1, c);
	vertex3_buf_push(buf, p2, t2, c);
	vertex3_buf_push(buf, p2, t2, c);
	vertex3_buf_push(buf, p1, t1, c);
	vertex3_buf_push(buf, p3, t3, c);
}

void vertex3_buf_up(struct vertex3_buf *buf, struct v3f p, GLfloat w, GLfloat d, struct aab2f t, int l)
{
	struct v4c c = v4c(l << 4, l << 4, l << 4, 255);
	struct v3f p0 = v3_add(p, v3f(w, 0, d));
	struct v3f p1 = v3_add(p, v3f(w, 0, 0));
	struct v3f p2 = v3_add(p, v3f(0, 0, d));
	struct v3f p3 = v3_add(p, v3f(0, 0, 0));
	struct v2f t0 = v2f(t.x1, t.y1);
	struct v2f t1 = v2f(t.x0, t.y1);
	struct v2f t2 = v2f(t.x1, t.y0);
	struct v2f t3 = v2f(t.x0, t.y0);
	vertex3_buf_push(buf, p0, t0, c);
	vertex3_buf_push(buf, p1, t1, c);
	vertex3_buf_push(buf, p2, t2, c);
	vertex3_buf_push(buf, p2, t2, c);
	vertex3_buf_push(buf, p1, t1, c);
	vertex3_buf_push(buf, p3, t3, c);
}

void vertex3_buf_back(struct vertex3_buf *buf, struct v3f p, GLfloat w, GLfloat h, struct aab2f t, int l)
{
	struct v4c c = v4c(l << 2, l << 2, l << 2, 255);
	struct v3f p0 = v3_add(p, v3f(w, h, 0));
	struct v3f p1 = v3_add(p, v3f(w, 0, 0));
	struct v3f p2 = v3_add(p, v3f(0, h, 0));
	struct v3f p3 = v3_add(p, v3f(0, 0, 0));
	struct v2f t0 = v2f(t.x0, t.y0);
	struct v2f t1 = v2f(t.x0, t.y1);
	struct v2f t2 = v2f(t.x1, t.y0);
	struct v2f t3 = v2f(t.x1, t.y1);
	vertex3_buf_push(buf, p0, t0, c);
	vertex3_buf_push(buf, p1, t1, c);
	vertex3_buf_push(buf, p2, t2, c);
	vertex3_buf_push(buf, p2, t2, c);
	vertex3_buf_push(buf, p1, t1, c);
	vertex3_buf_push(buf, p3, t3, c);
}

void vertex3_buf_front(struct vertex3_buf *buf, struct v3f p, GLfloat w, GLfloat h, struct aab2f t, int l)
{
	struct v4c c = v4c(l << 2, l << 2, l << 2, 255);
	struct v3f p0 = v3_add(p, v3f(0, 0, 0));
	struct v3f p1 = v3_add(p, v3f(w, 0, 0));
	struct v3f p2 = v3_add(p, v3f(0, h, 0));
	struct v3f p3 = v3_add(p, v3f(w, h, 0));
	struct v2f t0 = v2f(t.x1, t.y1);
	struct v2f t1 = v2f(t.x0, t.y1);
	struct v2f t2 = v2f(t.x1, t.y0);
	struct v2f t3 = v2f(t.x0, t.y0);
	vertex3_buf_push(buf, p0, t0, c);
	vertex3_buf_push(buf, p1, t1, c);
	vertex3_buf_push(buf, p2, t2, c);
	vertex3_buf_push(buf, p2, t2, c);
	vertex3_buf_push(buf, p1, t1, c);
	vertex3_buf_push(buf, p3, t3, c);
}

struct renderer *renderer(int vbo_count, const struct vertex_traits *traits)
{
	struct renderer *r = calloc(1, sizeof(*r));
	r->traits = traits;
	r->vbo_count = vbo_count;
	r->vbo_names = calloc(vbo_count, sizeof(*r->vbo_names));
	r->vbo_sizes = calloc(vbo_count, sizeof(*r->vbo_sizes));
	glGenBuffers(r->vbo_count, r->vbo_names);
	assert(glGetError() == GL_NO_ERROR);
	return r;
}

void renderer_destroy(struct renderer *r)
{
	assert(glGetError() == GL_NO_ERROR);
	glDeleteBuffers(r->vbo_count, r->vbo_names);
	free(r->vbo_names);
	free(r->vbo_sizes);
	free(r);
}

void renderer_begin(struct renderer *r)
{
	assert(glGetError() == GL_NO_ERROR);
}

void renderer_buffer(struct renderer *r, GLenum mode, size_t buf)
{
	assert(buf < r->vbo_count);
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo_names[buf]);
	r->traits->enable_client_states();
	glDrawArrays(mode, 0, r->vbo_sizes[buf]);
	r->traits->disable_client_states();
	assert(glGetError() == GL_NO_ERROR);
}

void renderer_end(struct renderer *r)
{
	assert(glGetError() == GL_NO_ERROR);
}

void renderer_update(struct renderer *r, size_t buf, const void *data, size_t size)
{
	assert(glGetError() == GL_NO_ERROR);
	assert(buf < r->vbo_count);
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo_names[buf]);
	r->traits->update_vbo(data, size);
	r->vbo_sizes[buf] = size;
	assert(glGetError() == GL_NO_ERROR);
}


