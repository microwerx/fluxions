#ifndef FLUXIONS_SIMPLE_SPHERE_HPP
#define FLUXIONS_SIMPLE_SPHERE_HPP

#include <fluxions_gte_matrix4.hpp>
#include <fluxions_opengl.hpp>
#include <fluxions_simple_scene_graph_node.hpp>
#include <fluxions_colors.hpp>

namespace Fluxions
{
	class SimpleSphere : public SimpleSceneGraphNode
	{
	public:
		std::string mtllibName;
		std::string mtlName;
		GLuint mtllibId = 0;
		GLuint mtlId = 0;
		GLuint objectId = 0;

		const char* type() const override { return "SimpleSphere"; }
		const char* keyword() const override { return "sphere"; }
		Color3f color() const override { return FxColors3::Rose; }

		bool read(const std::string& keyword, std::istream& istr) override;
		bool write(std::ostream& ostr) const override;
	};
} // namespace Fluxions

#endif