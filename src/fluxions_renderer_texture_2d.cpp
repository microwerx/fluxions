#include "pch.hpp"
#include <fluxions_renderer_texture_2d.hpp>

namespace Fluxions
{
	void RendererTexture2D::init(const std::string& name) {
		RendererObject::init(name);
		texture.init(name);
	}

	void RendererTexture2D::kill() {
		texture.kill();
		RendererObject::kill();
	}
}