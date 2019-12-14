#ifndef FLUXIONS_SIMPLE_PROGRAM_HPP
#define FLUXIONS_SIMPLE_PROGRAM_HPP

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <fluxions_opengl.hpp>
#include <fluxions_simple_shader.hpp>
#include <fluxions_simple_uniform.hpp>

namespace Fluxions
{
	class SimpleProgram
	{
	private:
		GLuint program = 0;
		using SimpleShaderPtr = std::shared_ptr<SimpleShader>;
		std::vector<SimpleShaderPtr> shaders;

		GLint linkStatus_ = 0;
		bool linked = false;
		std::string infoLog;

		GLint validateStatus_ = 0;
		bool validated = false;
		std::string validateLog;
		//uniformBlocks;

	public:
		struct AttribUniformInfo
		{
			GLint size;
			GLenum type;
			GLint index;

			const char* GetNameOfType();
		};
		std::map<std::string, AttribUniformInfo> activeAttributes;
		std::map<std::string, AttribUniformInfo> activeUniforms;

		SimpleProgram();
		~SimpleProgram();

		void Create();
		void Delete();
		GLint GetAttribLocation(const char* name);
		GLint GetUniformLocation(const char* name);
		GLint GetUniformBlockIndex(const char* name);
		const std::string& GetInfoLog();
		GLuint GetProgram() { return program; }

		void SetUniformBlock(const std::string& uniformBlockName, GLuint buffer, GLuint blockBindingIndex, GLintptr offset, GLsizei size);

		bool IsLinked() { return linkStatus_ != 0; }
		void Use();
		void ApplyUniforms(std::map<std::string, SimpleUniform> uniforms);
		bool ApplyUniform(const std::string& uniformName, SimpleUniform uniform);

		void AttachShaders(std::shared_ptr<SimpleShader>& shaderPtr);
		void BindAttribLocation(GLuint index, const char* name);
		bool Link();
	};

	using SimpleProgramPtr = std::shared_ptr<SimpleProgram>;
} // namespace Fluxions

#endif