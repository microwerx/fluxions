#ifndef FLUXIONS_RENDERER_GPU_TEXTURE_HPP
#define FLUXIONS_RENDERER_GPU_TEXTURE_HPP

#include <fluxions_gte_image.hpp>
#include <fluxions_renderer_object.hpp>

namespace Fluxions {
	class RendererGpuTexture : public RendererObject {
	public:
		GLint unit = 0;
		GLint samplerId = 0;

		// This value is used to count how long it took to build this object
		double buildTime = 0.0;
		// This zfar is used to for rendering shadow maps
		float zfar = 0.0f;
		// This flag is used to signal an external process to rerender this texture
		bool dirty = true;
		// This flag is used to determine whether to build mip maps
		bool useMipMaps = true;
		// This flag is used to determine whether to load the image every time
		bool alwaysLoad{ false };
		// This option is used to determine how the input image should be swizzled
		int onloadSwizzle{ 0 };
		// This option is used to determine if the image should be flipped when uploaded
		bool onloadFlipY{ false };

		std::string mappath;
		bool maploaded{ false };
		std::string uniformname;
		std::string samplername;

		RendererGpuTexture(GLenum target);
		~RendererGpuTexture() override;

		void init(const std::string& name, RendererObject* pparent = nullptr) override;
		void kill() override;
		const char* type() const override;
		const char* keyword() const override { return "texture"; }

		bool loadMap(const std::string& path, bool generateMipmaps);
		bool loadMap();

		void bind(int unit);
		void unbind();
		void create();
		void createStorage(GLenum internalformat, GLint width, GLint height, GLint depth, GLenum format, GLenum type);
		void createTexture2D(GLsizei width = 64, GLsizei height = 64);
		void createTextureShadow2D(GLsizei width = 64, GLsizei height = 64);
		void createTextureCube(GLsizei size = 64);
		void createTextureShadowCube(GLsizei size = 64);
		void createTexture2DArray(GLsizei width = 64, GLsizei height = 64, GLsizei depth = 64);
		void createTexture2DArrayShadow(GLsizei width = 64, GLsizei height = 64, GLsizei depth = 64);
		void setTextureCube(const Image3f& image);
		void setTextureCube(const Image3ub& image);
		void setTextureCube(const Image3us& image);
		void setTextureCube(const Image4f& image);
		void setTextureCube(const Image4ub& image);
		void setTextureCube(const Image4us& image);
		void setTexture2D(const Image3f& image, unsigned arrayElement = 0);
		void setTexture2D(const Image3ub& image, unsigned arrayElement = 0);
		void setTexture2D(const Image3us& image, unsigned arrayElement = 0);
		void setTexture2D(const Image4f& image, unsigned arrayElement = 0);
		void setTexture2D(const Image4ub& image, unsigned arrayElement = 0);
		void setTexture2D(const Image4us& image, unsigned arrayElement = 0);
		void setDefaultParameters(GLenum minFilter = GL_NEAREST, GLenum magFilter = GL_NEAREST, GLenum wrapMode = GL_CLAMP_TO_EDGE);
		void generateMipmap();
		GLuint getTexture() const;
		GLuint getTarget() const { return target_; }
	private:
		void _setTexture2D(int width, int height, unsigned format, unsigned type, unsigned arrayElement, const void* data);
		GLenum target_ = GL_TEXTURE_CUBE_MAP;
		int lastUnitBound_ = -1;
		bool created_ = false;
		std::shared_ptr<GLuint> texture_;
	}; // class RendererGpuTexture

	inline void DeleteRendererGpuTexture(RendererGpuTexture** t) {
		if (!*t) return;
		delete t;
		*t = nullptr;
	}
} // namespace Fluxions

#endif
