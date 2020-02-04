#ifndef FLUXIONS_RENDERER_SHADER_HPP
#define FLUXIONS_RENDERER_SHADER_HPP

#include <fluxions_renderer_object.hpp>

namespace Fluxions {
	struct RendererShader : public RendererObject {
		GLenum shaderType = 0;
		GLuint shader = 0;
		std::string source;
		std::string infoLog;
		bool didCompile = false;
		bool hadError = false;

		RendererShader();
		~RendererShader();

		void init(const std::string& name, RendererObject* pparent, GLenum shaderType);
		void kill() override;
		const char* type() const noexcept override;

		bool compileSource(const std::string& sourceCode);
		const char* status() const override;

	private:
		void create_shader(GLenum shaderType);
		void delete_shader();
	};

	using RendererShaderPtr = std::shared_ptr<RendererShader>;

	bool CompileShaderFromFile(RendererShaderPtr& shader, GLenum type, const std::string& filename);
} // namespace Fluxions

#endif
