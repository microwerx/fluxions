#include "fluxions_ssg_pch.hpp"
#include <fluxions_ssg_scene_graph.hpp>
#include <fluxions_ssg_renderer_plugin.hpp>
#include <stdexcept>

namespace Fluxions {
	ISimpleRendererPlugin::ISimpleRendererPlugin(SimpleSceneGraph* pointerToSSG) {
		if (!pointerToSSG)
			throw std::invalid_argument("pssg cannot be a nullptr");
		pssg = pointerToSSG;
		pssg->userdata = this;
	}
}
