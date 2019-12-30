#ifndef FLUXIONS_SIMPLE_POINT_LIGHT_HPP
#define FLUXIONS_SIMPLE_POINT_LIGHT_HPP

#include <fluxions_simple_scene_graph_base.hpp>
#include <fluxions_simple_scene_graph_node.hpp>
#include <fluxions_renderer_texture_cube.hpp>

namespace Fluxions
{

	struct SimplePointLight : public BasePointLight, public SimpleSceneGraphNode
	{
		unsigned index;
		float E0;
		float falloffRadius;
		Vector3f position;

		const char* type() const override { return "SimplePointLight"; }
		const char* keyword() const override{ return "pointLight"; }
		Color3f color() const override { return FxColors3::Yellow; }

		bool read(const std::string& keyword, std::istream& istr) override;
		bool write(std::ostream& ostr) const override;

		// FIXME: Why do we have a renderer element here?
		RendererTextureCube scs;
	};
} // namespace Fluxions

#endif