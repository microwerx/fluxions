#ifndef FLUXIONS_SHADER_PROGRAM_LOCATIONS_HPP
#define FLUXIONS_SHADER_PROGRAM_LOCATIONS_HPP

#include <fluxions_opengl.hpp>
#include <fluxions_ssg_property.hpp>
#include <fluxions_renderer_program.hpp>

namespace Fluxions {
	struct __ShaderProgramMapInfo {
		int mapNameLoc{ -1 };
		int mapNameMixLoc{ -1 };
		std::string mapName;
		std::string altMtlMapName;
	};

	struct __ShaderProgramLocations {
		//using int_int = std::pair<int, int>;
		//int_int map_Ka{ -1, -1 };
		//int_int map_Kd{ -1, -1 };
		//int_int map_Ks{ -1, -1 };
		//int_int map_Ke{ -1, -1 };
		//int_int map_Pc{ -1, -1 };
		//int_int map_Pm{ -1, -1 };
		//int_int map_Ps{ -1, -1 };
		//int_int map_norm{ -1, -1 };
		//int_int map_Kdroughness{ -1, -1 };
		//int_int map_Ksroughness{ -1, -1 };
		//int_int map_Kior{ -1, -1 };
		//int_int map_KmetallicSpecular{ -1, -1 };
		//int_int map_KclearcoatSheen{ -1, -1 };

		// {get<0>} MapName location, {get<1>} MapNameMix location, {get<2>} MapName
		std::vector<__ShaderProgramMapInfo> maps;

		//GLint Ka = -1;
		//GLint Kd = -1;
		//GLint Ks = -1;
		//GLint Ke = -1;
		//GLint Tr = -1;
		//GLint Tf = -1;
		//GLint Ns = -1;
		//GLint Ni = -1;

		//GLint PBKdm = -1;
		//GLint PBKsm = -1;
		//GLint PBior = -1;
		//GLint PBGGXgamma = -1;
		//GLint PBm = -1;
		//GLint PBk = -1;
		//GLint PBmap = -1;
		//GLint PBmap_mix = -1;

		//GLint map_Ka = -1;
		//GLint map_Kd = -1;
		//GLint map_Ks = -1;
		//GLint map_Ke = -1;
		//GLint map_bump = -1;
		//GLint map_normal = -1;
		//GLint map_Tr = -1;
		//GLint map_Tf = -1;
		//GLint map_Ns = -1;
		//GLint map_Ni = -1;

		//GLint map_Ka_mix = -1;
		//GLint map_Kd_mix = -1;
		//GLint map_Ks_mix = -1;
		//GLint map_Ke_mix = -1;
		//GLint map_bump_mix = -1;
		//GLint map_normal_mix = -1;
		//GLint map_Tr_mix = -1;
		//GLint map_Tf_mix = -1;
		//GLint map_Ns_mix = -1;
		//GLint map_Ni_mix = -1;

		//GLint sphere_array = -1;
		//GLint sphere_count = -1;
		//GLint sphere_Ke = -1;

		//GLint sphl_light_count = -1;
		//GLint sphl_lights_enabled[16] = { -1 };
		//GLint sphl_lights_position[16] = { -1 };
		//GLint sphl_lights_E0[16] = { -1 };
		//GLint sphl_lights_lightProbeCubeMap[16] = { -1 };
		//GLint sphl_lights_environmentCubeMap[16] = { -1 };
		//GLint sphl_lights_depthShadowCubeMap[16] = { -1 };
		//GLint sphl_lights_depthShadowZFar[16] = { -1 };
		//GLint sphl_lights_sph[16][9] = { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1} };

		//GLint toneMapScale = -1;
		//GLint toneMapExposure = -1;
		//GLint toneMapGamma = -1;
		//GLint sunShadowMap = -1;
		//GLint sunColorMap = -1;
		//GLint sunDirTo = -1;
		//GLint sunColor = -1;
		//GLint sunSize = -1;
		//GLint sunE0 = -1;
		//GLint groundE0 = -1;
		//GLint sunShadowMatrix = -1;
		//GLint enviroCubeMapAmount = -1;
		//GLint enviroCubeMap = -1;
		//GLint pbskyCubeMap = -1;
		//GLint shaderDebugChoice = -1;
		//GLint shaderDebugLight = -1;
		//GLint shaderDebugSphl = -1;

		//std::map<std::string, GLint> locations;

		//SimpleAssociativePropertyList newLocationList;

		void getMap(GLuint program, std::string&& mapName, std::string&& altMtlMapName);
		void getMaterialProgramLocations(RendererProgram& program);
	};
} // namespace Fluxions

#endif
