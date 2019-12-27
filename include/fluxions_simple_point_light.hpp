#ifndef FLUXIONS_SIMPLE_POINT_LIGHT_HPP
#define FLUXIONS_SIMPLE_POINT_LIGHT_HPP

#include <fluxions_gte_vector3.hpp>
#include <fluxions_simple_scene_graph_node.hpp>
#include <fluxions_renderer_texture_cube.hpp>

namespace Fluxions
{
	struct SimplePointLight : public SimpleSceneGraphNode
	{
		unsigned index;
		float E0;
		float falloffRadius;
		Vector3f position;

		virtual const char* type() const override { return "SimplePointLight"; }

		// FIXME: Why do we have a renderer element here?
		RendererTextureCube scs;
	};
} // namespace Fluxions

#endif