#include "fluxions_renderer_pch.hpp"
#include <fluxions_renderer_base.hpp>
#include <fluxions_renderer_program.hpp>

namespace Fluxions {
	void RendererProgram::init(const std::string& name,
							   RendererObject* pparent) {
		RendererObject::init(name, pparent);
		create_program();
	}

	void RendererProgram::kill() {
		delete_program();
		RendererObject::kill();
	}

	const char* RendererProgram::type() const {
		return "RendererProgram";
	}

	const char* RendererProgram::AttribUniformInfo::GetNameOfType() const {
		switch (type) {
		case GL_INT:
			return "GL_INT";
		case GL_INT_VEC2:
			return "GL_INT_VEC2";
		case GL_INT_VEC3:
			return "GL_INT_VEC3";
		case GL_INT_VEC4:
			return "GL_INT_VEC4";
		case GL_UNSIGNED_INT:
			return "GL_UNSIGNED_INT";
		case GL_UNSIGNED_INT_VEC2:
			return "GL_UNSIGNED_INT_VEC2";
		case GL_UNSIGNED_INT_VEC3:
			return "GL_UNSIGNED_INT_VEC3";
		case GL_UNSIGNED_INT_VEC4:
			return "GL_UNSIGNED_INT_VEC4";
		case GL_BOOL:
			return "GL_BOOL";
		case GL_BOOL_VEC2:
			return "GL_BOOL_VEC2";
		case GL_BOOL_VEC3:
			return "GL_BOOL_VEC3";
		case GL_BOOL_VEC4:
			return "GL_BOOL_VEC4";
		case GL_FLOAT:
			return "GL_FLOAT";
		case GL_FLOAT_VEC2:
			return "GL_FLOAT_VEC2";
		case GL_FLOAT_VEC3:
			return "GL_FLOAT_VEC3";
		case GL_FLOAT_VEC4:
			return "GL_FLOAT_VEC4";
		case GL_FLOAT_MAT2:
			return "GL_FLOAT_MAT2";
		case GL_FLOAT_MAT3:
			return "GL_FLOAT_MAT3";
		case GL_FLOAT_MAT4:
			return "GL_FLOAT_MAT4";
		case GL_FLOAT_MAT2x3:
			return "GL_FLOAT_MAT2x3";
		case GL_FLOAT_MAT2x4:
			return "GL_FLOAT_MAT2x4";
		case GL_FLOAT_MAT3x2:
			return "GL_FLOAT_MAT3x2";
		case GL_FLOAT_MAT3x4:
			return "GL_FLOAT_MAT3x4";
		case GL_FLOAT_MAT4x2:
			return "GL_FLOAT_MAT4x2";
		case GL_FLOAT_MAT4x3:
			return "GL_FLOAT_MAT4x3";
		case GL_DOUBLE:
			return "GL_DOUBLE";
		case GL_DOUBLE_VEC2:
			return "GL_DOUBLE_VEC2";
		case GL_DOUBLE_VEC3:
			return "GL_DOUBLE_VEC3";
		case GL_DOUBLE_VEC4:
			return "GL_DOUBLE_VEC4";
		case GL_DOUBLE_MAT2:
			return "GL_DOUBLE_MAT2";
		case GL_DOUBLE_MAT3:
			return "GL_DOUBLE_MAT3";
		case GL_DOUBLE_MAT4:
			return "GL_DOUBLE_MAT4";
		case GL_DOUBLE_MAT2x3:
			return "GL_DOUBLE_MAT2x3";
		case GL_DOUBLE_MAT2x4:
			return "GL_DOUBLE_MAT2x4";
		case GL_DOUBLE_MAT3x2:
			return "GL_DOUBLE_MAT3x2";
		case GL_DOUBLE_MAT3x4:
			return "GL_DOUBLE_MAT3x4";
		case GL_DOUBLE_MAT4x2:
			return "GL_DOUBLE_MAT4x2";
		case GL_DOUBLE_MAT4x3:
			return "GL_DOUBLE_MAT4x3";
		case GL_SAMPLER_1D:
			return "GL_SAMPLER_1D";
		case GL_SAMPLER_2D:
			return "GL_SAMPLER_2D";
		case GL_SAMPLER_3D:
			return "GL_SAMPLER_3D";
		case GL_SAMPLER_CUBE:
			return "GL_SAMPLER_CUBE";
		case GL_SAMPLER_CUBE_SHADOW:
			return "GL_SAMPLER_CUBE_SHADOW";
		case GL_SAMPLER_1D_SHADOW:
			return "GL_SAMPLER_1D_SHADOW";
		case GL_SAMPLER_2D_SHADOW:
			return "GL_SAMPLER_2D_SHADOW";
		case GL_SAMPLER_1D_ARRAY:
			return "GL_SAMPLER_1D_ARRAY";
		case GL_SAMPLER_2D_ARRAY:
			return "GL_SAMPLER_2D_ARRAY";
		case GL_SAMPLER_1D_ARRAY_SHADOW:
			return "GL_SAMPLER_1D_ARRAY_SHADOW";
		case GL_SAMPLER_2D_ARRAY_SHADOW:
			return "GL_SAMPLER_2D_ARRAY_SHADOW";
		case GL_SAMPLER_2D_MULTISAMPLE:
			return "GL_SAMPLER_2D_MULTISAMPLE";
		case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
			return "GL_SAMPLER_2D_MULTISAMPLE_ARRAY";
		case GL_SAMPLER_BUFFER:
			return "GL_SAMPLER_BUFFER";
		case GL_SAMPLER_2D_RECT:
			return "GL_SAMPLER_2D_RECT";
		case GL_SAMPLER_2D_RECT_SHADOW:
			return "GL_SAMPLER_2D_RECT_SHADOW";
		}
		return "UNKNOWN";
	}

	RendererProgram::RendererProgram() {}

	RendererProgram::~RendererProgram() {}

	void RendererProgram::create_program() {
		if (program)
			return;
		FxCreateProgram(&program);
	}

	void RendererProgram::delete_program() {
		if (program) {
			FxDeleteProgram(&program);
			for (RendererShaderPtr& s : attachedShaders) {
				s.reset();
			}
			attachedShaders.clear();
		}
	}

	GLint RendererProgram::getAttribLocation(const std::string& name) const {
		if (!linked)
			return -1;
		auto it = activeAttributes.find(name);
		if (it == activeAttributes.end())
			return -1;
		return it->second.index;
	}

	GLint RendererProgram::getUniformLocation(const std::string& name) const {
		if (!linked)
			return -1;
		auto it = activeUniforms.find(name);
		if (it == activeUniforms.end())
			return -1;
		return it->second.index;
	}

	//GLint RendererProgram::getUniformBlockIndex(const std::string& name) {
	//	if (!linked)
	//		return -1;
	//	return glGetUniformBlockIndex(program, name.c_str());
	//}

	void RendererProgram::use() {
		if (!linked) link();
		if (!linked) return;
		glUseProgram(program);
	}

	bool RendererProgram::applyUniform(const std::string& uniformName, RendererUniform uniform) const {
		if (!linked) return false;
		GLint loc = getUniformLocation(uniformName.c_str());
		if (loc >= 0) {
			uniform.setProgramUniform(loc);
			return true;
		}
		else {
			// HFLOGWARNING("Program %i tried to set invalid/unused uniform %s", program, uniformName.c_str());
			return false;
		}
	}

	bool RendererProgram::applyUniform(int loc, RendererUniform uniform) const {
		if (!linked) return false;
		if (loc >= 0) {
			uniform.setProgramUniform(loc);
			return true;
		}
		return false;
	}

	void RendererProgram::applyUniforms(const RendererUniformMap& uniforms) const {
		if (!linked)
			return;
		//for (auto it = uniforms.begin(); it != uniforms.end(); ++it) {
		for (const auto& [uniformName, uniform] : uniforms) {
			GLint loc = getUniformLocation(uniformName);
			if (loc >= 0) {
				uniform.setProgramUniform(loc);
			}
			else {
				HFLOGINFO("unable to set uniform %s (%d) for program %d.",
						  uniformName.c_str(), loc, program);
			}
		}
	}

	bool RendererProgram::uniform1i(const std::string& uniformName, int val) {
		int loc = getUniformLocation(uniformName);
		if (loc < 0) return false;
		glUniform1i(loc, val);
		return true;
	}

	bool RendererProgram::uniform1f(const std::string& uniformName, float val) {
		int loc = getUniformLocation(uniformName);
		if (loc < 0) return false;
		glUniform1f(loc, val);
		return true;
	}

	bool RendererProgram::uniform2f(const std::string& uniformName, const float* val) {
		int loc = getUniformLocation(uniformName);
		if (loc < 0) return false;
		glUniform2fv(loc, 1, val);
		return true;
	}

	bool RendererProgram::uniform3f(const std::string& uniformName, const float* val) {
		int loc = getUniformLocation(uniformName);
		if (loc < 0) return false;
		glUniform3fv(loc, 1, val);
		return true;
	}

	bool RendererProgram::uniform4f(const std::string& uniformName, const float* val) {
		int loc = getUniformLocation(uniformName);
		if (loc < 0) return false;
		glUniform4fv(loc, 1, val);
		return true;
	}

	bool RendererProgram::uniformMatrix4f(const std::string& uniformName, const Matrix4f& val) {
		int loc = getUniformLocation(uniformName);
		if (loc < 0) return false;
		glUniformMatrix4fv(loc, 1, 0, val.const_ptr());
		return true;
	}

	void RendererProgram::loadShader(const std::string& path, GLenum type) {
		if (path.empty()) return;
		FilePathInfo fpi(path);
		RendererShaderPtr shader = std::make_shared<RendererShader>();
		shader->init(fpi.filename(), this, type);
		if (CompileShaderFromFile(shader, type, path))
			attachShaders(shader);
		else HFLOGERROR("shader '%s' compile failed", path.c_str());
	}

	void RendererProgram::detachShaders() {
		for (auto& shader : attachedShaders) {
			glDetachShader(program, shader->shader);
		}
		attachedShaders.clear();
		usable_ = linked = false;
	}

	void RendererProgram::loadShaders() {
		for (auto& [k, v] : shaderpaths) {
			loadShader(v.first, v.second);
		}
		//loadShader(vertshaderpath, GL_VERTEX_SHADER);
		//loadShader(fragshaderpath, GL_FRAGMENT_SHADER);
		//loadShader(geomshaderpath, GL_GEOMETRY_SHADER);
	}

	void RendererProgram::attachShaders(RendererShaderPtr& shaderPtr) {
		if (!shaderPtr)
			return;
		if (!shaderPtr->didCompile)
			return;
		shaderPtr->setParent(this);
		attachedShaders.push_back(shaderPtr);
		usable_ = linked = false;
	}

	void RendererProgram::bindAttribLocation(GLuint index, const char* name) {
		glBindAttribLocation(program, index, name);
		usable_ = linked = false;
	}

	void LogGLEWError() {
		GLenum glerror = glGetError();
		if (glerror != 0) {
			HFLOGERROR("GL Error: %s", (const char*)glewGetErrorString(glerror));
		}
	}

	bool RendererProgram::link() {
		if (program == 0 || !glIsProgram(program)) {
			HFLOGERROR("program %d does not exist", program);
			return false;
		}

		for (auto& shaderIt : attachedShaders) {
			glAttachShader(program, shaderIt->shader);
		}

		for (auto& ra : requestedVertexAttribs) {
			bindAttribLocation(ra.index, ra.name.c_str());
		}

		// bind shader blocks indices to binding points
		GLint bufSize;

		try {
			HFLOGINFO("Trying to link program %d", program);
			glLinkProgram(program);
			HFLOGINFO("Program %d glLinkProgram() completed", program);
		}
		catch (...) {
			HFLOGERROR("Unknown error linking program %d", program);
		}

		linkStatus_ = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus_);
		usable_ = linked = (linkStatus_ == GL_TRUE);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
		infoLog.resize(bufSize);
		if (bufSize > 0) {
			glGetProgramInfoLog(program, bufSize, NULL, &infoLog[0]);
			HFLOGERROR("Program link error:\n%s", infoLog.c_str());
		}

		validateStatus_ = 0;
		glValidateProgram(program);
		glGetProgramiv(program, GL_VALIDATE_STATUS, &validateStatus_);
		validated = (validateStatus_ == GL_TRUE);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
		validateLog.resize(bufSize);
		if (bufSize > 0) {
			glGetProgramInfoLog(program, 0, NULL, &validateLog[0]);
			HFLOGERROR("Program validation error:\n%s", validateLog.c_str());
		}

		RendererUniformBlock::SetProgramBindingPoints(program);

		activeAttributes.clear();
		activeUniforms.clear();
		activeUniformBlocks.clear();
		if (linked) {
			// get list of active attributes/uniforms
			char buffer[4096];
			int length;
			GLint size;
			GLenum type;
			GLint maxUniformLength;
			GLint numUniforms;
			GLint maxAttribLength;
			GLint numAttribs;
			GLint maxUniformBlockNameLength;
			GLint numUniformBlocks;

			glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLength);
			glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
			glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribLength);
			glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttribs);
			glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxUniformBlockNameLength);
			glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);

			for (int i = 0; i < numUniforms; i++) {
				glGetActiveUniform(program, i, maxUniformLength, &length, &size, &type, buffer);
				std::string name = buffer;
				auto& object = activeUniforms[name];
				object.index = glGetUniformLocation(program, buffer);
				object.size = size;
				object.type = type;
				HFLOGSTR(object.desc, "uniform (%02d) %20s %s", object.index, object.GetNameOfType(), name.c_str());
				if (object.index < 0) continue;
				HFLOGINFO(object.desc.c_str());
			}

			for (int i = 0; i < numAttribs; i++) {
				glGetActiveAttrib(program, i, maxAttribLength, &length, &size, &type, buffer);
				std::string name = buffer;
				auto& object = activeAttributes[name];
				object.index = glGetAttribLocation(program, buffer);
				object.size = size;
				object.type = type;
				HFLOGSTR(object.desc, "attrib  (%02d) %20s %s", object.index, object.GetNameOfType(), name.c_str());
				HFLOGINFO(object.desc.c_str());
			}

			for (int i = 0; i < numUniformBlocks; i++) {
				glGetActiveUniformBlockName(program, i, maxUniformBlockNameLength, &length, buffer);
				std::string name = buffer;
				auto& object = activeUniformBlocks[name];
				object.index = glGetUniformBlockIndex(program, buffer);
				glGetActiveUniformBlockiv(program, object.index, GL_UNIFORM_BLOCK_DATA_SIZE, &object.size);
				glGetActiveUniformBlockiv(program, object.index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &object.ubUniforms);
				object.ubUniformIndices.resize(object.ubUniforms, 0);
				glGetActiveUniformBlockiv(program, object.index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &object.ubUniformIndices[0]);

				object.type = 0;
				GLint referenced;
				glGetActiveUniformBlockiv(program, object.index, GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER, &referenced);
				object.type |= (referenced ? 1 : 0) << 0;
				glGetActiveUniformBlockiv(program, object.index, GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER, &referenced);
				object.type |= (referenced ? 1 : 0) << 1;
				//glGetActiveUniformBlockiv(program, object.index, GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER, &referenced);
				//object.type |= (referenced ? 1 : 0) << 2;
				//glGetActiveUniformBlockiv(program, object.index, GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER, &referenced);
				//object.type |= (referenced ? 1 : 0) << 3;
				//glGetActiveUniformBlockiv(program, object.index, GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER, &referenced);
				//object.type |= (referenced ? 1 : 0) << 4;
				//glGetActiveUniformBlockiv(program, object.index, GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER, &referenced);
				//object.type |= (referenced ? 1 : 0) << 5;
				//const char* referencedDesc = (object.type == 1) ? "VS  " :
				//	(object.type == 2) ? "  FS" :
				//	(object.type == 3) ? "VSFS" : "    ";
				HFLOGSTR(object.desc, "ublock  (%02i) x%3i ufrms%4i bytes %s",
						 object.index,
						 object.ubUniforms,
						 object.size,
						 name.c_str());
				HFLOGINFO(object.desc.c_str());
				for (auto& id : object.ubUniformIndices) {
					//for (const auto& [k, o] : activeUniforms) {
					//	if (o.index == id) {
					//		HFLOGINFO("%2i %s", id, k.c_str());
					//		break;
					//	}
					//}
					glGetActiveUniform(program, id, maxUniformLength, &length, &size, &object.type, buffer);
					if (strstr(buffer, "[") && !strstr(buffer, "[0]"))
						continue;
					HFLOGINFO("uniform (%02d) %20s %s", id, object.GetNameOfType(), buffer);
				}
			}

			HFLOGINFO("program %d linked.", program);
		}

		return linked;
	}

	const std::string& RendererProgram::getInfoLog() const {
		return infoLog;
	}

	GLuint RendererProgram::getProgram() const {
		return program;
	}

	void RendererProgram::setUniformBlock(const std::string& uniformBlockName, GLuint buffer, GLuint blockBindingIndex, GLintptr offset, GLsizei size) {
		GLuint blockIndex = glGetUniformBlockIndex(program, uniformBlockName.c_str());
		glUniformBlockBinding(program, blockIndex, blockBindingIndex);
		glBindBufferRange(GL_UNIFORM_BUFFER, blockBindingIndex, buffer, offset, size);
	}
} // namespace Fluxions
