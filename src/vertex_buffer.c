
#include "vertex_buffer.h"

#include <assert.h>
#include <stdlib.h>

#define T_COL 1.
#define X_COL .8
#define Z_COL .6
#define B_COL .4

const struct v2f texcoord_from_mat[][3] = {
	{ { 0 / 16., 0 / 16. }, { 0 / 16., 1 / 16. }, { 0 / 16., 2 / 16. } },
	{ { 1 / 16., 0 / 16. }, { 1 / 16., 1 / 16. }, { 1 / 16., 2 / 16. } },
	{ { 2 / 16., 0 / 16. }, { 2 / 16., 1 / 16. }, { 2 / 16., 2 / 16. } },
	{ { 3 / 16., 0 / 16. }, { 3 / 16., 1 / 16. }, { 3 / 16., 2 / 16. } },
	{ { 4 / 16., 0 / 16. }, { 4 / 16., 1 / 16. }, { 4 / 16., 2 / 16. } },
	{ { 5 / 16., 0 / 16. }, { 5 / 16., 1 / 16. }, { 5 / 16., 2 / 16. } },
	{ { 6 / 16., 0 / 16. }, { 6 / 16., 1 / 16. }, { 6 / 16., 2 / 16. } },
	{ { 7 / 16., 0 / 16. }, { 7 / 16., 1 / 16. }, { 7 / 16., 2 / 16. } },
	{ { 8 / 16., 0 / 16. }, { 8 / 16., 1 / 16. }, { 8 / 16., 2 / 16. } },
	{ { 9 / 16., 0 / 16. }, { 9 / 16., 1 / 16. }, { 9 / 16., 2 / 16. } },
	{ { 10 / 16., 0 / 16. }, { 10 / 16., 1 / 16. }, { 10 / 16., 2 / 16. } },
};

struct vertex_desc vertices_face_dn[] = {
	{ 0., 0., 0., 0., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 1., 1., 0., B_COL, B_COL, B_COL, 2 },
};

struct vertex_desc vertices_face_up[] = {
	{ 1., 0., 1., 0., 1., T_COL, T_COL, T_COL, 1 },
	{ 1., 0., 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., 0., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 0., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 1., 0., 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., 0., 0., 1., 0., T_COL, T_COL, T_COL, 1 },
};

struct vertex_desc vertices_face_lf[] = {
	{ 0., 0., 0., 0., 1., X_COL, X_COL, X_COL },
	{ 0., 0., 1., 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., 0., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 1., 0., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 0., 1., 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., 1., 1., 0., X_COL, X_COL, X_COL },
};

struct vertex_desc vertices_face_rt[] = {
	{ 0., 0., 1., 0., 1., X_COL, X_COL, X_COL },
	{ 0., 0., 0., 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., 1., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 1., 1., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 0., 0., 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., 0., 1., 0., X_COL, X_COL, X_COL },
};

struct vertex_desc vertices_face_bk[] = {
	{ 1., 0., 0., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 0., 1., 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_face_ft[] = {
	{ 0., 0., 0., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 1., 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_slab_dn[] = {
	{ 1., .5, 1., 0., 1., T_COL, T_COL, T_COL, 1 },
	{ 1., .5, 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., .5, 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., .5, 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 1., .5, 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., .5, 0., 1., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 0., 0., 0., .5, X_COL, X_COL, X_COL },
	{ 0., 0., 1., 1., .5, X_COL, X_COL, X_COL },
	{ 0., .5, 0., 0., 0., X_COL, X_COL, X_COL },
	{ 0., .5, 0., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 0., 1., 1., .5, X_COL, X_COL, X_COL },
	{ 0., .5, 1., 1., 0., X_COL, X_COL, X_COL },
	{ 1., 0., 1., 0., .5, X_COL, X_COL, X_COL },
	{ 1., 0., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 1., .5, 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., .5, 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., 0., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 1., .5, 0., 1., 0., X_COL, X_COL, X_COL },
	{ 1., 0., 0., 0., .5, Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 1., .5, 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., .5, 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 0., .5, 0., 1., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 1., 0., .5, Z_COL, Z_COL, Z_COL },
	{ 1., 0., 1., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 0., .5, 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., .5, 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., 0., 1., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 1., .5, 1., 1., 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_slab_up[] = {
	{ 0., .5, 0., 0., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., .5, 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 0., .5, 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., .5, 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., .5, 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., .5, 1., 1., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., .5, 0., 0., 1., X_COL, X_COL, X_COL },
	{ 0., .5, 1., 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., 0., 0., .5, X_COL, X_COL, X_COL },
	{ 0., 1., 0., 0., .5, X_COL, X_COL, X_COL },
	{ 0., .5, 1., 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., 1., 1., .5, X_COL, X_COL, X_COL },
	{ 1., .5, 1., 0., 1., X_COL, X_COL, X_COL },
	{ 1., .5, 0., 1., 1., X_COL, X_COL, X_COL },
	{ 1., 1., 1., 0., .5, X_COL, X_COL, X_COL },
	{ 1., 1., 1., 0., .5, X_COL, X_COL, X_COL },
	{ 1., .5, 0., 1., 1., X_COL, X_COL, X_COL },
	{ 1., 1., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 1., .5, 0., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 0., .5, 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 0., .5, Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 0., .5, Z_COL, Z_COL, Z_COL },
	{ 0., .5, 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 0., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 0., .5, 1., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 1., .5, 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 0., .5, Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 0., .5, Z_COL, Z_COL, Z_COL },
	{ 1., .5, 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 1., 1., .5, Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_slab_lf[] = {
	{ 0., 0., 0., 0., 1., B_COL, B_COL, B_COL, 2 },
	{ .5, 0., 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ .5, 0., 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ .5, 0., 1., 1., 0., B_COL, B_COL, B_COL, 2 },
	{ .5, 1., 1., 0., 1., T_COL, T_COL, T_COL, 1 },
	{ .5, 1., 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ .5, 1., 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., 0., 1., 0., T_COL, T_COL, T_COL, 1 },
	{ .5, 0., 1., 0., 1., X_COL, X_COL, X_COL },
	{ .5, 0., 0., 1., 1., X_COL, X_COL, X_COL },
	{ .5, 1., 1., 0., 0., X_COL, X_COL, X_COL },
	{ .5, 1., 1., 0., 0., X_COL, X_COL, X_COL },
	{ .5, 0., 0., 1., 1., X_COL, X_COL, X_COL },
	{ .5, 1., 0., 1., 0., X_COL, X_COL, X_COL },
	{ .5, 0., 0., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ .5, 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 0., 1., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 1., 0., 1., Z_COL, Z_COL, Z_COL },
	{ .5, 0., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, 0., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ .5, 1., 1., 1., 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_slab_rt[] = {
	{ .5, 0., 0., 0., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ .5, 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ .5, 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 1., 1., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., 1., 1., 0., 1., T_COL, T_COL, T_COL, 1 },
	{ 1., 1., 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ .5, 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ .5, 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 1., 1., 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ .5, 1., 0., 1., 0., T_COL, T_COL, T_COL, 1 },
	{ .5, 0., 0., 0., 1., X_COL, X_COL, X_COL },
	{ .5, 0., 1., 1., 1., X_COL, X_COL, X_COL },
	{ .5, 1., 0., 0., 0., X_COL, X_COL, X_COL },
	{ .5, 1., 0., 0., 0., X_COL, X_COL, X_COL },
	{ .5, 0., 1., 1., 1., X_COL, X_COL, X_COL },
	{ .5, 1., 1., 1., 0., X_COL, X_COL, X_COL },
	{ 1., 0., 0., 0., 1., Z_COL, Z_COL, Z_COL },
	{ .5, 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ .5, 1., 0., 1., 0., Z_COL, Z_COL, Z_COL },
	{ .5, 0., 1., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 0., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ .5, 1., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, 1., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., 0., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 1., 1., 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_slab_bk[] = {
	{ 0., 0., 0., 0., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., .5, 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., .5, 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., .5, 1., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., 1., .5, 0., 1., T_COL, T_COL, T_COL, 1 },
	{ 1., 1., 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., .5, 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., .5, 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 1., 1., 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., 0., 1., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 0., 0., 0., 1., X_COL, X_COL, X_COL },
	{ 0., 0., .5, 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., 0., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 1., 0., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 0., .5, 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., .5, 1., 0., X_COL, X_COL, X_COL },
	{ 1., 0., .5, 0., 1., X_COL, X_COL, X_COL },
	{ 1., 0., 0., 1., 1., X_COL, X_COL, X_COL },
	{ 1., 1., .5, 0., 0., X_COL, X_COL, X_COL },
	{ 1., 1., .5, 0., 0., X_COL, X_COL, X_COL },
	{ 1., 0., 0., 1., 1., X_COL, X_COL, X_COL },
	{ 1., 1., 0., 1., 0., X_COL, X_COL, X_COL },
	{ 0., 0., .5, 0., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 0., .5, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., .5, 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 1., .5, 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., 0., .5, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., .5, 1., 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_slab_ft[] = {
	{ 0., 0., .5, 0., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., .5, 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., .5, 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 1., 1., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., 1., 1., 0., 1., T_COL, T_COL, T_COL, 1 },
	{ 1., 1., .5, 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 1., 1., .5, 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., .5, 1., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 0., .5, 0., 1., X_COL, X_COL, X_COL },
	{ 0., 0., 1., 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., .5, 0., 0., X_COL, X_COL, X_COL },
	{ 0., 1., .5, 0., 0., X_COL, X_COL, X_COL },
	{ 0., 0., 1., 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., 1., 1., 0., X_COL, X_COL, X_COL },
	{ 1., 0., 1., 0., 1., X_COL, X_COL, X_COL },
	{ 1., 0., .5, 1., 1., X_COL, X_COL, X_COL },
	{ 1., 1., 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., 1., 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., 0., .5, 1., 1., X_COL, X_COL, X_COL },
	{ 1., 1., .5, 1., 0., X_COL, X_COL, X_COL },
	{ 1., 0., .5, 0., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 0., .5, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., .5, 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., 1., .5, 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 0., .5, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., .5, 1., 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_stairs_dl[] = {
	{ 1., .5, 1., 0., 1., T_COL, T_COL, T_COL, 1 },
	{ 1., .5, 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ .5, .5, 1., 0., .5, T_COL, T_COL, T_COL, 1 },
	{ .5, .5, 1., 0., .5, T_COL, T_COL, T_COL, 1 },
	{ 1., .5, 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ .5, .5, 0., 1., .5, T_COL, T_COL, T_COL, 1 },
	{ .5, 1., 1., 0., .5, T_COL, T_COL, T_COL, 1 },
	{ .5, 1., 0., 1., .5, T_COL, T_COL, T_COL, 1 },
	{ 0., 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ .5, 1., 0., 1., .5, T_COL, T_COL, T_COL, 1 },
	{ 0., 1., 0., 1., 0., T_COL, T_COL, T_COL, 1 },
	{ 1., 0., 1., 0., .5, X_COL, X_COL, X_COL },
	{ 1., 0., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 1., .5, 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., .5, 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., 0., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 1., .5, 0., 1., 0., X_COL, X_COL, X_COL },
	{ .5, .5, 1., 0., 1., X_COL, X_COL, X_COL },
	{ .5, .5, 0., 1., 1., X_COL, X_COL, X_COL },
	{ .5, 1., 1., 0., .5, X_COL, X_COL, X_COL },
	{ .5, 1., 1., 0., .5, X_COL, X_COL, X_COL },
	{ .5, .5, 0., 1., 1., X_COL, X_COL, X_COL },
	{ .5, 1., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 1., 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ .5, .5, 0., .5, .5, Z_COL, Z_COL, Z_COL },
	{ 1., .5, 0., 1., .5, Z_COL, Z_COL, Z_COL },
	{ .5, .5, 0., .5, .5, Z_COL, Z_COL, Z_COL },
	{ 0., 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, 1., 0., .5, 0., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 1., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 0., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, .5, 1., .5, .5, Z_COL, Z_COL, Z_COL },
	{ 1., 0., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., .5, 1., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, .5, 1., .5, .5, Z_COL, Z_COL, Z_COL },
	{ .5, 1., 1., .5, 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_stairs_dr[] = {
	{ 0., .5, 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., .5, 1., 0., 1., T_COL, T_COL, T_COL, 1 },
	{ .5, .5, 1., 0., .5, T_COL, T_COL, T_COL, 1 },
	{ 0., .5, 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ .5, .5, 1., 0., .5, T_COL, T_COL, T_COL, 1 },
	{ .5, .5, 0., 1., .5, T_COL, T_COL, T_COL, 1 },
	{ .5, 1., 0., 1., .5, T_COL, T_COL, T_COL, 1 },
	{ .5, 1., 1., 0., .5, T_COL, T_COL, T_COL, 1 },
	{ 1., 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ .5, 1., 0., 1., .5, T_COL, T_COL, T_COL, 1 },
	{ 1., 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 1., 1., 0., 1., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 0., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 0., 0., 1., 0., .5, X_COL, X_COL, X_COL },
	{ 0., .5, 1., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 0., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 0., .5, 1., 0., 0., X_COL, X_COL, X_COL },
	{ 0., .5, 0., 1., 0., X_COL, X_COL, X_COL },
	{ .5, .5, 0., 1., 1., X_COL, X_COL, X_COL },
	{ .5, .5, 1., 0., 1., X_COL, X_COL, X_COL },
	{ .5, 1., 1., 0., .5, X_COL, X_COL, X_COL },
	{ .5, .5, 0., 1., 1., X_COL, X_COL, X_COL },
	{ .5, 1., 1., 0., .5, X_COL, X_COL, X_COL },
	{ .5, 1., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 1., 0., 0., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, .5, 0., .5, .5, Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., .5, 0., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, .5, 0., .5, .5, Z_COL, Z_COL, Z_COL },
	{ .5, 1., 0., .5, 0., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 0., 1., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ .5, .5, 1., .5, .5, Z_COL, Z_COL, Z_COL },
	{ 0., .5, 1., 1., .5, Z_COL, Z_COL, Z_COL },
	{ .5, .5, 1., .5, .5, Z_COL, Z_COL, Z_COL },
	{ 1., 1., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, 1., 1., .5, 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_stairs_db[] = {
	{ 0., .5, 1., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 1., .5, 1., 0., 1., T_COL, T_COL, T_COL, 1 },
	{ 1., .5, .5, 0., .5, T_COL, T_COL, T_COL, 1 },
	{ 0., .5, 1., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 1., .5, .5, 0., .5, T_COL, T_COL, T_COL, 1 },
	{ 0., .5, .5, 1., .5, T_COL, T_COL, T_COL, 1 },
	{ 0., 1., .5, 1., .5, T_COL, T_COL, T_COL, 1 },
	{ 1., 1., .5, 0., .5, T_COL, T_COL, T_COL, 1 },
	{ 1., 1., 0., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., .5, 1., .5, T_COL, T_COL, T_COL, 1 },
	{ 1., 1., 0., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., 0., 1., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 0., 1., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 1., 0., 1., 0., .5, Z_COL, Z_COL, Z_COL },
	{ 1., .5, 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 1., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 1., .5, 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., .5, 1., 1., 0., Z_COL, Z_COL, Z_COL },
	{ 0., .5, .5, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., .5, .5, 0., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., .5, 0., .5, Z_COL, Z_COL, Z_COL },
	{ 0., .5, .5, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., .5, 0., .5, Z_COL, Z_COL, Z_COL },
	{ 0., 1., .5, 1., .5, Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 0., 1., X_COL, X_COL, X_COL },
	{ 0., 0., 1., 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., 0., 0., 0., X_COL, X_COL, X_COL },
	{ 0., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 0., 0., 1., 1., 1., X_COL, X_COL, X_COL },
	{ 0., .5, 1., 1., .5, X_COL, X_COL, X_COL },
	{ 0., 1., 0., 0., 0., X_COL, X_COL, X_COL },
	{ 0., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 0., 1., .5, .5, 0., X_COL, X_COL, X_COL },
	{ 1., 0., 1., 1., 1., X_COL, X_COL, X_COL },
	{ 1., 0., 0., 0., 1., X_COL, X_COL, X_COL },
	{ 1., 1., 0., 0., 0., X_COL, X_COL, X_COL },
	{ 1., 0., 1., 1., 1., X_COL, X_COL, X_COL },
	{ 1., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 1., .5, 1., 1., .5, X_COL, X_COL, X_COL },
	{ 1., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 1., 1., 0., 0., 0., X_COL, X_COL, X_COL },
	{ 1., 1., .5, .5, 0., X_COL, X_COL, X_COL },
};

struct vertex_desc vertices_stairs_df[] = {
	{ 1., .5, 0., 0., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., .5, 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 1., .5, .5, 0., .5, T_COL, T_COL, T_COL, 1 },
	{ 1., .5, .5, 0., .5, T_COL, T_COL, T_COL, 1 },
	{ 0., .5, 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., .5, .5, 1., .5, T_COL, T_COL, T_COL, 1 },
	{ 1., 1., .5, 0., .5, T_COL, T_COL, T_COL, 1 },
	{ 0., 1., .5, 1., .5, T_COL, T_COL, T_COL, 1 },
	{ 1., 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 1., 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., .5, 1., .5, T_COL, T_COL, T_COL, 1 },
	{ 0., 1., 1., 1., 0., T_COL, T_COL, T_COL, 1 },
	{ 1., 0., 0., 0., .5, Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 1., .5, 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., .5, 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 0., .5, 0., 1., 0., Z_COL, Z_COL, Z_COL },
	{ 1., .5, .5, 0., 1., Z_COL, Z_COL, Z_COL },
	{ 0., .5, .5, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., .5, 0., .5, Z_COL, Z_COL, Z_COL },
	{ 1., 1., .5, 0., .5, Z_COL, Z_COL, Z_COL },
	{ 0., .5, .5, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., .5, 1., .5, Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 1., 1., X_COL, X_COL, X_COL },
	{ 0., 0., 1., 0., 1., X_COL, X_COL, X_COL },
	{ 0., 1., 1., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 0., 0., 1., 1., X_COL, X_COL, X_COL },
	{ 0., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 0., .5, 0., 1., .5, X_COL, X_COL, X_COL },
	{ 0., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 0., 1., 1., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 1., .5, .5, 0., X_COL, X_COL, X_COL },
	{ 1., 0., 1., 0., 1., X_COL, X_COL, X_COL },
	{ 1., 0., 0., 1., 1., X_COL, X_COL, X_COL },
	{ 1., 1., 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 1., 0., 0., 1., 1., X_COL, X_COL, X_COL },
	{ 1., .5, 0., 1., .5, X_COL, X_COL, X_COL },
	{ 1., 1., 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 1., 1., .5, .5, 0., X_COL, X_COL, X_COL },
};

struct vertex_desc vertices_stairs_ul[] = {
	{ 1., .5, 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., .5, 1., 0., 1., B_COL, B_COL, B_COL, 2 },
	{ .5, .5, 1., 0., .5, B_COL, B_COL, B_COL, 2 },
	{ 1., .5, 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ .5, .5, 1., 0., .5, B_COL, B_COL, B_COL, 2 },
	{ .5, .5, 0., 1., .5, B_COL, B_COL, B_COL, 2 },
	{ .5, 0., 0., 1., .5, B_COL, B_COL, B_COL, 2 },
	{ .5, 0., 1., 0., .5, B_COL, B_COL, B_COL, 2 },
	{ 0., 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ .5, 0., 0., 1., .5, B_COL, B_COL, B_COL, 2 },
	{ 0., 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., 0., 1., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., 1., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 1., 1., 1., 0., .5, X_COL, X_COL, X_COL },
	{ 1., .5, 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., 1., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 1., .5, 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., .5, 0., 1., 0., X_COL, X_COL, X_COL },
	{ .5, .5, 0., 1., 1., X_COL, X_COL, X_COL },
	{ .5, .5, 1., 0., 1., X_COL, X_COL, X_COL },
	{ .5, 0., 1., 0., .5, X_COL, X_COL, X_COL },
	{ .5, .5, 0., 1., 1., X_COL, X_COL, X_COL },
	{ .5, 0., 1., 0., .5, X_COL, X_COL, X_COL },
	{ .5, 0., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 0., 1., 0., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, .5, 0., .5, .5, Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., .5, 0., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 0., 0., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, .5, 0., .5, .5, Z_COL, Z_COL, Z_COL },
	{ .5, 0., 0., .5, 0., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 0., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ .5, .5, 1., .5, .5, Z_COL, Z_COL, Z_COL },
	{ 1., .5, 1., 1., .5, Z_COL, Z_COL, Z_COL },
	{ .5, .5, 1., .5, .5, Z_COL, Z_COL, Z_COL },
	{ 0., 0., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, 0., 1., .5, 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_stairs_ur[] = {
	{ 0., .5, 1., 0., 1., B_COL, B_COL, B_COL, 2 },
	{ 0., .5, 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ .5, .5, 1., 0., .5, B_COL, B_COL, B_COL, 2 },
	{ .5, .5, 1., 0., .5, B_COL, B_COL, B_COL, 2 },
	{ 0., .5, 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ .5, .5, 0., 1., .5, B_COL, B_COL, B_COL, 2 },
	{ .5, 0., 1., 0., .5, B_COL, B_COL, B_COL, 2 },
	{ .5, 0., 0., 1., .5, B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ .5, 0., 0., 1., .5, B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 0., 1., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., 1., 1., 0., .5, X_COL, X_COL, X_COL },
	{ 0., 1., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 0., .5, 1., 0., 0., X_COL, X_COL, X_COL },
	{ 0., .5, 1., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 1., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 0., .5, 0., 1., 0., X_COL, X_COL, X_COL },
	{ .5, .5, 1., 0., 1., X_COL, X_COL, X_COL },
	{ .5, .5, 0., 1., 1., X_COL, X_COL, X_COL },
	{ .5, 0., 1., 0., .5, X_COL, X_COL, X_COL },
	{ .5, 0., 1., 0., .5, X_COL, X_COL, X_COL },
	{ .5, .5, 0., 1., 1., X_COL, X_COL, X_COL },
	{ .5, 0., 0., 1., .5, X_COL, X_COL, X_COL },
	{ 0., 1., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 0., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ .5, .5, 0., .5, .5, Z_COL, Z_COL, Z_COL },
	{ 0., .5, 0., 1., .5, Z_COL, Z_COL, Z_COL },
	{ .5, .5, 0., .5, .5, Z_COL, Z_COL, Z_COL },
	{ 1., 0., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, 0., 0., .5, 0., Z_COL, Z_COL, Z_COL },
	{ 1., 1., 1., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 0., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, .5, 1., .5, .5, Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., .5, 1., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 1., 0., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .5, .5, 1., .5, .5, Z_COL, Z_COL, Z_COL },
	{ .5, 0., 1., .5, 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_stairs_ub[] = {
	{ 1., .5, 1., 0., 1., B_COL, B_COL, B_COL, 2 },
	{ 0., .5, 1., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., .5, .5, 0., .5, B_COL, B_COL, B_COL, 2 },
	{ 1., .5, .5, 0., .5, B_COL, B_COL, B_COL, 2 },
	{ 0., .5, 1., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 0., .5, .5, 1., .5, B_COL, B_COL, B_COL, 2 },
	{ 1., 0., .5, 0., .5, B_COL, B_COL, B_COL, 2 },
	{ 0., 0., .5, 1., .5, B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 0., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 0., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., .5, 1., .5, B_COL, B_COL, B_COL, 2 },
	{ 0., 0., 0., 1., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., 1., 1., 0., .5, Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 1., .5, 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., .5, 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 0., .5, 1., 1., 0., Z_COL, Z_COL, Z_COL },
	{ 1., .5, .5, 0., 1., Z_COL, Z_COL, Z_COL },
	{ 0., .5, .5, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 0., .5, 0., .5, Z_COL, Z_COL, Z_COL },
	{ 1., 0., .5, 0., .5, Z_COL, Z_COL, Z_COL },
	{ 0., .5, .5, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 0., .5, 1., .5, Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., 0., 0., 1., X_COL, X_COL, X_COL },
	{ 0., 0., 0., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 1., 1., 1., 1., X_COL, X_COL, X_COL },
	{ 0., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 0., .5, 1., 1., .5, X_COL, X_COL, X_COL },
	{ 0., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 0., 0., 0., 0., 0., X_COL, X_COL, X_COL },
	{ 0., 0., .5, .5, 0., X_COL, X_COL, X_COL },
	{ 1., 1., 0., 0., 1., X_COL, X_COL, X_COL },
	{ 1., 1., 1., 1., 1., X_COL, X_COL, X_COL },
	{ 1., 0., 0., 0., 0., X_COL, X_COL, X_COL },
	{ 1., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 1., 1., 1., 1., 1., X_COL, X_COL, X_COL },
	{ 1., .5, 1., 1., .5, X_COL, X_COL, X_COL },
	{ 1., 0., 0., 0., 0., X_COL, X_COL, X_COL },
	{ 1., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 1., 0., .5, .5, 0., X_COL, X_COL, X_COL },
};

struct vertex_desc vertices_stairs_uf[] = {
	{ 0., .5, 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., .5, 0., 0., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., .5, .5, 0., .5, B_COL, B_COL, B_COL, 2 },
	{ 0., .5, 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., .5, .5, 0., .5, B_COL, B_COL, B_COL, 2 },
	{ 0., .5, .5, 1., .5, B_COL, B_COL, B_COL, 2 },
	{ 0., 0., .5, 1., .5, B_COL, B_COL, B_COL, 2 },
	{ 1., 0., .5, 0., .5, B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., .5, 1., .5, B_COL, B_COL, B_COL, 2 },
	{ 1., 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., 1., 1., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., 1., 0., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 1., 1., 0., 0., .5, Z_COL, Z_COL, Z_COL },
	{ 1., .5, 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 1., 0., 1., .5, Z_COL, Z_COL, Z_COL },
	{ 1., .5, 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., .5, 0., 1., 0., Z_COL, Z_COL, Z_COL },
	{ 0., .5, .5, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., .5, .5, 0., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 0., .5, 0., .5, Z_COL, Z_COL, Z_COL },
	{ 0., .5, .5, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 0., .5, 0., .5, Z_COL, Z_COL, Z_COL },
	{ 0., 0., .5, 1., .5, Z_COL, Z_COL, Z_COL },
	{ 0., 1., 1., 0., 1., X_COL, X_COL, X_COL },
	{ 0., 1., 0., 1., 1., X_COL, X_COL, X_COL },
	{ 0., 0., 1., 0., 0., X_COL, X_COL, X_COL },
	{ 0., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 0., 1., 0., 1., 1., X_COL, X_COL, X_COL },
	{ 0., .5, 0., 1., .5, X_COL, X_COL, X_COL },
	{ 0., 0., 1., 0., 0., X_COL, X_COL, X_COL },
	{ 0., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 0., 0., .5, .5, 0., X_COL, X_COL, X_COL },
	{ 1., 1., 0., 1., 1., X_COL, X_COL, X_COL },
	{ 1., 1., 1., 0., 1., X_COL, X_COL, X_COL },
	{ 1., 0., 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., 1., 0., 1., 1., X_COL, X_COL, X_COL },
	{ 1., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 1., .5, 0., 1., .5, X_COL, X_COL, X_COL },
	{ 1., .5, .5, .5, .5, X_COL, X_COL, X_COL },
	{ 1., 0., 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., 0., .5, .5, 0., X_COL, X_COL, X_COL },
};

struct vertex_desc vertices_pane_x[] = {
	{ .46875, 0., 0., 0., 1., B_COL, B_COL, B_COL, 2 },
	{ .53125, 0., 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ .46875, 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ .46875, 0., 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ .53125, 0., 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ .53125, 0., 1., 1., 0., B_COL, B_COL, B_COL, 2 },

	{ .53125, 1., 1., 0., 1., T_COL, T_COL, T_COL, 1 },
	{ .53125, 1., 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ .46875, 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ .46875, 1., 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ .53125, 1., 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ .46875, 1., 0., 1., 0., T_COL, T_COL, T_COL, 1 },

	{ .46875, 0., 0., 0., 1., X_COL, X_COL, X_COL },
	{ .46875, 0., 1., 1., 1., X_COL, X_COL, X_COL },
	{ .46875, 1., 0., 0., 0., X_COL, X_COL, X_COL },
	{ .46875, 1., 0., 0., 0., X_COL, X_COL, X_COL },
	{ .46875, 0., 1., 1., 1., X_COL, X_COL, X_COL },
	{ .46875, 1., 1., 1., 0., X_COL, X_COL, X_COL },

	{ .53125, 0., 1., 0., 1., X_COL, X_COL, X_COL },
	{ .53125, 0., 0., 1., 1., X_COL, X_COL, X_COL },
	{ .53125, 1., 1., 0., 0., X_COL, X_COL, X_COL },
	{ .53125, 1., 1., 0., 0., X_COL, X_COL, X_COL },
	{ .53125, 0., 0., 1., 1., X_COL, X_COL, X_COL },
	{ .53125, 1., 0., 1., 0., X_COL, X_COL, X_COL },

	{ .53125, 0., 0., 0., 1., Z_COL, Z_COL, Z_COL },
	{ .46875, 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ .53125, 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .53125, 1., 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .46875, 0., 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ .46875, 1., 0., 1., 0., Z_COL, Z_COL, Z_COL },

	{ .46875, 0., 1., 0., 1., Z_COL, Z_COL, Z_COL },
	{ .53125, 0., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ .46875, 1., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .46875, 1., 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ .53125, 0., 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ .53125, 1., 1., 1., 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_pane_y[] = {
	{ 0., .46875, 0., 0., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., .46875, 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 0., .46875, 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., .46875, 1., 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., .46875, 0., 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., .46875, 1., 1., 0., B_COL, B_COL, B_COL, 2 },

	{ 1., .53125, 1., 0., 1., T_COL, T_COL, T_COL, 1 },
	{ 1., .53125, 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., .53125, 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., .53125, 1., 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 1., .53125, 0., 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., .53125, 0., 1., 0., T_COL, T_COL, T_COL, 1 },

	{ 0., .46875, 0., 0., 1., X_COL, X_COL, X_COL },
	{ 0., .46875, 1., 1., 1., X_COL, X_COL, X_COL },
	{ 0., .53125, 0., 0., 0., X_COL, X_COL, X_COL },
	{ 0., .53125, 0., 0., 0., X_COL, X_COL, X_COL },
	{ 0., .46875, 1., 1., 1., X_COL, X_COL, X_COL },
	{ 0., .53125, 1., 1., 0., X_COL, X_COL, X_COL },

	{ 1., .46875, 1., 0., 1., X_COL, X_COL, X_COL },
	{ 1., .46875, 0., 1., 1., X_COL, X_COL, X_COL },
	{ 1., .53125, 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., .53125, 1., 0., 0., X_COL, X_COL, X_COL },
	{ 1., .46875, 0., 1., 1., X_COL, X_COL, X_COL },
	{ 1., .53125, 0., 1., 0., X_COL, X_COL, X_COL },

	{ 1., .46875, 0., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 0., .46875, 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., .53125, 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., .53125, 0., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., .46875, 0., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., .53125, 0., 1., 0., Z_COL, Z_COL, Z_COL },

	{ 0., .46875, 1., 0., 1., Z_COL, Z_COL, Z_COL },
	{ 1., .46875, 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., .53125, 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., .53125, 1., 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., .46875, 1., 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., .53125, 1., 1., 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_desc vertices_pane_z[] = {
	{ 0., 0., .46875, 0., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., .46875, 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., .53125, 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 0., 0., .53125, 0., 0., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., .46875, 1., 1., B_COL, B_COL, B_COL, 2 },
	{ 1., 0., .53125, 1., 0., B_COL, B_COL, B_COL, 2 },

	{ 1., 1., .53125, 0., 1., T_COL, T_COL, T_COL, 1 },
	{ 1., 1., .46875, 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., .53125, 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., .53125, 0., 0., T_COL, T_COL, T_COL, 1 },
	{ 1., 1., .46875, 1., 1., T_COL, T_COL, T_COL, 1 },
	{ 0., 1., .46875, 1., 0., T_COL, T_COL, T_COL, 1 },

	{ 0., 0., .46875, 0., 1., X_COL, X_COL, X_COL },
	{ 0., 0., .53125, 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., .46875, 0., 0., X_COL, X_COL, X_COL },
	{ 0., 1., .46875, 0., 0., X_COL, X_COL, X_COL },
	{ 0., 0., .53125, 1., 1., X_COL, X_COL, X_COL },
	{ 0., 1., .53125, 1., 0., X_COL, X_COL, X_COL },

	{ 1., 0., .53125, 0., 1., X_COL, X_COL, X_COL },
	{ 1., 0., .46875, 1., 1., X_COL, X_COL, X_COL },
	{ 1., 1., .53125, 0., 0., X_COL, X_COL, X_COL },
	{ 1., 1., .53125, 0., 0., X_COL, X_COL, X_COL },
	{ 1., 0., .46875, 1., 1., X_COL, X_COL, X_COL },
	{ 1., 1., .46875, 1., 0., X_COL, X_COL, X_COL },

	{ 1., 0., .46875, 0., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 0., .46875, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., .46875, 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., 1., .46875, 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 0., .46875, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., .46875, 1., 0., Z_COL, Z_COL, Z_COL },

	{ 0., 0., .53125, 0., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 0., .53125, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 0., 1., .53125, 0., 0., Z_COL, Z_COL, Z_COL },
	{ 0., 1., .53125, 0., 0., Z_COL, Z_COL, Z_COL },
	{ 1., 0., .53125, 1., 1., Z_COL, Z_COL, Z_COL },
	{ 1., 1., .53125, 1., 0., Z_COL, Z_COL, Z_COL },
};

struct vertex_buffer *vertex_buffer(int vbo_count)
{
	struct vertex_buffer *r = calloc(1, sizeof(*r));
	r->vbo_count = vbo_count;
	r->vbo_names = calloc(vbo_count, sizeof(*r->vbo_names));
	r->vbo_sizes = calloc(vbo_count, sizeof(*r->vbo_sizes));
	glGenBuffers(r->vbo_count, r->vbo_names);
	assert(glGetError() == GL_NO_ERROR);
	return r;
}

void vertex_buffer_destroy(struct vertex_buffer *r)
{
	assert(glGetError() == GL_NO_ERROR);
	glDeleteBuffers(r->vbo_count, r->vbo_names);
	free(r->vbo_names);
	free(r->vbo_sizes);
	free(r);
}

void vertex_buffer_enable(struct vertex_buffer *r)
{
	assert(glGetError() == GL_NO_ERROR);
}

void vertex_buffer_draw_slice(struct vertex_buffer *r, GLenum mode, size_t buf,
		size_t first, size_t count)
{
	assert(buf < r->vbo_count);
	assert(first + count <= r->vbo_sizes[buf]);
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo_names[buf]);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), (char *)0);
	glTexCoordPointer(4, GL_FLOAT, sizeof(struct vertex), (char *)offsetof(struct vertex, u0));
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(struct vertex), (char *)offsetof(struct vertex, r));
	glDrawArrays(mode, first, count);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	assert(glGetError() == GL_NO_ERROR);
}

void vertex_buffer_draw(struct vertex_buffer *r, GLenum mode, size_t buf)
{
	vertex_buffer_draw_slice(r, mode, buf, 0, r->vbo_sizes[buf]);
}

void vertex_buffer_disable(struct vertex_buffer *r)
{
	assert(glGetError() == GL_NO_ERROR);
}

void vertex_buffer_update(struct vertex_buffer *r, size_t buf, const void *data, size_t size)
{
	assert(glGetError() == GL_NO_ERROR);
	assert(buf < r->vbo_count);
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo_names[buf]);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(struct vertex), data, GL_STATIC_DRAW);
	r->vbo_sizes[buf] = size;
	assert(glGetError() == GL_NO_ERROR);
}


