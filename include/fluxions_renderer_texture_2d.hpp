#ifndef FLUXIONS_RENDERER_TEXTURE_2D_HPP
#define FLUXIONS_RENDERER_TEXTURE_2D_HPP

#include <fluxions_renderer_gpu_texture.hpp>

namespace Fluxions
{
	struct RendererTexture2D : public RendererGpuTexture {
		RendererTexture2D();

		const char* type() const override;
	};
}

#endif