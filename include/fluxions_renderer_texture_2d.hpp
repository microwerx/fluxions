#ifndef FLUXIONS_RENDERER_TEXTURE_2D_HPP
#define FLUXIONS_RENDERER_TEXTURE_2D_HPP

#include <fluxions_renderer_gpu_texture.hpp>

namespace Fluxions {
	class RendererTexture2D : public RendererGpuTexture {
	public:
		RendererTexture2D();
		~RendererTexture2D() override;

		const char* type() const override;
	};
}

#endif
