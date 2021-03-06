/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#include "tone_mapper.h"

#include <stdlib.h>

#include "box2.h"

ToneMapper::ToneMapper(float dt, size_t samples)
	: samples(samples), dt(dt)
{
	luminance = 1;
	max_intensity = 1;
	gamma = .66;
	a = (.5 / samples);
	b = (1 - 2 * a) / max_intensity;
	sky_color = v3f(1, 1, 1);
	spot_color_0 = v3f(.94, .63, 0);
	spot_color_1 = v3f(.31, .16, 0);

	float n = 4;
	float pow_n_dt = pow(n, dt);
	float pow_n_minus_one_dt = pow(n - 1, dt);
	k = (pow_n_dt - pow_n_minus_one_dt) / pow_n_dt;

	mapped_sky_color = new v3f[samples];
	mapped_spot_color = new v3f[samples];
	mapped_color = new v3f[samples * samples];

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, samples, samples, 0, GL_RGB,
			GL_FLOAT, mapped_color);
}

ToneMapper::~ToneMapper()
{
	glDeleteTextures(1, &texture);
	delete [] mapped_sky_color;
	delete [] mapped_spot_color;
	delete [] mapped_color;
}

void ToneMapper::update(float sky, float spot)
{
	// Calculate the mean of light in non-opaque cells around the player item.

	time += dt;
	v3f c1 = sky_color;
	v3f c2 = spot_color_0 + spot_color_1 * (1 + sinf(time * 5));

	sky_brightness = (c1.x + c1.y + c1.z) / 3;
	spot_brightness = (c2.x + c2.y + c2.z) / 3;

	float k = 0;

	luminance += (sky * sky_brightness + spot * spot_brightness - luminance) * k;

	// regenerate textures

	float A = .25 / pow(luminance, gamma) + .75;
	float scale = max_intensity / samples;

	c1 = c1 * scale;
	c2 = c2 * scale;

	for (int i = 0; i < samples; ++i) {
		mapped_sky_color[i] = c1 * static_cast<float>(i);
		mapped_spot_color[i] = c2 * static_cast<float>(i);
	}

	int i = 0;
	for (auto p : box2s(0, 0, samples - 1, samples - 1))
		mapped_color[i++] = A * pow(mapped_sky_color[p.y]
				+ mapped_spot_color[p.x], gamma);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, samples, samples, GL_RGB,
			GL_FLOAT, mapped_color);
}

