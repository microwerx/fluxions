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
#include "pch.h"
#include <iostream>
#include <fluxions_simple_renderer.hpp>

namespace Fluxions
{

	const char *SimpleProgram::AttribUniformInfo::GetNameOfType()
	{
		switch (type)
		{
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

	SimpleProgram::SimpleProgram()
	{
	}

	SimpleProgram::~SimpleProgram()
	{
		Delete();
	}

	void SimpleProgram::Create()
	{
		if (program)
			return;
		program = glCreateProgram();
		hflog.info("%s(): program %d created.", __FUNCTION__, program);
	}

	void SimpleProgram::Delete()
	{
		if (program)
		{
			hflog.info("%s(): program %d deleted.", __FUNCTION__, program);
			glDeleteProgram(program);
			program = 0;
		}
	}

	GLint SimpleProgram::GetAttribLocation(const char *name)
	{
		if (!linked)
			return -1;
		auto it = activeAttributes.find(name);
		if (it == activeAttributes.end())
			return -1;
		return it->second.index;
	}

	GLint SimpleProgram::GetUniformLocation(const char *name)
	{
		if (!linked)
			return -1;
		auto it = activeUniforms.find(name);
		if (it == activeUniforms.end())
			return -1;
		return it->second.index;
	}

	GLint SimpleProgram::GetUniformBlockIndex(const char *name)
	{
		if (!linked)
			return -1;
		return glGetUniformBlockIndex(program, name);
	}

	void SimpleProgram::Use()
	{
		if (!linked)
			Link();
		if (!linked)
			return;
		glUseProgram(program);
	}

	bool SimpleProgram::ApplyUniform(const std::string &uniformName, SimpleUniform uniform)
	{
		if (!linked)
			return false;
		GLint loc = GetUniformLocation(uniformName.c_str());
		if (loc >= 0)
		{
			//uniform.location = loc;
			uniform.SetProgramUniform(loc);
			return true;
		}
		else
		{
			// hflog.warning("%s(): Program %i tried to set invalid/unused uniform %s", __FUNCTION__, program, uniformName.c_str());
			return false;
		}
	}

	void SimpleProgram::ApplyUniforms(std::map<std::string, SimpleUniform> uniforms)
	{
		if (!linked)
			return;
		for (auto it = uniforms.begin(); it != uniforms.end(); ++it)
		{
			GLint loc = GetUniformLocation(it->first.c_str());
			if (loc >= 0)
			{
				it->second.SetProgramUniform(loc);
			}
			else
			{
				hflog.info("%s(): unable to set uniform %s (%d) for program %d.", __FUNCTION__, it->first.c_str(), loc, program);
			}
		}
	}

	void SimpleProgram::AttachShaders(std::shared_ptr<SimpleShader> &shaderPtr)
	{
		if (!shaderPtr)
			return;
		if (!shaderPtr->didCompile)
			return;
		shaders.push_back(shaderPtr);
		linked = false;
	}

	void SimpleProgram::BindAttribLocation(GLuint index, const char *name)
	{
		glBindAttribLocation(program, index, name);
		linked = false;
	}

	void LogGLEWError() {
		GLenum glerror = glGetError();
		if (glerror != 0)
		{
			hflog.errorfn(__FUNCTION__, "GL Error: %s", (const char *)glewGetErrorString(glerror));
		}
	}

	bool SimpleProgram::Link()
	{
		if (program == 0 || !glIsProgram(program))
		{
			hflog.errorfn(__FUNCTION__, "program %d does not exist", program);
			return false;
		}

		for (auto &shaderIt : shaders)
		{
			glAttachShader(program, shaderIt->shader);
		}

		GLint bufSize;

		try {
			hflog.infofn(__FUNCTION__, "Trying to link program %d", program);
			glLinkProgram(program);
			hflog.infofn(__FUNCTION__, "Program %d glLinkProgram() completed", program);
		}
		catch (...) {
			hflog.errorfn(__FUNCTION__, "Unknown error linking program %d", program);
		}

		linkStatus_ = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus_);
		linked = (linkStatus_ == GL_TRUE);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
		infoLog.resize(bufSize);
		if (bufSize > 0)
		{
			glGetProgramInfoLog(program, bufSize, NULL, &infoLog[0]);
			hflog.errorfn(__FUNCTION__, "Program link error:\n%s", infoLog.c_str());
		}

		validateStatus_ = 0;
		glValidateProgram(program);
		glGetProgramiv(program, GL_VALIDATE_STATUS, &validateStatus_);
		validated = (validateStatus_ == GL_TRUE);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
		validateLog.resize(bufSize);
		if (bufSize > 0)
		{
			glGetProgramInfoLog(program, 0, NULL, &validateLog[0]);
			hflog.errorfn(__FUNCTION__, "Program validation error:\n%s", validateLog.c_str());
		}

		activeAttributes.clear();
		activeUniforms.clear();
		if (linked)
		{
			// get list of active attributes/uniforms
			char buffer[4096];
			int length;
			GLint size;
			GLenum type;
			GLint maxUniformLength;
			GLint numUniforms;
			GLint maxAttribLength;
			GLint numAttribs;

			glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLength);
			glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
			glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribLength);
			glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttribs);

			for (int i = 0; i < numUniforms; i++)
			{
				glGetActiveUniform(program, i, maxUniformLength, &length, &size, &type, buffer);
				std::string name = buffer;
				activeUniforms[name].index = glGetUniformLocation(program, buffer);
				activeUniforms[name].size = size;
				activeUniforms[name].type = type;
				hflog.infofn(__FUNCTION__, "uniform (%02d) %20s %s", activeUniforms[name].index, activeUniforms[name].GetNameOfType(), name.c_str());
			}

			for (int i = 0; i < numAttribs; i++)
			{
				glGetActiveAttrib(program, i, maxAttribLength, &length, &size, &type, buffer);
				std::string name = buffer;
				activeAttributes[name].index = glGetAttribLocation(program, buffer);
				activeAttributes[name].size = size;
				activeAttributes[name].type = type;
				hflog.infofn(__FUNCTION__, "attrib  (%02d) %20s %s", activeAttributes[name].index, activeAttributes[name].GetNameOfType(), name.c_str());
			}

			hflog.infofn(__FUNCTION__, "program %d linked.", program);
		}

		return linked;
	}

	const std::string &SimpleProgram::GetInfoLog()
	{
		return infoLog;
	}

	void SimpleProgram::SetUniformBlock(const std::string &uniformBlockName, GLuint buffer, GLuint blockBindingIndex, GLintptr offset, GLsizei size)
	{
		GLuint blockIndex = glGetUniformBlockIndex(program, uniformBlockName.c_str());
		glUniformBlockBinding(program, blockIndex, blockBindingIndex);
		glBindBufferRange(GL_UNIFORM_BUFFER, blockBindingIndex, buffer, offset, size);
	}

	// SIMPLEBLOCKUNIFORMSYSTEM

	SimpleUniformBlockSystem::SimpleUniformBlockSystem()
	{
	}

	SimpleUniformBlockSystem::~SimpleUniformBlockSystem()
	{
		Delete();
	}

	void SimpleUniformBlockSystem::Create(GLsizei totalMemory, GLuint blockIndex)
	{
		if (uniformBlockBufferId != 0)
			Delete();
		buffer.resize(totalMemory);
		blockIndexId = blockIndex;
		glGenBuffers(1, &uniformBlockBufferId);
		hflog.info("%s(): uniform block %d created.", __FUNCTION__, uniformBlockBufferId);
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockBufferId);
		glBufferData(GL_UNIFORM_BUFFER, buffer.size(), buffer.data(), GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, blockIndexId, uniformBlockBufferId);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void SimpleUniformBlockSystem::Delete()
	{
		if (uniformBlockBufferId != 0)
		{
			hflog.info("%s(): uniform block %d deleted.", __FUNCTION__, uniformBlockBufferId);
			glDeleteBuffers(1, &uniformBlockBufferId);
			uniformBlockBufferId = 0;
			blockIndexId = 0;
		}
	}

	void SimpleUniformBlockSystem::Update()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockBufferId);
		glBufferData(GL_UNIFORM_BUFFER, buffer.size(), buffer.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void SimpleUniformBlockSystem::UpdateSubData(GLsizei offset, GLsizei size, GLbyte *data)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockBufferId);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
} // namespace Fluxions
