/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#include "terraform.h"

#include <limits.h>

#include <libnoise/noise.h>
#include "noiseutils.h"

#include "box2.h"

void terraform(int64_t seed, Chunk *c)
{
	v2ll p0 = c->get_p();
	box2f bb(0, 0, Chunk::W, Chunk::D);
	bb += v2f(p0);
	bb /= 500.f;

	noise::module::RidgedMulti hill_base;
	noise::module::ScaleBias hill;
	hill.SetSourceModule(0, hill_base);
	hill.SetScale(0.5);
	hill.SetBias(0.5);

	noise::module::Billow flat_base;
	flat_base.SetFrequency(2.0);
	noise::module::ScaleBias flat;
	flat.SetSourceModule(0, flat_base);
	flat.SetScale(0.125);
	flat.SetBias(0.125);

	noise::module::Perlin land_type;
	land_type.SetFrequency(0.5);
	land_type.SetPersistence(0.25);
	land_type.SetSeed(0);

	noise::module::Select land;
	land.SetSourceModule(0, flat);
	land.SetSourceModule(1, hill);
	land.SetControlModule(land_type);
	land.SetBounds(0.0, 1000.0);
	land.SetEdgeFalloff(0.125);

	noise::module::RidgedMulti deep_base;
	noise::module::ScaleBias deep;
	deep.SetSourceModule(0, deep_base);
	deep.SetScale(0.5);
	deep.SetBias(-0.5);

	noise::module::Billow beach_base;
	beach_base.SetFrequency(2.0);
	noise::module::ScaleBias beach;
	beach.SetSourceModule(0, beach_base);
	beach.SetScale(0.125);
	beach.SetBias(-0.125);

	noise::module::Perlin ocean_type;
	ocean_type.SetFrequency(0.5);
	ocean_type.SetPersistence(0.25);
	ocean_type.SetSeed(1000);

	noise::module::Select ocean;
	ocean.SetSourceModule(0, beach);
	ocean.SetSourceModule(1, deep);
	ocean.SetControlModule(ocean_type);
	ocean.SetBounds(0.0, 1000.0);
	ocean.SetEdgeFalloff(0.125);

	noise::module::Perlin finalType;
	finalType.SetFrequency(0.5);
	finalType.SetPersistence(0.25);
	finalType.SetSeed(2000);

	noise::module::Select finalTerrain;
	finalTerrain.SetSourceModule(0, land);
	finalTerrain.SetSourceModule(1, ocean);
	finalTerrain.SetControlModule(finalType);
	finalTerrain.SetBounds(0.0, 1000.0);
	finalTerrain.SetEdgeFalloff(0.125);

	noise::utils::NoiseMap heightMap;
	noise::utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(finalTerrain);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(16, 16);
	heightMapBuilder.SetBounds(bb.x0, bb.x1, bb.y0, bb.y1);
	heightMapBuilder.Build();

	for (auto p : box2ll(0, 0, 15, 15)) {
		v3ll q(p.x, 0, p.y);
		float f = heightMap.GetValue(p.x, p.y);
		int h = 128 + f * 64;
		int mat;
		if (f <= 0)
			mat = MAT_SANDSTONE_SAND;
		else if (f < .5)
			mat = MAT_GRASS;
		else if (f < .6)
			mat = MAT_LIMESTONE_SAND;
		else if (f < .75)
			mat = MAT_LIMESTONE;
		else
			mat = MAT_MARBLE;
		while(q.y < h) {
			c->set_mat(q, mat);
			c->set_shape(q, SHAPE_BLOCK_DN);
			++q.y;
		}
		while (q.y < Chunk::H) {
			c->set_mat(q, 0);
			c->set_shape(q, SHAPE_NONE);
			++q.y;
		}
	}
}

