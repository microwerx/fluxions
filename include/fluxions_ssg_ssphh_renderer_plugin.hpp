#ifndef FLUXIONS_SSG_SSPHH_RENDERER_PLUGIN_HPP
#define FLUXIONS_SSG_SSPHH_RENDERER_PLUGIN_HPP

#include <string>
#include <vector>
#include <sstream>
#include <fluxions_simple_scene_graph.hpp>
#include <fluxions_simple_ssphh_light.hpp>

namespace Fluxions
{
	class SSG_SSPHHRendererPlugin : public ISimpleRendererPlugin
	{
	private:
		struct UniformLocs
		{
			GLint sphl_light_count = -1;
			GLint sphl_lights_enabled[16] = { -1 };
			GLint sphl_lights_position[16] = { -1 };
			GLint sphl_lights_E0[16] = { -1 };
			GLint sphl_lights_lightProbeCubeMap[16] = { -1 };
			GLint sphl_lights_environmentCubeMap[16] = { -1 };
			GLint sphl_lights_depthShadowCubeMap[16] = { -1 };
			GLint sphl_lights_depthShadowZFar[16] = { -1 };
			GLint sphl_lights_sph[16][9] = { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1} };
		} locs;
	public:
		SSG_SSPHHRendererPlugin(SimpleSceneGraph* pointerToSSG);

		std::vector<SimpleSSPHHLight> ssphhLights;

		bool Read(const std::string& cmd, std::istringstream& istr) override;
		bool ReadUniformLocs(SimpleProgram& program) override;
		bool Prog() override;
		bool Draw() override;
		bool Init() override;
		bool Redo() override;
		bool Reset() override;
	private:
		SimpleSceneGraph* pssg = nullptr;
	};
}

#endif
