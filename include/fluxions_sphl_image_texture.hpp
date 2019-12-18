#ifndef FLUXIONS_SPHL_IMAGE_TEXTURE_HPP
#define FLUXIONS_SPHL_IMAGE_TEXTURE_HPP

#include <fluxions_gte_image.hpp>
#include <fluxions_gte_spherical_harmonic.hpp>
#include <fluxions_renderer_texture_cube.hpp>

namespace Fluxions
{
	struct SphlImageTexture
	{
		Image4f lightProbe;
		RendererTextureCube texture;

		bool loadLightProbe(const std::string& path);
		bool sphToLightProbe(const MultispectralSph4f& sph);
		bool uploadLightProbe();
	};
}

#endif // FLUXIONS_SPHL_IMAGE_TEXTURE_HPP