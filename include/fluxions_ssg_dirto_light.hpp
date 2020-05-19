#ifndef FLUXIONS_SSG_DIRTO_LIGHT_HPP
#define FLUXIONS_SSG_DIRTO_LIGHT_HPP

#include <fluxions_ssg_base.hpp>
#include <fluxions_ssg_node.hpp>

namespace Fluxions {
	struct SimpleDirToLight : public SimpleSceneGraphNode {
		BaseDirToLight ublock;
		Matrix4f viewMatrix;
		Matrix4f projMatrix;
		Matrix4f projviewMatrix;

		const char* type() const override { return "SimpleDirToLight"; }
		const char* keyword() const override { return "dirtoLight"; }
		Color3f color() const override { return { ublock.E0.r, ublock.E0.g, ublock.E0.b }; }

		bool read(const std::string& keyword, std::istream& istr) override;
		bool write(std::ostream& ostr) const override;
	};
} // namespace Fluxions

#endif
