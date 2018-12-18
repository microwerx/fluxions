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
#include "stdafx.h"
#include <fluxions.hpp>

#ifdef WIN32
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_image.lib")
//#pragma comment(lib, "glew32s.lib")
#endif

namespace Fluxions
{
bool debugging = false;
const std::string BlankString("");

std::shared_ptr<SimpleShader> CompileShaderFromFile(GLenum type, const std::string &filename)
{
	std::shared_ptr<SimpleShader> shader(new SimpleShader());
	FilePathInfo fpi(filename);
	hflog.infofn(__FUNCTION__, "loading shader `%s'", fpi.fullfname.c_str());
	if (!fpi.IsFile())
		return shader;

	shader->Create(type);
	shader->source = ReadTextFile(filename);
	const GLchar *sourceString = shader->source.c_str();
	const GLint length = (int)shader->source.size();
	glShaderSource(shader->shader, 1, &sourceString, &length);
	glCompileShader(shader->shader);
	GLint ival = 0;
	glGetShaderiv(shader->shader, GL_COMPILE_STATUS, &ival);
	if (ival == GL_FALSE)
	{
		shader->didCompile = false;
		shader->hadError = true;
	}
	else
	{
		shader->didCompile = true;
		shader->hadError = false;
	}
	glGetShaderiv(shader->shader, GL_INFO_LOG_LENGTH, &ival);
	shader->infoLog.resize(ival);
	GLchar *infoLog = &shader->infoLog[0];
	glGetShaderInfoLog(shader->shader, ival, NULL, infoLog);
	if (shader->hadError)
	{
		hflog.errorfn(__FUNCTION__, "error compiling shader %s\n%s", filename.c_str(), shader->infoLog.c_str());
	}

	return shader;
}

std::string FindFileIfExists(const std::string &filename, const std::vector<std::string> &pathsToTry)
{
	std::string output;

	// Is there a file name to test?
	FilePathInfo fpi(filename);
	if (fpi.fullfname.empty())
		return output;

	if (TestIfFileExists(filename))
	{
		output = filename;
	}
	else
	{
		for (auto testPathIt : pathsToTry)
		{
			std::string testPath = testPathIt + fpi.fullfname;
			if (TestIfFileExists(testPath))
			{
				output = testPath;
				break;
			}
		}
	}

	return output;
}
} // namespace Fluxions
