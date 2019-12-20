#include "pch.hpp"
#include <fluxions_renderer_texture_2d.hpp>

namespace Fluxions
{
	void RendererTexture2D::init(const std::string& name, RendererObject* pparent) {
		RendererObject::init(name, pparent);
		texture.init(name, this);
	}

	void RendererTexture2D::kill() {
		texture.kill();
		RendererObject::kill();
	}

	const char* RendererTexture2D::type() const {
		return "RendererTexture2D";
	}
}