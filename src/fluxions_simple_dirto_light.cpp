#include "pch.hpp"
#include <fluxions_simple_dirto_light.hpp>
#include <fluxions_fileio_iostream.hpp>

namespace Fluxions
{
	bool SimpleDirToLight::read(const std::string& keyword, std::istream& istr) {
		if (keyword == this->keyword()) {
			E0 = ReadFloat(istr);
			dirTo = ReadVector3f(istr);
			return true;
		}
		return false;
	}
	
	bool SimpleDirToLight::write(std::ostream& ostr) const {
		SimpleSceneGraphNode::write(ostr);
		WriteFloat(ostr, E0);
		WriteVector3f(ostr, dirTo) << "\n";
		return true;
	}
}