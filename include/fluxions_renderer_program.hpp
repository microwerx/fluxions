#ifndef FLUXIONS_SIMPLE_PROGRAM_HPP
#define FLUXIONS_SIMPLE_PROGRAM_HPP

#include <fluxions_renderer_object.hpp>
#include <fluxions_renderer_shader.hpp>
#include <fluxions_renderer_uniform.hpp>

namespace Fluxions
{
	class RendererProgram : RendererObject
	{
	private:
		GLuint program = 0;		
		std::vector<RendererShaderPtr> shaders;

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

		RendererProgram();
		~RendererProgram();

		void init(const std::string& name) override;
		void kill() override;

		bool isLinked() const { return linkStatus_ != 0; }
		void use();
		void applyUniforms(const RendererUniformMap& uniforms) const;
		bool applyUniform(const std::string& uniformName, RendererUniform uniform) const;

		void attachShaders(RendererShaderPtr& shaderPtr);
		void bindAttribLocation(GLuint index, const char* name);
		bool link();

		GLint getAttribLocation(const std::string& name) const;
		GLint getUniformLocation(const std::string& name) const;
		//GLint getUniformBlockIndex(const std::string& name) const;
		const std::string& getInfoLog() const;
		GLuint getProgram() const;
	private:
		void create_program();
		void delete_program();

		void setUniformBlock(const std::string& uniformBlockName, GLuint buffer, GLuint blockBindingIndex, GLintptr offset, GLsizei size);
	};

	using RendererProgramPtr = std::shared_ptr<RendererProgram>;
} // namespace Fluxions

#endif