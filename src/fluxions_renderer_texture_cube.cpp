#include "pch.hpp"
#include <fluxions_renderer_texture_cube.hpp>

namespace Fluxions
{
	RendererTextureCube::RendererTextureCube()
		: RendererGpuTexture(GL_TEXTURE_CUBE_MAP) {}

	//void RendererTextureCube::init(const std::string& name) {
	//	RendererObject::init(name);
	//	texture.init(name);
	//}

	//void RendererTextureCube::kill() {
	//	texture.kill();
	//	RendererObject::kill();
	//}
}