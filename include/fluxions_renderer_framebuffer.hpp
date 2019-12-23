#ifndef FLUXIONS_RENDERER_FRAMEBUFFER_OBJECT_HPP
#define FLUXIONS_RENDERER_FRAMEBUFFER_OBJECT_HPP

#include <fluxions_renderer_object.hpp>
#include <fluxions_gte_matrix4.hpp>

namespace Fluxions
{
	class RendererFramebuffer : public RendererObject
	{
		GLint lastBoundUnit = 0;

	public:
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
		void setMultisamples(GLsizei newSamples, bool newUseMultisamples);
		void setProjectionViewMatrix(const Matrix4f& M);
		void addRenderbuffer(GLenum attachment, GLenum internalformat);
		void addTexture2D(GLenum attachment, GLenum target, GLenum internalformat, bool generateMipmaps);
		void addTextureCubeMap(GLenum attachment, GLenum target, GLenum internalformat, bool generateMipmaps);
		void setMapName(const std::string& mapName);

		// Used by Renderer{GLES30, ...}
		void restoreGLState();
	private:
		void deleteBuffers();
		void bindTextures(int unit);
		void unbindTextures();
		void setAllProjectionViewMatrices(const Matrix4f& M);
		void setCurrentCubeFace(GLenum face);
		void enable(GLenum target) noexcept { enables.push_back(target); }

		static constexpr int DefaultWidth = 64;
		static constexpr int DefaultHeight = 64;
		static constexpr int DefaultSamples = 4;

		bool dirty = false;
		unsigned fbo_status = 0;
		GLuint fbo = 0;
		GLint width = DefaultWidth;
		GLint height = DefaultHeight;
		GLsizei samples = DefaultSamples;
		bool useMultisamples = false;
		GLenum internalformat = 0;
		GLenum currentCubeFace = 0;
		Matrix4f projectionViewMatrix;

		struct RenderTarget {
			std::string mapName;
			GLuint object = 0;
			GLenum target = 0;
			GLenum attachment = 0;
			GLenum internalformat = 0;
			GLsizei width = 0;
			GLsizei height = 0;
			GLint levels = 0;
			GLenum currentCubeFace = 0;
			bool useMultisamples = false;
			GLsizei samples = 0;
			GLenum readBufferTarget = 0;
			bool generateMipmaps = false;
			Matrix4f projectionViewMatrix;
			GLuint unit = 0;
			GLuint sampler = 0;
		};

	public:
		std::vector<std::pair<GLenum, RenderTarget>> renderTargets;
		std::vector<GLenum> enables;
	private:
		void setDefaultParameters();
	};

	const char* GetFramebufferStatusAsString(GLenum status);
} // namespace Fluxions

#endif
