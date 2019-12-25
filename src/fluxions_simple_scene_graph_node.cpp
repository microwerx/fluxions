#include "pch.hpp"
#include <fluxions_simple_scene_graph_node.hpp>

namespace Fluxions
{
	bool SimpleSceneGraphNode::read(const std::string& keyword, std::istream& istr) {
		return false;
	}

	bool SimpleSceneGraphNode::write(std::ostream& ostr) {
		return false;
	}
}