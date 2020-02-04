#include "fluxions_renderer_pch.hpp"
#include <fluxions_shader_program_locations.hpp>

namespace Fluxions {
	void __ShaderProgramLocations::getMap(GLuint program, std::string&& mapName, std::string&& altMtlMapName) {
		int loc1 = glGetUniformLocation(program, mapName.c_str());
		int loc2 = glGetUniformLocation(program, (mapName + "Mix").c_str());
		maps.push_back({ loc1, loc2, std::move(mapName), std::move(altMtlMapName) });
	}

	void __ShaderProgramLocations::getMaterialProgramLocations(RendererProgram& program) {
		GLuint p = program.getProgram();
		maps.clear();
		getMap(p, "MapKa", "map_Ka");
		getMap(p, "MapKd", "map_Kd");
		getMap(p, "MapKs", "map_Ks");
		getMap(p, "MapKe", "map_Ke");
		getMap(p, "MapPc", "map_Pc");
		getMap(p, "MapPm", "map_Pm");
		getMap(p, "MapPs", "map_Ps");
		getMap(p, "MapNorm", "map_norm");
		getMap(p, "MapNorm", "map_normal");
		getMap(p, "MapKdroughness", "map_Kdroughness");
		getMap(p, "MapKsroughness", "map_Ksroughness");
		getMap(p, "MapKior", "map_Kior");
		getMap(p, "MapKmetallicSpecular", "map_Kmetallic");
		getMap(p, "MapKmetallicSpecular", "map_Kmetallicspecular");
		getMap(p, "MapKclearcoatSheen", "map_clearcoat");
		getMap(p, "MapKclearcoatSheen", "map_clearcoatsheen");
	}

	//void __ShaderProgramLocations::getMaterialProgramLocations(
	//	RendererProgram& program) {
	//	locations.clear();
	//	locations["Ka"] = Ka = program.getUniformLocation("Ka");
	//	locations["Kd"] = Kd = program.getUniformLocation("Kd");
	//	locations["Ks"] = Ks = program.getUniformLocation("Ks");
	//	locations["Ke"] = Ke = program.getUniformLocation("Ke");
	//	locations["Tr"] = Tr = program.getUniformLocation("Tr");
	//	locations["Tf"] = Tf = program.getUniformLocation("Tf");
	//	locations["Ns"] = Ns = program.getUniformLocation("Ns");
	//	locations["Ni"] = Ni = program.getUniformLocation("Ni");

	//	locations["PBk"] = PBk = program.getUniformLocation("PBk");
	//	locations["PBm"] = PBm = program.getUniformLocation("PBm");
	//	locations["PBKdm"] = PBKdm = program.getUniformLocation("PBKdm");
	//	locations["PBKsm"] = PBKsm = program.getUniformLocation("PBKsm");
	//	locations["PBior"] = PBior = program.getUniformLocation("PBior");
	//	locations["PBGGXgamma"] = PBGGXgamma =
	//		program.getUniformLocation("PBGGXgamma");
	//	locations["PBmap"] = PBmap = program.getUniformLocation("PBmap");
	//	///
	//	locations["map_Ka"] = map_Ka = program.getUniformLocation("map_Ka");
	//	locations["map_Kd"] = map_Kd = program.getUniformLocation("map_Kd");
	//	locations["map_Ks"] = map_Ks = program.getUniformLocation("map_Ks");
	//	locations["map_Ke"] = map_Ke = program.getUniformLocation("map_Ke");
	//	locations["map_Tr"] = map_Tr = program.getUniformLocation("map_Tr");
	//	locations["map_Tf"] = map_Tf = program.getUniformLocation("map_Tf");
	//	locations["map_Ni"] = map_Ni = program.getUniformLocation("map_Ni");
	//	locations["map_Ns"] = map_Ns = program.getUniformLocation("map_Ns");
	//	locations["map_bump"] = map_bump = program.getUniformLocation("map_bump");
	//	locations["map_normal"] = map_normal =
	//		program.getUniformLocation("map_normal");
	//	locations["map_Ka_mix"] = map_Ka_mix =
	//		program.getUniformLocation("map_Ka_mix");
	//	locations["map_Kd_mix"] = map_Kd_mix =
	//		program.getUniformLocation("map_Kd_mix");
	//	locations["map_Ks_mix"] = map_Ks_mix =
	//		program.getUniformLocation("map_Ks_mix");
	//	locations["map_Ke_mix"] = map_Ke_mix =
	//		program.getUniformLocation("map_Ke_mix");
	//	locations["map_Tf_mix"] = map_Tf_mix =
	//		program.getUniformLocation("map_Tf_mix");
	//	locations["map_Tr_mix"] = map_Tr_mix =
	//		program.getUniformLocation("map_Tr_mix");
	//	locations["map_Ni_mix"] = map_Ni_mix =
	//		program.getUniformLocation("map_Ni_mix");
	//	locations["map_Ns_mix"] = map_Ns_mix =
	//		program.getUniformLocation("map_Ns_mix");
	//	locations["map_bump_mix"] = map_bump_mix =
	//		program.getUniformLocation("map_bump_mix");
	//	locations["map_normal_mix"] = map_normal_mix =
	//		program.getUniformLocation("map_normal_mix");

	//	locations["Spheres"] = sphere_array =
	//		program.getUniformLocation("Spheres[0]");
	//	locations["SpheresCount"] = sphere_count =
	//		program.getUniformLocation("SpheresCount");
	//	locations["SpheresKe"] = sphere_Ke =
	//		program.getUniformLocation("SpheresKe[0]");

	//	locations["ToneMapScale"] = toneMapScale =
	//		program.getUniformLocation("ToneMapScale");
	//	locations["ToneMapExposure"] = toneMapExposure =
	//		program.getUniformLocation("ToneMapExposure");
	//	locations["ToneMapGamma"] = toneMapGamma =
	//		program.getUniformLocation("ToneMapGamma");
	//	locations["SunDirTo"] = sunDirTo = program.getUniformLocation("SunDirTo");
	//	locations["SunColor"] = sunColor = program.getUniformLocation("SunColor");
	//	locations["SunSize"] = sunSize = program.getUniformLocation("SunSize");
	//	locations["SunShadowMap"] = sunShadowMap =
	//		program.getUniformLocation("SunShadowMap");
	//	locations["SunColorMap"] = sunColorMap =
	//		program.getUniformLocation("SunColorMap");
	//	locations["SunShadowMatrix"] = sunShadowMatrix =
	//		program.getUniformLocation("SunShadowMatrix");
	//	locations["SunE0"] = sunE0 = program.getUniformLocation("SunE0");
	//	locations["GroundE0"] = groundE0 = program.getUniformLocation("GroundE0");

	//	locations["EnviroCubeMap"] = enviroCubeMap =
	//		program.getUniformLocation("EnviroCubeMap");
	//	locations["EnviroCubeMapAmount"] = enviroCubeMapAmount =
	//		program.getUniformLocation("EnviroCubeMapAmount");

	//	locations["PBSkyCubeMap"] = pbskyCubeMap =
	//		program.getUniformLocation("PBSkyCubeMap");

	//	locations["ShaderDebugChoice"] = shaderDebugChoice =
	//		program.getUniformLocation("ShaderDebugChoice");
	//	locations["ShaderDebugLight"] = shaderDebugLight =
	//		program.getUniformLocation("ShaderDebugLight");
	//	locations["ShaderDebugSphl"] = shaderDebugSphl =
	//		program.getUniformLocation("ShaderDebugSphl");

	//	// TODO: Do the new way by finding out what uniforms the program
	//	// actually has, or is looking for...
	//	newLocationList.Clear();

	//	for (auto uniform : program.activeUniforms) {
	//		// uniform.first is the mapname of the uniform...
	//		// uniform.second has the details...

	//		newLocationList[uniform.first] =
	//			MakeSharedIntProperty(uniform.second.index);
	//	}
	//}
} // namespace Fluxions
