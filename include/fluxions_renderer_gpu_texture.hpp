#ifndef FLUXIONS_SIMPLE_GPU_TEXTURE_HPP
#define FLUXIONS_SIMPLE_GPU_TEXTURE_HPP

#include <fluxions_renderer_object.hpp>

namespace Fluxions
{
	struct RendererGpuTexture : public RendererObject {
		RendererGpuTexture(GLenum target);
		~RendererGpuTexture();

		void init(const std::string& name) override;
		void kill() override;

		void bind(int unit);
		void unbind();
		void createStorage(GLenum internalformat, GLint width, GLint height, GLenum format, GLenum type);
		void createTexture2D(GLsizei width = 64, GLsizei height = 64);
		void createTextureShadow2D(GLsizei width = 64, GLsizei height = 64);
		void createTextureCube(GLsizei size = 64);
		void createTextureShadowCube(GLsizei size = 64);
		void setDefaultParameters(GLenum minFilter = GL_NEAREST, GLenum magFilter = GL_NEAREST, GLenum wrapMode = GL_CLAMP_TO_EDGE);
		void generateMipmap();
		GLuint getTexture() const;
	private:
		GLenum target_ = GL_TEXTURE_CUBE_MAP;
		int lastUnitBound_ = -1;
		bool created_ = false;
		std::shared_ptr<GLuint> texture_;
	}; // class RendererGpuTexture
} // namespace Fluxions

#endif