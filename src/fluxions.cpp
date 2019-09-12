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
#include "pch.hpp"
#include <fluxions.hpp>

namespace Fluxions
{
	bool debugging = false;
	const std::string BlankString("");
	bool __checkedGLInfo = false;
	std::string gl_version;
	std::string gl_renderer;
	std::string gl_vendor;

	std::shared_ptr<SimpleShader> CompileShaderFromFile(GLenum type, const std::string &filename)
	{
		const char *typeName = (type == GL_VERTEX_SHADER) ? "vertex" :
			(type == GL_FRAGMENT_SHADER) ? "fragment" :
			(type == GL_GEOMETRY_SHADER) ? "geometry" :
			"unknown";
		std::shared_ptr<SimpleShader> shader(new SimpleShader());
		FilePathInfo fpi(filename);
		if (!fpi.IsFile())
			return shader;
		HFLOGDEBUG("loading %s shader `%s'", typeName, fpi.fullfname.c_str());

		shader->Create(type);
		if (shader->shader == 0) {
			HFLOGERROR("%s shader could not be created!", typeName);
			return shader;
		}
		shader->source = ReadTextFile(filename);
		const GLchar *sourceString = shader->source.c_str();
		const GLint length = (int)shader->source.size();
		glShaderSource(shader->shader, 1, &sourceString, &length);
		glCompileShader(shader->shader);
		GLint ival = 0;
		glGetShaderiv(shader->shader, GL_COMPILE_STATUS, &ival);
		shader->didCompile = (ival == GL_TRUE);
		shader->hadError = (ival != GL_TRUE);
		glGetShaderiv(shader->shader, GL_INFO_LOG_LENGTH, &ival);
		shader->infoLog.resize(ival);
		GLchar *infoLog = &shader->infoLog[0];
		glGetShaderInfoLog(shader->shader, ival, NULL, infoLog);
		if (shader->hadError)
		{
			HFLOGERROR("shader %d compile error for %s\n%s", shader->shader, filename.c_str(), shader->infoLog.c_str());
		}
		if (shader->didCompile)
		{
			HFLOGDEBUG("shader %d compiled", shader->shader);
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

	std::map<std::string, std::string> MakeOptionsFromArgs(int argc, const char **argv)
	{
		std::map<std::string, std::string> argv_options;
		for (int i = 0; i < argc; i++)
		{
			std::string option = argv[i];
			HFLOGINFO("Processing '%s'", option.c_str());
			std::regex dashequals("(^-+|=)",
				std::regex_constants::ECMAScript);
			std::sregex_token_iterator it(option.begin(),
				option.end(),
				dashequals,
				-1);
			std::sregex_token_iterator end;
			size_t count = 0;

			std::string key = "";
			std::string value = "";
			for (; it != end; it++)
			{
				if (count == 1)
				{
					key = *it;
				}
				else if (count == 2)
				{
					value = *it;
				}
				std::string token = *it;
				HFLOGDEBUG("token %d '%s'", count, token.c_str());
				count++;
			}
			if (key.length() > 0)
			{
				argv_options.emplace(std::make_pair(key, value));
				HFLOGINFO("argv adding key '%s' = '%s'", key.c_str(), value.c_str());
			}
		}
		return argv_options;
	}

	void ReadGLInfo()
	{
		__checkedGLInfo = true;
		gl_version = (const char *)glGetString(GL_VERSION);
		gl_vendor = (const char *)glGetString(GL_VENDOR);
		gl_renderer = (const char *)glGetString(GL_RENDERER);

		HFLOGINFO("GL_RENDERER: %s", gl_renderer.c_str());
		HFLOGINFO("GL_VERSION:  %s", gl_version.c_str());
		HFLOGINFO("GL_VENDOR:   %s", gl_vendor.c_str());
	}

	const std::string &GetRenderer()
	{
		if (!__checkedGLInfo) ReadGLInfo();
		return gl_renderer;
	}

	const std::string &GetVendor()
	{
		if (!__checkedGLInfo) ReadGLInfo();
		return gl_vendor;
	}

	const std::string &GetVersion()
	{
		if (!__checkedGLInfo) ReadGLInfo();
		return gl_version;
	}

	void Init()
	{
		// Perform any necessary initialization steps here
		HFLOGINFO("Initializing Fluxions");
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			HFLOGERROR("glewInit() -> %s", glewGetErrorString(err));
		}
		ReadGLInfo();

		glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB, GL_NICEST);

#ifndef FLUXIONS_NO_SDL
		HFLOGINFO("Initializing SDL");
		SDL_Init(SDL_INIT_EVERYTHING);
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
#endif
	}

	void Kill()
	{

	}

	void YieldThread()
	{
#ifdef _WIN32
		INPUT input;
		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(0);
#elif _POSIX_VERSION
		usleep(0);
#endif
	}

} // namespace Fluxions
