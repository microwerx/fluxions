#include "pch.hpp"
#include <fluxions_fileio_iostream.hpp>
#include <fluxions_simple_scene_graph_node.hpp>

namespace Fluxions
{
	bool SimpleSceneGraphNode::read(const std::string& keyword, std::istream& istr) {
		if (keyword == this->keyword()) {
			setName(ReadString(istr));
			return true;
		}
		return false;
	}

	bool SimpleSceneGraphNode::write(std::ostream& ostr) const {
		ostr << keyword() << " \"" << name() << "\"\n";
		return true;
	}
}