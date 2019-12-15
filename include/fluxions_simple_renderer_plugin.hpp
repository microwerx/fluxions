#ifndef FLUXIONS_SIMPLE_RENDERER_PLUGIN_HPP
#define FLUXIONS_SIMPLE_RENDERER_PLUGIN_HPP

#include <fluxions_simple_program.hpp>
#include <fluxions_simple_scene_graph.hpp>

namespace Fluxions
{
	class SimpleSceneGraph;

	struct ISimpleRendererPlugin
	{
		ISimpleRendererPlugin(SimpleSceneGraph* pointerToSSG);

		virtual bool reset() = 0;
		virtual bool Init() = 0;
		virtual bool Redo() = 0;
		virtual bool Read(const std::string& cmd, std::istringstream& istr) = 0;
		virtual bool ReadUniformLocs(SimpleProgram& program) = 0;
		virtual bool Prog() = 0;
		virtual bool Draw() = 0;

		SimpleSceneGraph* pssg;
	}; // class ISimpleRendererPlugin
} // namespace Fluxions

#endif