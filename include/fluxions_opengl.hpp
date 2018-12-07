// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
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
#define GLEW_STATIC
#include <GL/glew.h>
#ifdef WIN32
#include <GL/wglew.h>
#endif
#include <GL/freeglut.h>
#include "glut_extensions.hpp"
#include <map>
#include <string>
#include <hatchetfish.hpp>

namespace Fluxions
{
using namespace std;

class OpenGLNameTranslator
{
	map<string, int> enums;
	map<int, string> enum_strings;
	const string empty_string;

  public:
	OpenGLNameTranslator();

	inline int GetEnum(const string &name) const
	{
		auto it = enums.find(name);
		if (it == enums.end())
			it = enums.find(string("GL_") + name);
		if (it == enums.end())
			return 0;
		return it->second;
	}

	inline const char *GetString(int id) const
	{
		auto it = enum_strings.find(id);
		if (it != enum_strings.end())
			return it->second.c_str();
		return empty_string.c_str();
	}
};

extern OpenGLNameTranslator glNameTranslator;

class QuickGLErrorChecker
{
  public:
	QuickGLErrorChecker() { e = glGetError(); }

	bool IsError()
	{
		e = glGetError();
		if (e != GL_NO_ERROR)
		{
			hflog.error("%s(): OpenGL Error %s", __FUNCTION__, glewGetErrorString(e));
			return true;
		}
		return false;
	}

	void Reset()
	{
		e = glGetError();
	}

  private:
	GLenum e;
};
} // namespace Fluxions

#endif
