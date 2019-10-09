#ifndef FLUXIONS_SIMPLE_GPU_TEXTURE_HPP
#define FLUXIONS_SIMPLE_GPU_TEXTURE_HPP

#include <memory>
#include <fluxions_opengl.hpp>

namespace Fluxions
{
	struct SimpleGpuTexture
	{
		SimpleGpuTexture(GLenum target);
		~SimpleGpuTexture();

		void Bind(int unit);
		void Unbind();
		void CreateStorage(GLenum internalformat, GLint width, GLint height, GLenum format, GLenum type);
		void CreateTexture2D(GLsizei width = 64, GLsizei height = 64);
		void CreateTextureShadow2D(GLsizei width = 64, GLsizei height = 64);
		void CreateTextureCube(GLsizei size = 64);
		void CreateTextureShadowCube(GLsizei size = 64);
		void SetDefaultParameters(GLenum minFilter = GL_NEAREST, GLenum magFilter = GL_NEAREST, GLenum wrapMode = GL_CLAMP_TO_EDGE);
		void GenerateMipmap();
		GLuint GetTexture() const;
	private:
		GLenum target_ = GL_TEXTURE_CUBE_MAP;
		int lastUnitBound_ = -1;
		bool created_ = false;
		std::shared_ptr<GLuint> texture_;
	}; // class SimpleGpuTexture
} // namespace Fluxions

#endif