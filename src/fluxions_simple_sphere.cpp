#include "pch.hpp"
#include <fluxions_simple_sphere.hpp>
#include <fluxions_fileio_iostream.hpp>

namespace Fluxions
{
	bool SimpleSphere::read(const std::string& keyword, std::istream& istr) {
		if (keyword == this->keyword()) {
			mtlName = ReadString(istr);
			return true;
		}
		return false;
	}

	bool SimpleSphere::write(std::ostream& ostr) const {
		SimpleSceneGraphNode::write(ostr);
		WriteString(ostr, mtlName) << "\n";
		return true;
	}
}