#include "fluxions_renderer_pch.hpp"
#include <fluxions_renderer_texture_2d.hpp>

namespace Fluxions {
	RendererTexture2D::RendererTexture2D()
		:RendererGpuTexture(GL_TEXTURE_2D) {}

	RendererTexture2D::~RendererTexture2D() {}

	const char* RendererTexture2D::type() const {
		return "RendererTexture2D";
	}
}
