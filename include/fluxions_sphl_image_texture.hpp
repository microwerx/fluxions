#ifndef FLUXIONS_SPHL_IMAGE_TEXTURE_HPP
#define FLUXIONS_SPHL_IMAGE_TEXTURE_HPP

#include <fluxions_gte_image.hpp>
#include <fluxions_gte_spherical_harmonic.hpp>
#include <fluxions_simple_gpu_texture.hpp>

namespace Fluxions
{
	struct SphlImageTexture
	{
		Image4f lightProbe;
		SimpleGpuTexture texture = SimpleGpuTexture(GL_TEXTURE_CUBE_MAP);

		bool LoadLightProbe(const std::string& path);
		bool SphToLightProbe(const MultispectralSph4f& sph);
		bool UploadLightProbe();
	};
}

#endif // FLUXIONS_SPHL_IMAGE_TEXTURE_HPP