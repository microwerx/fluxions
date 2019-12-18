#ifndef FLUXIONS_SIMPLE_SHADER_HPP
#define FLUXIONS_SIMPLE_SHADER_HPP

#include <fluxions_renderer_object.hpp>

namespace Fluxions
{
	struct RendererShader : public RendererObject
	{
		GLenum type = 0;
		GLuint shader = 0;
		std::string source;
		std::string infoLog;
		bool didCompile = false;
		bool hadError = false;

		RendererShader();
		~RendererShader();

		void init(const std::string& name, GLenum shaderType);
		void kill() override;
		bool compileSource(const std::string& sourceCode);
	private:
		void create_shader(GLenum shaderType);
		void delete_shader();
	};

	using RendererShaderPtr = std::shared_ptr<RendererShader>;

	RendererShaderPtr CompileShaderFromFile(GLenum type, const std::string& filename);
} // namespace Fluxions

#endif