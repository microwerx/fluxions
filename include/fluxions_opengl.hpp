// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
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

#pragma comment (lib, "glu32.lib")    // OpenGL Utility library
#pragma comment (lib, "opengl32.lib") // Microsoft OpenGL library
#pragma comment (lib, "gdi32.lib")    // Windows GDI library
#pragma comment (lib, "winmm.lib")    // Windows MultiMedia library
#pragma comment (lib, "user32.lib")   // Windows User library
#endif

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

#define GLEW_NO_GLU
#include <GL/glew.h>

#ifdef FLUXIONS_USE_FREEGLUT
#include <GL/freeglut.h>
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
		void Reset();
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
