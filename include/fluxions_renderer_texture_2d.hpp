#ifndef FLUXIONS_RENDERER_TEXTURE_2D_HPP
#define FLUXIONS_RENDERER_TEXTURE_2D_HPP

#include <fluxions_renderer_gpu_texture.hpp>

namespace Fluxions
{
	struct RendererTexture2D : public RendererObject {
		GLint unit = 0;
		GLint samplerId = 0;
		double buildTime = 0.0;
		float zfar = 0.0f;
		RendererGpuTexture texture{ GL_TEXTURE_2D };
		bool dirty = true;

		std::string mappath;
		std::string uniformname;
		std::string samplername;

		void init(const std::string& name, RendererObject* pparent = nullptr) override;
		void kill() override;
		const char* type() const override;
	};
}

#endif