#ifndef FLUXIONS_RENDERER_SAMPLER_OBJECT_HPP
#define FLUXIONS_RENDERER_SAMPLER_OBJECT_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_renderer_object.hpp>

namespace Fluxions {
	class RendererSampler : public RendererObject {
	public:
		RendererSampler();
		~RendererSampler();

		void init(const std::string& newname, RendererObject* pparent) override;
		void kill() override;
		const char* type() const noexcept override;

		void bind(GLuint unit);
		void unbind();

		void setWrapS(GLint param);
		void setWrapT(GLint param);
		void setWrapR(GLint param);
		void setWrapST(GLint S, GLint T);
		void setWrapTR(GLint T, GLint R);
		void setWrapSTR(GLint S, GLint T, GLint R);
		void setMinFilter(GLint minFilter);
		void setMagFilter(GLint magFilter);
		void setMinMagFilters(GLint minFilter, GLint magFilter);
		void setCompareFunction(GLint func);
		void setCompareMode(GLint mode);
		void setMinLOD(GLfloat lod);
		void setMaxLOD(GLfloat lod);

		void applySettings(GLuint textureUnit, GLuint texture, GLenum target);
		void use(GLenum textureUnit, GLuint texture, GLenum target);

		GLuint getId() const { return id; }
		GLint getWrapS() const { return wrapS; }
		GLint getWrapT() const { return wrapT; }
		GLint getWrapR() const { return wrapR; }
		GLint getMinFilter() const { return minFilter; }
		GLint getMagFilter() const { return magFilter; }
		GLint getCompareFunc() const { return compareFunc; }
		GLint getCompareMode() const { return compareMode; }
		GLfloat getMinLOD() const { return maxLOD; }
		GLfloat getMaxLOD() const { return maxLOD; }

	private:
		GLint wrapS;
		GLint wrapT;
		GLint wrapR;
		GLint minFilter;
		GLint magFilter;
		GLint compareMode;
		GLint compareFunc;
		GLfloat minLOD;
		GLfloat maxLOD;
		GLuint id;

		GLint lastBoundUnit = -1;
	};
}

#endif
