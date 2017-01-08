
#ifndef VOXEL_RENDERER_H_
#define VOXEL_RENDERER_H_

#define GL_GLEXT_PROTOTYPES 1
#include "SDL2/SDL_opengl.h"
#include "GL/glext.h"
#undef GL_GLEXT_PROTOTYPES

#include "v2.h"
#include "v3.h"
#include "v4.h"
#include "aab2.h"

struct vertex_traits {
	void(*update_vbo)(const void *data, size_t size);
	void(*enable_client_states)(void);
	void(*disable_client_states)(void);
};

struct vertex2 {
	GLfloat x;
	GLfloat y;
	GLfloat u;
	GLfloat v;
};

struct vertex3 {
	GLfloat x, y, z;
	GLfloat u0, v0;
	GLfloat u1, v1;
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct vertex3_buf {
	struct vertex3 *data;
	size_t size;
	size_t alloc;
};

struct renderer {
	const struct vertex_traits *traits;
	size_t vbo_count;
	GLuint *vbo_names;
	size_t *vbo_sizes;
};

extern const struct vertex_traits vertex2_traits;
extern const struct vertex_traits vertex3_traits;

static inline struct vertex2 vertex2(GLfloat x, GLfloat y, GLfloat u, GLfloat v)
{
	return (struct vertex2){ x, y, u, v };
}

static inline struct vertex3 vertex3(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v)
{
	return (struct vertex3){ x, y, z, u, v };
}

struct vertex3_buf *vertex3_buf(void);
void vertex3_buf_destroy(struct vertex3_buf *b);
void vertex3_buf_push(struct vertex3_buf *buf, struct v3f v, struct v2f t0, struct v2f t1, struct v4c c);
void vertex3_buf_left(struct vertex3_buf *buf, struct v3f, GLfloat h, GLfloat d, struct v2f *lt, struct v2f *mt);
void vertex3_buf_right(struct vertex3_buf *buf, struct v3f, GLfloat h, GLfloat d, struct v2f *lt, struct v2f *mt);
void vertex3_buf_down(struct vertex3_buf *buf, struct v3f, GLfloat w, GLfloat d, struct v2f *lt, struct v2f *mt);
void vertex3_buf_up(struct vertex3_buf *buf, struct v3f, GLfloat w, GLfloat d, struct v2f *lt, struct v2f *mt);
void vertex3_buf_back(struct vertex3_buf *buf, struct v3f, GLfloat w, GLfloat h, struct v2f *lt, struct v2f *mt);
void vertex3_buf_front(struct vertex3_buf *buf, struct v3f, GLfloat w, GLfloat h, struct v2f *lt, struct v2f *mt);

struct renderer *renderer(int nbufs, const struct vertex_traits *traits);
void renderer_destroy(struct renderer *r);
void renderer_begin(struct renderer *r);
void renderer_buffer(struct renderer *r, GLenum mode, size_t buf);
void renderer_end(struct renderer *r);
void renderer_update(struct renderer *r, size_t buf, const void *data, size_t size);

#endif

