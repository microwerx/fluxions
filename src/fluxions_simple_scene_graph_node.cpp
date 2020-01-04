#include "pch.hpp"
#include <fluxions_fileio_iostream.hpp>
#include <fluxions_simple_scene_graph_node.hpp>

namespace Fluxions
{
	const char* SimpleSceneGraphNode::updateNodeName() {
		std::ostringstream ostr;
		ostr << name() << "[" << type() << "]: [" << status() << "]";
		nodeName_ = ostr.str();
		return nodeName_.c_str();
	}

	bool SimpleSceneGraphNode::read(const std::string& keyword, std::istream& istr) {
		if (keyword == this->keyword()) {
			setName(ReadString(istr));
			return true;
		}
		return false;
	}

	bool SimpleSceneGraphNode::write(std::ostream& ostr) const {
		ostr << "\ntransform ";
		WriteAffineMatrix4f(ostr, transform) << "\n";
		WriteLabel(ostr, keyword());
		WriteString(ostr, name()) << "\n";
		return true;
	}
}