#include "pch.hpp"
#include <fluxions_simple_scene_graph.hpp>
#include <fluxions_simple_renderer_plugin.hpp>
#include <stdexcept>

namespace Fluxions
{
	ISimpleRendererPlugin::ISimpleRendererPlugin(SimpleSceneGraph* pointerToSSG) {
		if (!pointerToSSG)
			throw std::invalid_argument("pssg cannot be a nullptr");
		pssg = pointerToSSG;
		pssg->userdata = this;
	}
}