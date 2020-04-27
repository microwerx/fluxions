#include "fluxions_ssg_pch.hpp"
#include <fluxions_ssg_point_light.hpp>
#include <fluxions_fileio_iostream.hpp>

namespace Fluxions {
	bool SimplePointLight::read(const std::string& keyword, std::istream& istr) {
		if (keyword == this->keyword()) {
			E0.r = ReadFloat(istr);
			E0.g = ReadFloat(istr);
			E0.b = ReadFloat(istr);
			E0.a = luma709(E0);
			position = ReadVector4f(istr);

			Vector3f O = this->worldMatrix().origin();
			bbox.reset();
			bbox += O;
			bbox += O + 0.5f;
			bbox += O - 0.5f;

			return true;
		}
		return false;
	}

	bool SimplePointLight::write(std::ostream& ostr) const {
		SimpleSceneGraphNode::write(ostr);
		WriteColor3f(ostr, E0);
		WriteVector4f(ostr, position);
		return true;
	}
} // namespace Fluxions
