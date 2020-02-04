#ifndef FLUXIONS_OPENGL_HPP
#define FLUXIONS_OPENGL_HPP

#include <fluxions_stdcxx.hpp>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#endif

// GLEW must be included before gl.h
#define GLEW_NO_GLU
#include <GL/glew.h>

#ifdef FLUXIONS_USE_GLES
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef FLUXIONS_USE_FREEGLUT
#include <GL/freeglut.h>
#endif

#ifndef FLUXIONS_NO_SDL2
//#ifndef HAVE_M_PI
//#define HAVE_M_PI 1
//#endif
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#endif

namespace Fluxions {
	class OpenGLNameTranslator {
	public:
		OpenGLNameTranslator();
		int getEnum(const std::string& name) const noexcept;
		const char* getString(int id) const noexcept;
	private:
		std::map<std::string, int> enums;
		std::map<int, std::string> enum_strings;
		const std::string empty_string;
	};

	extern OpenGLNameTranslator glNameTranslator;

	class QuickGLErrorChecker {
	public:
		QuickGLErrorChecker();
		bool isError();
		void reset();
	private:
		GLenum e;
	};

#ifndef _WIN32
#define APIENTRY
#endif
	void EnableGLDebugFunc();
	void APIENTRY FluxionsGLDebugFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
} // namespace Fluxions

#endif // FLUXIONS_OPENGL_HPP
