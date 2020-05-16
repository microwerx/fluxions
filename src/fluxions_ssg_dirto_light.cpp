#include "fluxions_ssg_pch.hpp"
#include <fluxions_ssg_dirto_light.hpp>
#include <fluxions_file_system.hpp>

namespace Fluxions {
	bool SimpleDirToLight::read(const std::string& keyword, std::istream& istr) {
		if (keyword == this->keyword()) {
			float _E0 = ReadFloat(istr);
			ublock.E0.reset(_E0, _E0, _E0);
			ublock.ublock = ReadVector3f(istr);
			return true;
		}
		return false;
	}

	bool SimpleDirToLight::write(std::ostream& ostr) const {
		SimpleSceneGraphNode::write(ostr);
		WriteFloat(ostr, ublock.E0.r);
		WriteVector3f(ostr, ublock.ublock.xyz()) << "\n";
		return true;
	}
} // namespace Fluxions
