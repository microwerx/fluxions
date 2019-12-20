#include "pch.hpp"
#include <fluxions_renderer_shader.hpp>

namespace Fluxions {
	RendererShaderPtr CompileShaderFromFile(RendererShaderPtr& shader, GLenum type, const std::string& filename) {
		const char* typeName =
			(type == GL_VERTEX_SHADER) ? "vertex" :
			(type == GL_FRAGMENT_SHADER) ? "fragment" :
			(type == GL_GEOMETRY_SHADER) ? "geometry" :
			"unknown";

		FilePathInfo fpi(filename);
		if (!fpi.IsFile()) {
			HFLOGERROR("file '%s' does not exist", filename.c_str());
			return shader;
		}
		HFLOGDEBUG("loading %s shader `%s'", typeName, fpi.fullfname.c_str());

		if (shader->shader == 0) {
			HFLOGERROR("%s shader could not be created!", typeName);
			return shader;
		}
		shader->compileSource(ReadTextFile(filename));
		
		if (shader->hadError) {
			HFLOGERROR("shader %d compile error for %s\n%s", shader->shader, fpi.fullfname.c_str(), shader->infoLog.c_str());
		}
		if (shader->didCompile) {
			HFLOGDEBUG("shader %d compiled", shader->shader);
		}

		return shader;
	}

	RendererShader::RendererShader() {}

	RendererShader::~RendererShader() {}

	void RendererShader::init(const std::string& name, RendererObject* pparent, GLenum shaderType) {
		RendererObject::init(name, pparent);
		create_shader(shaderType);
	}

	void RendererShader::kill() {
		RendererObject::kill();
		delete_shader();
	}

	bool RendererShader::compileSource(const std::string& sourceCode) {
		source = sourceCode;
		const GLchar* sourceString = source.c_str();
		const GLint length = (int)source.size();
		glShaderSource(shader, 1, &sourceString, &length);
		glCompileShader(shader);
		GLint ival = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &ival);
		didCompile = (ival == GL_TRUE);
		hadError = (ival != GL_TRUE);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &ival);
		infoLog.resize(ival);
		glGetShaderInfoLog(shader, ival, NULL, (GLchar*)&infoLog[0]);
		return didCompile;
	}

	void RendererShader::create_shader(GLenum shaderType) {
		if (shader)
			delete_shader();
		type = shaderType;
		shader = glCreateShader(type);
		source = "";
		didCompile = false;
		hadError = false;
		HFLOGDEBUG("shader %d created.", shader);
	}

	void RendererShader::delete_shader() {
		if (shader != 0) {
			HFLOGDEBUG("shader %d deleted.", shader);
			glDeleteShader(shader);
			shader = 0;
		}
	}
}