#include "pch.hpp"
#include <fluxions_simple_point_light.hpp>
#include <fluxions_fileio_iostream.hpp>

namespace Fluxions
{
	bool SimplePointLight::read(const std::string& keyword, std::istream& istr) {
		if (keyword == this->keyword()) {
			E0 = ReadFloat(istr);
			falloffRadius = ReadFloat(istr);
			position = ReadVector3f(istr);
			return true;
		}
		return false;
	}

	bool SimplePointLight::write(std::ostream& ostr) const {
		SimpleSceneGraphNode::write(ostr);
		WriteFloat(ostr, E0);
		WriteFloat(ostr, falloffRadius);
		WriteVector3f(ostr, position);
		return true;
	}
}