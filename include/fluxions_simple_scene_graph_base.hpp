#ifndef FLUXIONS_SIMPLE_SCENE_GRAPH_BASE_HPP
#define FLUXIONS_SIMPLE_SCENE_GRAPH_BASE_HPP

#include <fluxions_gte_color_math.hpp>
#include <fluxions_gte_matrix_extra.hpp>

namespace Fluxions{
	// 128 bytes
	struct BaseEnvironment {
		Vector4f toneMap;		// {r} = exposure, {g} = gamma, {b} = filmic highlight, {a} = filmic shadows
		Vector4f fogSH;			// {r} = L0, {g} = L1,M-1, {b} = L1,M0, {a} = L1,M1
		Color4f ground;			// {rgb} = ground color, {a} = ground luminance
		Vector4f location;		// {r} = latitude, {g} = longitude, {b} = altitude, {a} = ground altitude
		Vector4f date;			// {r} = year, {g} = month, {b} = day, {a} = jday
		Vector4f time;			// {r} = hour, {g} = minutes, {b} = seconds, {a} day seconds
		Color4f sun;			// {rgb} = disk radiance, {a} = solid angle
		Color4f moon;			// {rgb} = disk radiance, {a} = solid angle
	};

	// 64 bytes
	struct BaseDirToLight {
		Vector4f dirTo;		// {xyz} = direction to light, {a} = solid angle
		Vector4f shadow;	// {r} = znear, {g} = zfar, {ba} = ????
		Vector4f shape;		// {rgb} = n1, n2, n3, {a} = m
		Color4f E0;			// {rgb} = colored E0, {a} = luminance E0
	};

	// 64 bytes
	struct BasePointLight {
		Vector4f position;	// {xyz} = position of light, {a} = falloff radius
		Vector4f shadow;	// {r} = znear, {g} = zfar, {ba} = shape theta/phi
		Vector4f shape;		// {rgb} = n1, n2, n3, {a} = m
		Color4f E0;			// {rgb} = colored E0, {a} = luminance E0
	};

	// 2048 bytes
	struct BaseAnisoLight {
		Vector4f position;	// {xyz} = position of light, {a} = falloff radius
		Vector4f shadow;	// {r} = znear, {g} = zfar, {ba} = shape theta/phi
		Vector4f shape;     // {rgb} = n1, n2, n3, {a} = m
		Vector4f shapeSH;	// {r} = width, {g} = height, {b} = , {a} = maxDegree
		Matrix4f rotation;	// rotation of SPH
		Color4f SH[121];	// {rgb} = colored E0, {a} = SH coefficient
	};

	// 128 bytes of material information
	struct BaseMaterial {
		Color4f Kd;						// {rgb} = diffuse reflection, {a} = map mix amount
		Color4f Ks;						// {rgb} = specular reflection, {a} = map mix amount
		Color4f Ke;						// {rgb} = emissive light, {a} = map mix amount
		Color4f Kdroughness;			// {r} = roughness, {g} = Disney/Oren-Nayer mix, {b} = subsurface, {a} = map mix amount
		Color4f Ksroughness;			// {r} = roughness, {g} = GGX gamma, {b} = anisotropy, {a} = map mix amount
		Color4f Kior;					// {rgb} = specular index of refraction, {a} = diffuse index of refraction
		Color4f KmetallicSpecular;		// {r} = metallic, {g} = specular, {b} = specular tint, {a} = anisotropy angle
		Color4f KclearcoatSheen;		// {r} = clearcoat, {g} = gloss, {b} = sheen, {a} = sheen tint
	};
}

#endif