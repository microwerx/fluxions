#ifndef FLUXIONS_SSG_SPHERE_HPP
#define FLUXIONS_SSG_SPHERE_HPP

#include <fluxions_gte_matrix4.hpp>
#include <fluxions_opengl.hpp>
#include <fluxions_ssg_node.hpp>
#include <fluxions_colors.hpp>

namespace Fluxions {
	class SimpleSphere : public SimpleSceneGraphNode {
	public:
		std::string materialLibrary;
		std::string materialName;

		const char* type() const override { return "SimpleSphere"; }
		const char* keyword() const override { return "sphere"; }
		Color3f color() const override { return FxColors3::Rose; }

		bool read(const std::string& keyword, std::istream& istr) override;
		bool write(std::ostream& ostr) const override;
	};
} // namespace Fluxions

#endif
