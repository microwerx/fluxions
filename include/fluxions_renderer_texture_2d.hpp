#ifndef FLUXIONS_RENDERER_TEXTURE_2D_HPP
#define FLUXIONS_RENDERER_TEXTURE_2D_HPP

#include <fluxions_renderer_gpu_texture.hpp>

namespace Fluxions
{
	struct RendererTexture2D : public RendererGpuTexture {
		GLint unit = 0;
		GLint samplerId = 0;
		double buildTime = 0.0;
		float zfar = 0.0f;
		//RendererGpuTexture texture{ GL_TEXTURE_2D };
		bool dirty = true;

		std::string mappath;
		std::string uniformname;
		std::string samplername;

		RendererTexture2D();

		const char* type() const override;
	};
}

#endif