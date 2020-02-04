#ifndef FLUXIONS_RENDERER_CUBE_TEXTURE_HPP
#define FLUXIONS_RENDERER_CUBE_TEXTURE_HPP

#include <fluxions_renderer_object.hpp>
#include <fluxions_renderer_gpu_texture.hpp>

namespace Fluxions {
	class RendererTextureCube : public RendererGpuTexture {
	public:
		RendererTextureCube();
		~RendererTextureCube() override;
		const char* type() const noexcept override;
		const char* keyword() const override { return "textureCube"; }
	};
} // namespace Fluxions

#endif
