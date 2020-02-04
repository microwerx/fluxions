#ifndef FLUXIONS_SSG_GEOMETRY_GROUP_HPP
#define FLUXIONS_SSG_GEOMETRY_GROUP_HPP

#include <fluxions_ssg_node.hpp>

namespace Fluxions
{
struct SimpleGeometryGroup : SimpleSceneGraphNode
{
	FilePathInfo fpi;

	std::string objectName;
	//std::string mtllibName;
	unsigned objectId = 0;
	//unsigned mtllibId = 0;

	virtual const char *nodetype() const noexcept { return "SimpleGeometryGroup"; }
};
} // namespace Fluxions

#endif
