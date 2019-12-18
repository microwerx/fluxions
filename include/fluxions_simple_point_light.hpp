#ifndef FLUXIONS_SIMPLE_POINT_LIGHT_HPP
#define FLUXIONS_SIMPLE_POINT_LIGHT_HPP

#include <fluxions_gte_vector3.hpp>
#include <fluxions_renderer_texture_cube.hpp>

namespace Fluxions
{
	struct SimplePointLight
	{
		std::string name;
		size_t index;
		float E0;
		float falloffRadius;
		Vector3f position;

		// FIXME: Why do we have a renderer element here?
		RendererTextureCube scs;
	};
} // namespace Fluxions

#endif