#include "fluxions_ssg_pch.hpp"
#include <fluxions_file_system.hpp>
#include <fluxions_ssg_aniso_light.hpp>

namespace Fluxions {
	SimpleAnisoLight::SimpleAnisoLight()
		: SimpleSceneGraphNode() {}

	bool SimpleAnisoLight::read(const std::string& keyword_, std::istream& istr) {
		if (keyword_ == this->keyword()) {
			position = ReadVector4f(istr);
			SH[0] = ReadColor3f(istr);
			SH[0].a = 1.0f;
			transform = Matrix4f::MakeTranslation(position.xyz());
			return true;
		}
		return false;
	}

	bool SimpleAnisoLight::write(std::ostream& ostr) const {
		SimpleSceneGraphNode::write(ostr);
		WriteVector4f(ostr, position);
		WriteColor3f(ostr, SH[0].rgb());
		return true;
	}
}
