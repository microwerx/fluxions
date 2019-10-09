#ifndef FLUXIONS_SIMPLE_CUBE_TEXTURE_HPP
#define FLUXIONS_SIMPLE_CUBE_TEXTURE_HPP

#include <fluxions_simple_gpu_texture.hpp>

namespace Fluxions
{
	struct SimpleCubeTexture
	{
		GLint unit = 0;
		GLint samplerId = 0;
		double buildTime = 0.0;
		float zfar = 0.0f;
		SimpleGpuTexture texture = SimpleGpuTexture(GL_TEXTURE_CUBE_MAP);
		bool dirty = true;
	};
} // namespace Fluxions

#endif