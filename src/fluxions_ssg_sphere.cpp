#include "fluxions_ssg_pch.hpp"
#include <fluxions_ssg_sphere.hpp>
#include <fluxions_fileio_iostream.hpp>

namespace Fluxions {
	bool SimpleSphere::read(const std::string& keyword, std::istream& istr) {
		if (keyword == this->keyword()) {
			materialName = ReadString(istr);
			return true;
		}
		return false;
	}

	bool SimpleSphere::write(std::ostream& ostr) const {
		SimpleSceneGraphNode::write(ostr);
		WriteString(ostr, materialName) << "\n";
		return true;
	}
} // namespace Fluxions
