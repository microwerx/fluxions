#ifndef FLUXIONS_SIMPLE_POINT_LIGHT_HPP
#define FLUXIONS_SIMPLE_POINT_LIGHT_HPP

#include <fluxions_gte_vector3.hpp>
#include <fluxions_simple_cube_texture.hpp>

namespace Fluxions
{
	struct SimplePointLight
	{
		std::string name;
		size_t index;
		float E0;
		float falloffRadius;
		Vector3f position;

		SimpleCubeTexture scs;
	};
} // namespace Fluxions

#endif