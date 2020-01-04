#ifndef FLUXIONS_SIMPLE_GEOMETRY_GROUP_HPP
#define FLUXIONS_SIMPLE_GEOMETRY_GROUP_HPP

#include <string>
#include <fluxions_gte_bounding_box.hpp>
#include <fluxions_file_path_info.hpp>
#include <fluxions_simple_scene_graph_node.hpp>

namespace Fluxions
{
	struct SimpleGeometryGroup : SimpleSceneGraphNode
	{
		FilePathInfo fpi;

		std::string objectName;
		//std::string mtllibName;
		unsigned objectId = 0;
		//unsigned mtllibId = 0;

		virtual const char* nodetype() const noexcept { return "SimpleGeometryGroup"; }
	};
} // namespace Fluxions

#endif