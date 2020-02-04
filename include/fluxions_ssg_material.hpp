#ifndef FLUXIONS_SSG_MATERIAL_HPP
#define FLUXIONS_SSG_MATERIAL_HPP

#include <fluxions_ssg_base.hpp>

namespace Fluxions {
	struct SimpleMaterial : public BaseMaterial {
		std::string name;
		int renderIndex{ -1 };

		string_string_map maps;

		bool hasMap(const std::string& needle) const {
			return maps.count(needle);
		}
		//std::string map_Ka;
		//std::string map_Kd;
		//std::string map_Ks;
		//std::string map_Ke;
		//std::string map_Kdpbr;
		//std::string map_Kspbr;
		//std::string map_norm;
		//std::string map_bump;
		//std::string map_Ps;		// Sheen
		//std::string map_Pc;		// Clear Coat
		//std::string map_Pm;		// Metallic
	};

	//struct SimpleMaterial : BaseMaterial
	//{
	//	//Color3f Kd;
	//	//Color3f Ks;
	//	//Color3f Ke;
	//	Color3f Ka;
	//	Color3f Tf;      // Translucency;	// Tf
	//	float Tr = 0.0f; // TranslucencyLevel -- same as (1 - d)
	//	Color3f Refract;
	//	Color3f Opacity;
	//	float Ns = 0.0f; // Specular exponent [0 <= Ns <= 1000]
	//	float Ni = 0.0f; // Index of refraction [1 <= Ni <= 2.4]
	//	float d = 1.0f; // dissolved -- same as (1 - Tr)

	//	float PBm = 0.0f;         // for roughness
	//	float PBk = 0.0f;         // for metals
	//	float PBKdm = 0.0f;       // for diffuse roughness (Oren-Nayer)
	//	float PBKsm = 0.0f;       // for specular roughness (Cook-Torrance)
	//	float PBior = 0.0f;       // for Fresnel reflection (Cook-Torrance)
	//	float PBGGXgamma = 0.11f; // for GGX BRDF (0.0 - 1.0 maps to 1.0 to 10.0 in the shader)
	//	std::string PBmap;

	//	float EmissionGlossiness = 0;
	//	float ReflectFresnel = 0;
	//	float RefractGlossiness = 0;
	//	float ReflectGlossiness = 0;
	//	float AnisotropyAmount = 0;
	//	float AnisotropyRotation = 0;
	//	Color3f AttenuationColor;
	//	float AttenuationDistance = 0;
	//	Color3f ScatteringAlbedo;
	//	float MeanCosine = 0;
	//	std::vector<std::string> RefractMode;
	//	//bool HasRoundedCorners = false;
	//	//float RoundedCorners = 0;
	//	//bool IsPortal = false;
	//	//bool HasIesProfile = false;
	//	//std::string IesProfileFilename;
	//	//Matrix4f IesProfileMatrix;
	//	//bool IesProfileKeepSharp = false;
	//	std::vector<std::string> Invisible;
	//	std::string map_Ka;
	//	std::string map_Kd;
	//	std::string map_Ks;
	//	std::string map_Ke;
	//	std::string map_d;
	//	std::string map_opacity;
	//	std::string map_Ns;
	//	std::string map_Ni;
	//	std::string map_Tr; // translucency;
	//	std::string map_Tf; //Tf translucencyLevel;
	//	std::string map_refract;
	//	std::string map_anisotropy;
	//	std::string map_anisorotation;
	//	std::string map_scatteringAlbedo;
	//	std::string map_attenuation;
	//	std::string map_normal;
	//	std::string map_bump;

	//	SimpleAssociativePropertyList Properties;
	//};
}

#endif
