#ifndef FLUXIONS_SIMPLE_SHADER_HPP
#define FLUXIONS_SIMPLE_SHADER_HPP

#include <string>
#include <fluxions_opengl.hpp>

namespace Fluxions
{
	struct SimpleShader
	{
		GLenum type = 0;
		GLuint shader = 0;
		std::string source;
		std::string infoLog;
		bool didCompile = false;
		bool hadError = false;

		SimpleShader() {}

		~SimpleShader() {
			Delete();
		}

		void Create(GLenum shaderType) {
			if (shader)
				Delete();
			type = shaderType;
			shader = glCreateShader(type);
			source = "";
			didCompile = false;
			hadError = false;
			HFLOGDEBUG("shader %d created.", shader);
		}

		void Delete() {
			if (shader != 0) {
				HFLOGDEBUG("shader %d deleted.", shader);
				glDeleteShader(shader);
				shader = 0;
			}
		}
	};

	std::shared_ptr<SimpleShader> CompileShaderFromFile(GLenum type, const std::string& filename);

} // namespace Fluxions

#endif