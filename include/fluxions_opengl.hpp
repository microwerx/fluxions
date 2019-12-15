// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2019 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com
#ifndef FLUXIONS_OPENGL_HPP
#define FLUXIONS_OPENGL_HPP

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

#include <fluxions_draw_gl1gl2.hpp>
#include <map>
#include <string>
#include <hatchetfish.hpp>

namespace Fluxions
{
	class OpenGLNameTranslator
	{
	public:
		OpenGLNameTranslator();
		int GetEnum(const std::string &name) const noexcept;
		const char *GetString(int id) const noexcept;
	private:
		std::map<std::string, int> enums;
		std::map<int, std::string> enum_strings;
		const std::string empty_string;
	};

	extern OpenGLNameTranslator glNameTranslator;

	class QuickGLErrorChecker
	{
	public:
		QuickGLErrorChecker();
		bool IsError();
		void reset();
	private:
		GLenum e;
	};

#ifndef _WIN32
#define APIENTRY
#endif
	void EnableGLDebugFunc();
	void APIENTRY FluxionsGLDebugFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const GLvoid *userParam);
} // namespace Fluxions

#endif // FLUXIONS_OPENGL_HPP
