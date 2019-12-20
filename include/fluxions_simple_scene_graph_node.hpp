#ifndef FLUXIONS_SIMPLE_SCENE_GRAPH_NODE_HPP
#define FLUXIONS_SIMPLE_SCENE_GRAPH_NODE_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_gte_matrix4.hpp>
#include <fluxions_gte_bounding_box.hpp>

namespace Fluxions
{
	struct SimpleSceneGraphNode {
		Matrix4f transform;
		Matrix4f addlTransform;
		BoundingBoxf bbox;
		std::string nodename_;

		virtual const char* nodetype() const noexcept { return "SimpleSceneGraphNode"; }
		virtual const char* nodename() const noexcept { return nodename_.c_str(); }
	};
}

#endif