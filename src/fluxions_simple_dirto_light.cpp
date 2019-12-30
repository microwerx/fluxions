#include "pch.hpp"
#include <fluxions_simple_dirto_light.hpp>
#include <fluxions_fileio_iostream.hpp>

namespace Fluxions
{
	bool SimpleDirToLight::read(const std::string& keyword, std::istream& istr) {
		if (keyword == this->keyword()) {
			float _E0 = ReadFloat(istr);
			E0.reset(_E0, _E0, _E0);
			dirTo = ReadVector3f(istr);
			return true;
		}
		return false;
	}
	
	bool SimpleDirToLight::write(std::ostream& ostr) const {
		SimpleSceneGraphNode::write(ostr);
		WriteFloat(ostr, E0.r);
		WriteVector3f(ostr, dirTo.xyz()) << "\n";
		return true;
	}
}