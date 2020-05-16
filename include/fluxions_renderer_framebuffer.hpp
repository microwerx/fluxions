#ifndef FLUXIONS_RENDERER_FRAMEBUFFER_OBJECT_HPP
#define FLUXIONS_RENDERER_FRAMEBUFFER_OBJECT_HPP

#include <fluxions_renderer_object.hpp>
#include <fluxions_gte_matrix4.hpp>

namespace Fluxions {
	class RendererGpuTexture;

	class RendererFramebuffer : public RendererObject {
		GLint lastBoundUnit = 0;

	public:
		bool viewportAutoresize{ false };

		RendererFramebuffer();
		~RendererFramebuffer();

		void init(const std::string& name, RendererObject* pparent = nullptr) override;
		void kill() override;
		const char* type() const override;
		const char* status() const override;
		bool usable() const override;

		bool make();
		void useForWriting();
		void useForReading();
		void use();
		void generateMipmaps();
		void setDimensions(GLsizei newWidth, GLsizei newHeight);
		void setLayers(GLsizei newLayers);
		void setMultisamples(GLsizei newSamples, bool newUseMultisamples);
		void setProjectionViewMatrix(const Matrix4f& M);
		void addRenderbuffer(GLenum attachment, GLenum internalformat);
		void addTexture2D(GLenum attachment, GLenum target, GLenum internalformat, bool generateMipmaps);
		void addTextureCubeMap(GLenum attachment, GLenum target, GLenum internalformat, bool generateMipmaps);
		void addTexture2DArrays(GLenum attachment, GLenum textarget, GLenum internalformat, bool generateMipmaps);
		void setCurrentLayer(GLint layer);
		void setMapName(const std::string& mapName);

		// Used by Renderer{GLES30, ...}
		void unbind();

		int width() const { return width_; }
		int height() const { return height_; }
	private:
		void deleteBuffers();
		void bindTextures(int unit);
		void unbindTextures();
		void setAllProjectionViewMatrices(const Matrix4f& M);
		void enable(GLenum target) noexcept { enables.push_back(target); }

		static constexpr int DefaultLayers = 1;
		static constexpr int DefaultWidth = 64;
		static constexpr int DefaultHeight = 64;
		static constexpr int DefaultSamples = 4;
		static constexpr int LowQuality = 0;	// Use GL_UNSIGNED_BYTE and GL_DEPTH_COMPONENT16
		static constexpr int MediumQuality = 1; // Use GL_UNSIGNED_SHORT and GL_DEPTH_COMPONENT24
		static constexpr int HighQuality = 2;	// Use GL_FLOAT and GL_DEPTH_COMPONENT32F
		static constexpr int DefaultQuality = MediumQuality;

		bool dirty = false;
		unsigned fbo_status = 0;
		GLuint fbo = 0;
		GLint quality = DefaultQuality;
		GLint width_ = DefaultWidth;
		GLint height_ = DefaultHeight;
		GLint layers_ = DefaultLayers;
		GLsizei samples = DefaultSamples;
		bool useMultisamples = false;
		GLenum internalformat = 0;
		GLenum currentLayer = 0;
		Matrix4f projectionViewMatrix;

		struct RenderTarget {
			std::string mapName;
			RendererGpuTexture* pGpuTexture{ nullptr };
			GLuint object{ 0 };
			GLenum target{ 0 };
			//GLsizei width{ 0 };
			//GLsizei height{ 0 };

			GLenum attachment{ 0 };
			GLenum internalformat{ 0 };
			GLenum format{ 0 };
			GLenum type{ 0 };

			GLint layers{ 1 };
			GLint levels{ 0 };
			GLint currentLayer{ 0 };
			bool useMultisamples = false;
			GLsizei samples = 0;
			GLenum readBufferTarget = 0;
			bool generateMipmaps = false;
			Matrix4f projectionViewMatrix;
			int unit = 0;
			int sampler = 0;
		};

		void _setFormats(RenderTarget& rt);

	public:
		std::vector<std::pair<GLenum, RenderTarget>> renderTargets;
		std::vector<GLenum> enables;
	private:
		void setDefaultParameters();
	};
} // namespace Fluxions

#endif
