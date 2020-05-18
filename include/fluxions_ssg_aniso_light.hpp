#ifndef FLUXIONS_SSG_ANISO_LIGHT_HPP
#define FLUXIONS_SSG_ANISO_LIGHT_HPP

#include <fluxions_ssg_base.hpp>
#include <fluxions_ssg_node.hpp>

namespace Fluxions {
	struct SimpleAnisoLight : public SimpleSceneGraphNode {
		BaseAnisoLight ublock;
		Matrix4f viewMatrix;
		Matrix4f projMatrix;
		Matrix4f projviewMatrix;

		SimpleAnisoLight();

		const char* type() const override { return "SimpleAnisoLight"; }
		const char* keyword() const override { return "anisoLight"; }
		Color3f color() const override { return { ublock.SH[0].r, ublock.SH[0].g, ublock.SH[0].b }; }

		bool read(const std::string& keyword, std::istream& istr) override;
		bool write(std::ostream& ostr) const override;
	};
} // namespace Fluxions

#endif
