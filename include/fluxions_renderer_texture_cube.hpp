#ifndef FLUXIONS_SIMPLE_CUBE_TEXTURE_HPP
#define FLUXIONS_SIMPLE_CUBE_TEXTURE_HPP

#include <fluxions_renderer_object.hpp>
#include <fluxions_renderer_gpu_texture.hpp>

namespace Fluxions
{
	struct RendererTextureCube : public RendererGpuTexture
	{
		GLint unit = 0;
		GLint samplerId = 0;
		double buildTime = 0.0;
		float zfar = 0.0f;
		//RendererGpuTexture texture{ GL_TEXTURE_CUBE_MAP };
		bool dirty = true;
		
		RendererTextureCube();

		//void init(const std::string& name) override;
		//void kill() override;
	};
} // namespace Fluxions

#endif