#ifndef FLUXIONS_RENDERER_PROGRAM_HPP
#define FLUXIONS_RENDERER_PROGRAM_HPP

#include <fluxions_renderer_object.hpp>
#include <fluxions_renderer_shader.hpp>
#include <fluxions_renderer_uniform.hpp>
#include <fluxions_renderer_uniform_blocks.hpp>

namespace Fluxions {
	class RendererProgram : public RendererObject {
	private:
		GLuint program = 0;

		GLint linkStatus_ = 0;
		bool linked = false;
		std::string infoLog;

		GLint validateStatus_ = 0;
		bool validated = false;
		std::string validateLog;
		//uniformBlocks;

	public:
		struct RequestedVertexAttrib {
			int index{ 0 };
			std::string name;

			RequestedVertexAttrib() {}
			RequestedVertexAttrib(int _index, const std::string& _name) : index(_index), name(_name) {}
		};
		std::vector<RequestedVertexAttrib> requestedVertexAttribs;

		struct AttribUniformInfo {
			GLint size{ 0 };
			GLenum type{ 0 };
			GLint index{ 0 };
			GLint ubUniforms{ 0 };
			std::vector<int> ubUniformIndices;
			std::string desc;

			const char* GetNameOfType() const;
		};
		std::vector<RendererShaderPtr> attachedShaders;
		std::map<std::string, AttribUniformInfo> activeAttributes;
		std::map<std::string, AttribUniformInfo> activeUniforms;
		std::map<std::string, AttribUniformInfo> activeUniformBlocks;

		//std::string vertshaderpath;
		//std::string fragshaderpath;
		//std::string geomshaderpath;
		std::map<std::string, std::pair<std::string, GLenum>> shaderpaths;

		RendererProgram();
		~RendererProgram();

		void init(const std::string& name, RendererObject* pparent = nullptr) override;
		void kill() override;
		const char* type() const override;

		bool isLinked() const { return linkStatus_ != 0; }
		void use();
		void applyUniforms(const RendererUniformMap& uniforms) const;
		bool applyUniform(const std::string& uniformName, RendererUniform uniform) const;
		bool applyUniform(int loc, RendererUniform uniform) const;
		bool uniform1i(const std::string& uniformName, int val);
		bool uniform1f(const std::string& uniformName, float val);
		bool uniform2f(const std::string& uniformName, const float* val);
		bool uniform3f(const std::string& uniformName, const float* val);
		bool uniform4f(const std::string& uniformName, const float* val);
		bool uniformMatrix4f(const std::string& uniformName, const Matrix4f& val);

		void detachShaders();
		void loadShaders();
		void attachShaders(RendererShaderPtr& shaderPtr);
		void bindAttribLocation(GLuint index, const char* name);
		bool link();

		GLint getAttribLocation(const std::string& name) const;
		GLint getUniformLocation(const std::string& name) const;
		//GLint getUniformBlockIndex(const std::string& name) const;
		const std::string& getInfoLog() const;
		GLuint getProgram() const;
	private:
		void loadShader(const std::string& path, GLenum type);

		void create_program();
		void delete_program();

		void setUniformBlock(const std::string& uniformBlockName, GLuint buffer, GLuint blockBindingIndex, GLintptr offset, GLsizei size);
	};

	using RendererProgramPtr = std::shared_ptr<RendererProgram>;
} // namespace Fluxions

#endif
