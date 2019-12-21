#include "pch.hpp"
#include <fluxions_renderer_texture_cube.hpp>

namespace Fluxions
{
	RendererTextureCube::RendererTextureCube()
		: RendererGpuTexture(GL_TEXTURE_CUBE_MAP) {}

	const char* RendererTextureCube::type() const noexcept {
		return "RendererTextureCube";
	}
}