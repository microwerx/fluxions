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

#define NOMINMAX
#define GLEW_NO_GLU
#include <GL/glew.h>
#ifndef FLUXIONS_NO_FREEGLUT
#include <GL/freeglut.h>
#endif
#include <glut_extensions.hpp>
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

#ifndef WIN32
#define APIENTRY
#endif
	void EnableGLDebugFunc();
	void APIENTRY FluxionsGLDebugFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const GLvoid *userParam);
} // namespace Fluxions

#endif // FLUXIONS_OPENGL_HPP
