#ifndef FLUXIONS_SIMPLE_CUBE_TEXTURE_HPP
#define FLUXIONS_SIMPLE_CUBE_TEXTURE_HPP

#include <fluxions_renderer_object.hpp>
#include <fluxions_renderer_gpu_texture.hpp>

namespace Fluxions
{
	struct RendererTextureCube : public RendererGpuTexture
	{
		RendererTextureCube();
		const char* type() const noexcept override;
	};
} // namespace Fluxions

#endif