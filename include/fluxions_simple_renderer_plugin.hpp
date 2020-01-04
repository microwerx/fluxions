#ifndef FLUXIONS_SIMPLE_RENDERER_PLUGIN_HPP
#define FLUXIONS_SIMPLE_RENDERER_PLUGIN_HPP

#include <fluxions_base.hpp>

namespace Fluxions
{
	class SimpleSceneGraph;
	class RendererContext;
	class RendererGLES30;
	class RendererProgram;

	struct ISimpleRendererPlugin
	{
		ISimpleRendererPlugin(SimpleSceneGraph* pointerToSSG);

		virtual bool reset() = 0;
		virtual bool init() = 0;
		virtual bool redo() = 0;
		virtual bool read(const std::string& cmd, std::istringstream& istr) = 0;
		virtual bool readUniformLocs(RendererProgram* rc_program_ptr) = 0;
		virtual bool prog() = 0;
		virtual bool draw() = 0;

		SimpleSceneGraph* pssg;
		RendererContext* pRendererContext;
		RendererGLES30* pRenderer;
	}; // class ISimpleRendererPlugin
} // namespace Fluxions

#endif