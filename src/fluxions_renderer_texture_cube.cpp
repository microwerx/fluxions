#include "fluxions_renderer_pch.hpp"
#include <fluxions_renderer_texture_cube.hpp>

namespace Fluxions {
	RendererTextureCube::RendererTextureCube()
		: RendererGpuTexture(GL_TEXTURE_CUBE_MAP) {}

	RendererTextureCube::~RendererTextureCube() {}

	const char* RendererTextureCube::type() const noexcept {
		return "RendererTextureCube";
	}
}
