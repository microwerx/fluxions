#ifndef FLUXIONS_SSG_DIRTO_LIGHT_HPP
#define FLUXIONS_SSG_DIRTO_LIGHT_HPP

#include <fluxions_ssg_base.hpp>
#include <fluxions_ssg_node.hpp>

namespace Fluxions {
	struct SimpleDirToLight : public BaseDirToLight, public SimpleSceneGraphNode {
		const char* type() const override { return "SimpleDirToLight"; }
		const char* keyword() const override { return "dirtoLight"; }
		Color3f color() const override { return FxColors3::Yellow; }

		bool read(const std::string& keyword, std::istream& istr) override;
		bool write(std::ostream& ostr) const override;
	};
} // namespace Fluxions

#endif
