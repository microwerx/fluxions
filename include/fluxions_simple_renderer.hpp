// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
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
#ifndef FLUXIONS_SIMPLE_RENDERER_HPP
#define FLUXIONS_SIMPLE_RENDERER_HPP

#include <vector>
#include <iterator>
#include <memory>
#include <memory.h>
#include <map>
#include <algorithm>
#include <hatchetfish.hpp>
#include <fluxions_gte.hpp>
#include <fluxions_opengl.hpp>
#include <fluxions_simple_materials.hpp>

namespace Fluxions
{
using namespace std;

class SimpleUniform
{
  public:
	SimpleUniform() { ChangeType(GL_INT, 1, GL_FALSE, NULL); }
	~SimpleUniform() { buffer.clear(); }

	SimpleUniform(GLint intValue) { ChangeType(GL_INT, 1, GL_FALSE, (GLubyte *)&intValue); }
	SimpleUniform(GLuint uintValue) { ChangeType(GL_UNSIGNED_INT, 1, GL_FALSE, (GLubyte *)&uintValue); }
	SimpleUniform(GLfloat floatValue) { ChangeType(GL_FLOAT, 1, GL_FALSE, (GLubyte *)&floatValue); }
	SimpleUniform(GLdouble doubleValue) { ChangeType(GL_DOUBLE, 1, GL_FALSE, (GLubyte *)&doubleValue); }
	SimpleUniform(GLsizei count, GLfloat *v) { ChangeType(GL_FLOAT, count, GL_FALSE, (GLubyte *)v); }
	SimpleUniform(GLsizei count, GLdouble *v) { ChangeType(GL_DOUBLE, count, GL_FALSE, (GLubyte *)v); }
	SimpleUniform(GLenum newType, GLint newCount, GLboolean newTranspose, const GLubyte *newData) { ChangeType(newType, newCount, newTranspose, newData); }
	SimpleUniform(GLenum newType, GLint newCount, GLboolean newTranspose, const GLint *newData) { ChangeType(newType, newCount, newTranspose, reinterpret_cast<const GLubyte *>(newData)); }
	SimpleUniform(GLenum newType, GLint newCount, GLboolean newTranspose, const GLuint *newData) { ChangeType(newType, newCount, newTranspose, reinterpret_cast<const GLubyte *>(newData)); }
	SimpleUniform(GLenum newType, GLint newCount, GLboolean newTranspose, const GLfloat *newData) { ChangeType(newType, newCount, newTranspose, reinterpret_cast<const GLubyte *>(newData)); }

	GLint location = -1;
	GLenum type;
	GLenum baseType;
	GLsizei count;
	GLsizei sizeOfType;
	GLsizei sizeInBytes;
	GLboolean transpose;
	GLint components = 1;

	vector<GLubyte> buffer;

	inline const SimpleUniform &operator=(const SimpleUniform &uniform)
	{
		// name = uniform.name;
		ChangeType(uniform.type, uniform.count, uniform.transpose, uniform.buffer.data());
		return *this;
	}

	inline SimpleUniform &operator=(const TMatrix2<GLfloat> &m)
	{
		UniformMatrix2fv(m.m());
		return *this;
	}
	inline SimpleUniform &operator=(const TMatrix3<GLfloat> &m)
	{
		UniformMatrix3fv(m.m());
		return *this;
	}
	inline SimpleUniform &operator=(const TMatrix4<GLfloat> &m)
	{
		UniformMatrix4fv(m.m());
		return *this;
	}
	//inline SimpleUniform & operator=(const TMatrix2x3<GLfloat> &m) { UniformMatrix2x3fv(m.m); return *this; }
	//inline SimpleUniform & operator=(const TMatrix2x4<GLfloat> &m) { UniformMatrix2x3fv(m.m); return *this; }
	//inline SimpleUniform & operator=(const TMatrix3x2<GLfloat> &m) { UniformMatrix2x3fv(m.m); return *this; }
	//inline SimpleUniform & operator=(const TMatrix3x4<GLfloat> &m) { UniformMatrix2x3fv(m.m); return *this; }
	//inline SimpleUniform & operator=(const TMatrix4x2<GLfloat> &m) { UniformMatrix2x3fv(m.m); return *this; }
	//inline SimpleUniform & operator=(const TMatrix4x3<GLfloat> &m) { UniformMatrix2x3fv(m.m); return *this; }

	inline SimpleUniform &operator=(const TMatrix2<double> &m) { return *this = (TMatrix2<float>)(m); }
	inline SimpleUniform &operator=(const TMatrix3<double> &m) { return *this = (TMatrix3<float>)(m); }
	inline SimpleUniform &operator=(const TMatrix4<double> &m) { return *this = (TMatrix4<float>)(m); }
	//inline SimpleUniform & operator=(const TMatrix2x3<double> &m) { return *this = (TMatrix2x3<float>)(m); }
	//inline SimpleUniform & operator=(const TMatrix2x4<double> &m) { return *this = (TMatrix2x4<float>)(m); }
	//inline SimpleUniform & operator=(const TMatrix3x2<double> &m) { return *this = (TMatrix3x2<float>)(m); }
	//inline SimpleUniform & operator=(const TMatrix3x4<double> &m) { return *this = (TMatrix3x4<float>)(m); }
	//inline SimpleUniform & operator=(const TMatrix4x2<double> &m) { return *this = (TMatrix4x2<float>)(m); }
	//inline SimpleUniform & operator=(const TMatrix4x3<double> &m) { return *this = (TMatrix4x3<float>)(m); }

	inline SimpleUniform &operator=(const TVector2<GLfloat> &v)
	{
		Uniform2fv(v.v());
		return *this;
	}
	inline SimpleUniform &operator=(const TVector3<GLfloat> &v)
	{
		Uniform3fv(v.v());
		return *this;
	}
	inline SimpleUniform &operator=(const TVector4<GLfloat> &v)
	{
		Uniform4fv(v.v());
		return *this;
	}

	inline SimpleUniform &operator=(const TVector2<double> &v) { return *this = (TVector2<float>)(v); }
	inline SimpleUniform &operator=(const TVector3<double> &v) { return *this = (TVector3<float>)(v); }
	inline SimpleUniform &operator=(const TVector4<double> &v) { return *this = (TVector4<float>)(v); }

	inline SimpleUniform &operator=(const TVector2<GLint> &v)
	{
		Uniform2iv(v.v());
		return *this;
	}
	inline SimpleUniform &operator=(const TVector3<GLint> &v)
	{
		Uniform3iv(v.v());
		return *this;
	}
	inline SimpleUniform &operator=(const TVector4<GLint> &v)
	{
		Uniform4iv(v.v());
		return *this;
	}

	inline SimpleUniform &operator=(const TVector2<GLuint> &v)
	{
		Uniform2uiv(v.v());
		return *this;
	}
	inline SimpleUniform &operator=(const TVector3<GLuint> &v)
	{
		Uniform3uiv(v.v());
		return *this;
	}
	inline SimpleUniform &operator=(const TVector4<GLuint> &v)
	{
		Uniform4uiv(v.v());
		return *this;
	}

	SimpleUniform(const TMatrix2<GLfloat> &m) { UniformMatrix2fv(m.m()); }
	SimpleUniform(const TMatrix3<GLfloat> &m) { UniformMatrix3fv(m.m()); }
	SimpleUniform(const TMatrix4<GLfloat> &m) { UniformMatrix4fv(m.m()); }
	//SimpleUniform(const TMatrix2x3<GLfloat> & m) { UniformMatrix2x3fv(m.m()); }
	//SimpleUniform(const TMatrix2x4<GLfloat> & m) { UniformMatrix2x4fv(m.m()); }
	//SimpleUniform(const TMatrix3x2<GLfloat> & m) { UniformMatrix3x2fv(m.m()); }
	//SimpleUniform(const TMatrix3x4<GLfloat> & m) { UniformMatrix3x4fv(m.m()); }
	//SimpleUniform(const TMatrix4x2<GLfloat> & m) { UniformMatrix4x2fv(m.m()); }
	//SimpleUniform(const TMatrix4x3<GLfloat> & m) { UniformMatrix4x3fv(m.m()); }

	SimpleUniform(const TMatrix2<double> &m) { UniformMatrix2fv(((TMatrix2<float>)(m)).m()); }
	SimpleUniform(const TMatrix3<double> &m) { UniformMatrix3fv(((TMatrix3<float>)(m)).m()); }
	SimpleUniform(const TMatrix4<double> &m) { UniformMatrix4fv(((TMatrix4<float>)(m)).m()); }
	//SimpleUniform(const TMatrix2x3<double> & m) { UniformMatrix2x3fv(((TMatrix2x3<float>)(m)).m); }
	//SimpleUniform(const TMatrix2x4<double> & m) { UniformMatrix2x4fv(((TMatrix2x4<float>)(m)).m); }
	//SimpleUniform(const TMatrix3x2<double> & m) { UniformMatrix3x2fv(((TMatrix3x2<float>)(m)).m); }
	//SimpleUniform(const TMatrix3x4<double> & m) { UniformMatrix3x4fv(((TMatrix3x4<float>)(m)).m); }
	//SimpleUniform(const TMatrix4x2<double> & m) { UniformMatrix4x2fv(((TMatrix4x2<float>)(m)).m); }
	//SimpleUniform(const TMatrix4x3<double> & m) { UniformMatrix4x3fv(((TMatrix4x3<float>)(m)).m); }

	SimpleUniform(const TVector2<GLfloat> &v) { Uniform2fv(v.v()); }
	SimpleUniform(const TVector3<GLfloat> &v) { Uniform3fv(v.v()); }
	SimpleUniform(const TVector4<GLfloat> &v) { Uniform4fv(v.v()); }

	SimpleUniform(const TVector2<double> &v)
	{
		TVector2<float> u = (TVector2<float>)v;
		Uniform2fv(&u.x);
	}
	SimpleUniform(const TVector3<double> &v)
	{
		TVector3<float> u = (TVector3<float>)v;
		Uniform2fv(&u.x);
	}
	SimpleUniform(const TVector4<double> &v)
	{
		TVector4<float> u = (TVector4<float>)v;
		Uniform2fv(&u.x);
	}

	SimpleUniform(const TVector2<GLint> &v) { Uniform2iv(v.v()); }
	SimpleUniform(const TVector3<GLint> &v) { Uniform3iv(v.v()); }
	SimpleUniform(const TVector4<GLint> &v) { Uniform4iv(v.v()); }

	SimpleUniform(const TVector2<GLuint> &v) { Uniform2uiv(v.v()); }
	SimpleUniform(const TVector3<GLuint> &v) { Uniform3uiv(v.v()); }
	SimpleUniform(const TVector4<GLuint> &v) { Uniform4uiv(v.v()); }

	SimpleUniform(TMatrix2<GLfloat> &&m) { UniformMatrix2fv(m.m()); }
	SimpleUniform(TMatrix3<GLfloat> &&m) { UniformMatrix3fv(m.m()); }
	SimpleUniform(TMatrix4<GLfloat> &&m) { UniformMatrix4fv(m.m()); }
	//SimpleUniform(TMatrix2x3<GLfloat> && m) { UniformMatrix2x3fv(m.m); }
	//SimpleUniform(TMatrix2x4<GLfloat> && m) { UniformMatrix2x4fv(m.m); }
	//SimpleUniform(TMatrix3x2<GLfloat> && m) { UniformMatrix3x2fv(m.m); }
	//SimpleUniform(TMatrix3x4<GLfloat> && m) { UniformMatrix3x4fv(m.m); }
	//SimpleUniform(TMatrix4x2<GLfloat> && m) { UniformMatrix4x2fv(m.m); }
	//SimpleUniform(TMatrix4x3<GLfloat> && m) { UniformMatrix4x3fv(m.m); }

	SimpleUniform(TMatrix2<double> &&m) { UniformMatrix2fv(((TMatrix2<float>)(m)).m()); }
	SimpleUniform(TMatrix3<double> &&m) { UniformMatrix3fv(((TMatrix3<float>)(m)).m()); }
	SimpleUniform(TMatrix4<double> &&m) { UniformMatrix4fv(((TMatrix4<float>)(m)).m()); }
	//SimpleUniform(TMatrix2x3<double> && m) { UniformMatrix2x3fv(((TMatrix2x3<float>)(m)).m); }
	//SimpleUniform(TMatrix2x4<double> && m) { UniformMatrix2x4fv(((TMatrix2x4<float>)(m)).m); }
	//SimpleUniform(TMatrix3x2<double> && m) { UniformMatrix3x2fv(((TMatrix3x2<float>)(m)).m); }
	//SimpleUniform(TMatrix3x4<double> && m) { UniformMatrix3x4fv(((TMatrix3x4<float>)(m)).m); }
	//SimpleUniform(TMatrix4x2<double> && m) { UniformMatrix4x2fv(((TMatrix4x2<float>)(m)).m); }
	//SimpleUniform(TMatrix4x3<double> && m) { UniformMatrix4x3fv(((TMatrix4x3<float>)(m)).m); }

	SimpleUniform(TVector2<GLfloat> &&v) { Uniform2fv(v.v()); }
	SimpleUniform(TVector3<GLfloat> &&v) { Uniform3fv(v.v()); }
	SimpleUniform(TVector4<GLfloat> &&v) { Uniform4fv(v.v()); }

	SimpleUniform(TVector2<double> &&v) { Uniform2fv(((TVector2<float>)(v)).v()); }
	SimpleUniform(TVector3<double> &&v) { Uniform3fv(((TVector3<float>)(v)).v()); }
	SimpleUniform(TVector4<double> &&v) { Uniform4fv(((TVector4<float>)(v)).v()); }

	SimpleUniform(TVector2<GLint> &&v) { Uniform2iv(v.v()); }
	SimpleUniform(TVector3<GLint> &&v) { Uniform3iv(v.v()); }
	SimpleUniform(TVector4<GLint> &&v) { Uniform4iv(v.v()); }

	SimpleUniform(TVector2<GLuint> &&v) { Uniform2uiv(v.v()); }
	SimpleUniform(TVector3<GLuint> &&v) { Uniform3uiv(v.v()); }
	SimpleUniform(TVector4<GLuint> &&v) { Uniform4uiv(v.v()); }

	inline void SetProgramUniform(GLint uniformLocation)
	{
		if (uniformLocation < 0)
			return;
		GLenum whichType = 0;
		if (type == GL_FLOAT_MAT2 ||
			type == GL_FLOAT_MAT3 ||
			type == GL_FLOAT_MAT4 ||
			type == GL_FLOAT_MAT2x3 ||
			type == GL_FLOAT_MAT2x4 ||
			type == GL_FLOAT_MAT3x2 ||
			type == GL_FLOAT_MAT3x4 ||
			type == GL_FLOAT_MAT4x2 ||
			type == GL_FLOAT_MAT4x3 ||
			type == GL_DOUBLE_MAT2 ||
			type == GL_DOUBLE_MAT3 ||
			type == GL_DOUBLE_MAT4 ||
			type == GL_DOUBLE_MAT2x3 ||
			type == GL_DOUBLE_MAT2x4 ||
			type == GL_DOUBLE_MAT3x2 ||
			type == GL_DOUBLE_MAT3x4 ||
			type == GL_DOUBLE_MAT4x2 ||
			type == GL_DOUBLE_MAT4x3)
		{
			whichType = type;
		}
		else if (baseType == GL_FLOAT)
		{
			if (components == 1)
				whichType = GL_FLOAT;
			else if (components == 2)
				whichType = GL_FLOAT_VEC2;
			else if (components == 3)
				whichType = GL_FLOAT_VEC3;
			else if (components == 4)
				whichType = GL_FLOAT_VEC4;
		}
		else if (baseType == GL_INT)
		{
			if (components == 1)
				whichType = GL_INT;
			else if (components == 2)
				whichType = GL_INT_VEC2;
			else if (components == 3)
				whichType = GL_INT_VEC3;
			else if (components == 4)
				whichType = GL_INT_VEC4;
		}
		else if (baseType == GL_UNSIGNED_INT)
		{
			if (components == 1)
				whichType = GL_UNSIGNED_INT;
			else if (components == 2)
				whichType = GL_UNSIGNED_INT_VEC2;
			else if (components == 3)
				whichType = GL_UNSIGNED_INT_VEC3;
			else if (components == 4)
				whichType = GL_UNSIGNED_INT_VEC4;
		}
		switch (whichType)
		{
		case GL_FLOAT:
			glUniform1fv(uniformLocation, count, (GLfloat *)&buffer[0]);
			break;
		case GL_FLOAT_VEC2:
			glUniform2fv(uniformLocation, count, (GLfloat *)&buffer[0]);
			break;
		case GL_FLOAT_VEC3:
			glUniform3fv(uniformLocation, count, (GLfloat *)&buffer[0]);
			break;
		case GL_FLOAT_VEC4:
			glUniform4fv(uniformLocation, count, (GLfloat *)&buffer[0]);
			break;
		case GL_INT:
			glUniform1iv(uniformLocation, count, (GLint *)&buffer[0]);
			break;
		case GL_INT_VEC2:
			glUniform2iv(uniformLocation, count, (GLint *)&buffer[0]);
			break;
		case GL_INT_VEC3:
			glUniform3iv(uniformLocation, count, (GLint *)&buffer[0]);
			break;
		case GL_INT_VEC4:
			glUniform4iv(uniformLocation, count, (GLint *)&buffer[0]);
			break;
		case GL_UNSIGNED_INT:
			glUniform1uiv(uniformLocation, count, (GLuint *)&buffer[0]);
			break;
		case GL_UNSIGNED_INT_VEC2:
			glUniform2uiv(uniformLocation, count, (GLuint *)&buffer[0]);
			break;
		case GL_UNSIGNED_INT_VEC3:
			glUniform3uiv(uniformLocation, count, (GLuint *)&buffer[0]);
			break;
		case GL_UNSIGNED_INT_VEC4:
			glUniform4uiv(uniformLocation, count, (GLuint *)&buffer[0]);
			break;
		case GL_FLOAT_MAT2:
			glUniformMatrix2fv(uniformLocation, count, transpose, (GLfloat *)&buffer[0]);
			break;
		case GL_FLOAT_MAT3:
			glUniformMatrix3fv(uniformLocation, count, transpose, (GLfloat *)&buffer[0]);
			break;
		case GL_FLOAT_MAT4:
			glUniformMatrix4fv(uniformLocation, count, transpose, (GLfloat *)&buffer[0]);
			break;
		case GL_FLOAT_MAT2x3:
			glUniformMatrix2x3fv(uniformLocation, count, transpose, (GLfloat *)&buffer[0]);
			break;
		case GL_FLOAT_MAT2x4:
			glUniformMatrix2x4fv(uniformLocation, count, transpose, (GLfloat *)&buffer[0]);
			break;
		case GL_FLOAT_MAT3x2:
			glUniformMatrix3x2fv(uniformLocation, count, transpose, (GLfloat *)&buffer[0]);
			break;
		case GL_FLOAT_MAT3x4:
			glUniformMatrix3x4fv(uniformLocation, count, transpose, (GLfloat *)&buffer[0]);
			break;
		case GL_FLOAT_MAT4x2:
			glUniformMatrix4x2fv(uniformLocation, count, transpose, (GLfloat *)&buffer[0]);
			break;
		case GL_FLOAT_MAT4x3:
			glUniformMatrix4x3fv(uniformLocation, count, transpose, (GLfloat *)&buffer[0]);
			break;
		case GL_DOUBLE_MAT2:
			glUniformMatrix2dv(uniformLocation, count, transpose, (GLdouble *)&buffer[0]);
			break;
		case GL_DOUBLE_MAT3:
			glUniformMatrix3dv(uniformLocation, count, transpose, (GLdouble *)&buffer[0]);
			break;
		case GL_DOUBLE_MAT4:
			glUniformMatrix4dv(uniformLocation, count, transpose, (GLdouble *)&buffer[0]);
			break;
		case GL_DOUBLE_MAT2x3:
			glUniformMatrix2x3dv(uniformLocation, count, transpose, (GLdouble *)&buffer[0]);
			break;
		case GL_DOUBLE_MAT2x4:
			glUniformMatrix2x4dv(uniformLocation, count, transpose, (GLdouble *)&buffer[0]);
			break;
		case GL_DOUBLE_MAT3x2:
			glUniformMatrix3x2dv(uniformLocation, count, transpose, (GLdouble *)&buffer[0]);
			break;
		case GL_DOUBLE_MAT3x4:
			glUniformMatrix3x4dv(uniformLocation, count, transpose, (GLdouble *)&buffer[0]);
			break;
		case GL_DOUBLE_MAT4x2:
			glUniformMatrix4x2dv(uniformLocation, count, transpose, (GLdouble *)&buffer[0]);
			break;
		case GL_DOUBLE_MAT4x3:
			glUniformMatrix4x3dv(uniformLocation, count, transpose, (GLdouble *)&buffer[0]);
			break;
		}
	}

	inline void ChangeType(GLenum newType, GLint newCount, GLboolean newTranspose, const GLubyte *newData)
	{
		if (newData == NULL)
			return;
		if (type != newType)
		{
			location = -1;
		}
		type = newType;
		baseType = glutGetBaseType(newType);
		components = glutGetTypeComponents(newType);
		sizeOfType = glutGetSizeOfType(newType);
		sizeInBytes = newCount * sizeOfType;
		count = newCount;
		transpose = newTranspose;
		buffer.resize(sizeInBytes, 0);
		memcpy(&buffer[0], newData, sizeInBytes);
	}

	inline void Uniform1fv(GLenum newType, int newCount, const GLfloat *values) { ChangeType(newType, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform1fv(int newCount, const GLfloat *values) { ChangeType(GL_FLOAT, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform2fv(int newCount, const GLfloat *values) { ChangeType(GL_FLOAT_VEC2, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform3fv(int newCount, const GLfloat *values) { ChangeType(GL_FLOAT_VEC3, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform4fv(int newCount, const GLfloat *values) { ChangeType(GL_FLOAT_VEC4, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform1iv(int newCount, const GLint *values) { ChangeType(GL_INT, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform2iv(int newCount, const GLint *values) { ChangeType(GL_INT_VEC2, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform3iv(int newCount, const GLint *values) { ChangeType(GL_INT_VEC3, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform4iv(int newCount, const GLint *values) { ChangeType(GL_INT_VEC4, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform1uiv(int newCount, const GLuint *values) { ChangeType(GL_UNSIGNED_INT, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform2uiv(int newCount, const GLuint *values) { ChangeType(GL_UNSIGNED_INT_VEC2, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform3uiv(int newCount, const GLuint *values) { ChangeType(GL_UNSIGNED_INT_VEC3, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform4uiv(int newCount, const GLuint *values) { ChangeType(GL_UNSIGNED_INT_VEC4, newCount, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }

	inline void Uniform1fv(const GLfloat *values) { ChangeType(GL_FLOAT, 1, false, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform2fv(const GLfloat *values) { ChangeType(GL_FLOAT_VEC2, 1, false, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform3fv(const GLfloat *values) { ChangeType(GL_FLOAT_VEC3, 1, false, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform4fv(const GLfloat *values) { ChangeType(GL_FLOAT_VEC4, 1, false, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform1iv(const GLint *values) { ChangeType(GL_INT, 1, false, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform2iv(const GLint *values) { ChangeType(GL_INT_VEC2, 1, false, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform3iv(const GLint *values) { ChangeType(GL_INT_VEC3, 1, false, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform4iv(const GLint *values) { ChangeType(GL_INT_VEC4, 1, false, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform1uiv(const GLuint *values) { ChangeType(GL_UNSIGNED_INT, 1, false, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform2uiv(const GLuint *values) { ChangeType(GL_UNSIGNED_INT_VEC2, 1, false, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform3uiv(const GLuint *values) { ChangeType(GL_UNSIGNED_INT_VEC3, 1, false, reinterpret_cast<const GLubyte *>(values)); }
	inline void Uniform4uiv(const GLuint *values) { ChangeType(GL_UNSIGNED_INT_VEC4, 1, false, reinterpret_cast<const GLubyte *>(values)); }

	inline void UniformMatrix2fv(int newCount, GLboolean newTranspose, const GLfloat *values) { ChangeType(GL_FLOAT_MAT2, newCount, newTranspose, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix3fv(int newCount, GLboolean newTranspose, const GLfloat *values) { ChangeType(GL_FLOAT_MAT3, newCount, newTranspose, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix4fv(int newCount, GLboolean newTranspose, const GLfloat *values) { ChangeType(GL_FLOAT_MAT4, newCount, newTranspose, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix2x3fv(int newCount, GLboolean newTranspose, const GLfloat *values) { ChangeType(GL_FLOAT_MAT2x3, newCount, newTranspose, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix2x4fv(int newCount, GLboolean newTranspose, const GLfloat *values) { ChangeType(GL_FLOAT_MAT2x4, newCount, newTranspose, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix3x2fv(int newCount, GLboolean newTranspose, const GLfloat *values) { ChangeType(GL_FLOAT_MAT3x2, newCount, newTranspose, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix3x4fv(int newCount, GLboolean newTranspose, const GLfloat *values) { ChangeType(GL_FLOAT_MAT3x4, newCount, newTranspose, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix4x2fv(int newCount, GLboolean newTranspose, const GLfloat *values) { ChangeType(GL_FLOAT_MAT4x2, newCount, newTranspose, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix4x3fv(int newCount, GLboolean newTranspose, const GLfloat *values) { ChangeType(GL_FLOAT_MAT4x3, newCount, newTranspose, reinterpret_cast<const GLubyte *>(values)); }

	inline void UniformMatrix2fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT2, 1, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix3fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT3, 1, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix4fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT4, 1, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix2x3fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT2x3, 1, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix2x4fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT2x4, 1, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix3x2fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT3x2, 1, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix3x4fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT3x4, 1, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix4x2fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT4x2, 1, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformMatrix4x3fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT4x3, 1, GL_FALSE, reinterpret_cast<const GLubyte *>(values)); }

	inline void UniformTransposeMatrix2fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT2, 1, GL_TRUE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformTransposeMatrix3fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT3, 1, GL_TRUE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformTransposeMatrix4fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT4, 1, GL_TRUE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformTransposeMatrix2x3fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT2x3, 1, GL_TRUE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformTransposeMatrix2x4fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT2x4, 1, GL_TRUE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformTransposeMatrix3x2fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT3x2, 1, GL_TRUE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformTransposeMatrix3x4fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT3x4, 1, GL_TRUE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformTransposeMatrix4x2fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT4x2, 1, GL_TRUE, reinterpret_cast<const GLubyte *>(values)); }
	inline void UniformTransposeMatrix4x3fv(const GLfloat *values) { ChangeType(GL_FLOAT_MAT4x3, 1, GL_TRUE, reinterpret_cast<const GLubyte *>(values)); }

	template <typename T>
	T *getAs()
	{
		if (buffer.empty())
			return NULL;
		return reinterpret_cast<T *>(&buffer[0]);
	}
};

struct SimpleShader
{
	GLenum type = 0;
	GLuint shader = 0;
	string source;
	string infoLog;
	bool didCompile = false;
	bool hadError = false;

	SimpleShader()
	{
	}

	~SimpleShader()
	{
		Delete();
	}

	void Create(GLenum shaderType)
	{
		if (shader)
			Delete();
		type = shaderType;
		shader = glCreateShader(type);
		source = "";
		didCompile = false;
		hadError = false;
		hflog.info("%s(): shader %d created.", __FUNCTION__, shader);
	}

	void Delete()
	{
		if (shader != 0)
		{
			hflog.info("%s(): shader %d deleted.", __FUNCTION__, shader);
			glDeleteShader(shader);
			shader = 0;
		}
	}
};

struct SimpleUniformBlock
{
	GLuint blockIndex = 0;
	GLuint blockBinding = 0;
	GLint blockSize = 0;
	GLint blockOffset = 0;
};

struct SimpleUniformBlockSystem
{
	GLuint uniformBlockBufferId = 0;
	GLuint blockIndexId = 0;
	vector<GLbyte> buffer;

	SimpleUniformBlockSystem();
	~SimpleUniformBlockSystem();

	void Create(GLsizei totalMemory, GLuint blockIndex);
	void Delete();
	void Update();
	void UpdateSubData(GLsizei offset, GLsizei size, GLbyte *data);
};

shared_ptr<SimpleShader> CompileShaderFromFile(GLenum type, const string &filename);

class SimpleProgram
{
  private:
	GLuint program = 0;
	bool linked = false;
	vector<shared_ptr<SimpleShader>> shaders;
	string infoLog;
	GLint linkStatus_ = 0;
	GLint validateStatus_ = 0;
	string validateLog;
	//uniformBlocks;

  public:
	struct AttribUniformInfo
	{
		GLint size;
		GLenum type;
		GLint index;

		const char *GetNameOfType();
	};
	map<string, AttribUniformInfo> activeAttributes;
	map<string, AttribUniformInfo> activeUniforms;

	SimpleProgram();
	~SimpleProgram();

	void Create();
	void Delete();
	GLint GetAttribLocation(const char *name);
	GLint GetUniformLocation(const char *name);
	GLint GetUniformBlockIndex(const char *name);
	const string &GetInfoLog();
	GLuint GetProgram() { return program; }

	void SetUniformBlock(const string &uniformBlockName, GLuint buffer, GLuint blockBindingIndex, GLintptr offset, GLsizei size);

	bool IsLinked() { return linked; }
	void Use();
	void ApplyUniforms(map<string, SimpleUniform> uniforms);
	bool ApplyUniform(const string &uniformName, SimpleUniform &uniform);
	void AttachShaders(shared_ptr<SimpleShader> &shaderPtr);
	void BindAttribLocation(GLuint index, const char *name);
	bool Link();
};

using SimpleProgramPtr = shared_ptr<SimpleProgram>;

enum class VertexType
{
	UNDECIDED,
	FAST_VERTEX,
	SLOW_VERTEX
};

// 12 bytes/vertex
struct SimpleZVertex
{
	GLfloat position[3];

	SimpleZVertex()
	{
		position[0] = 0.0f;
		position[1] = 0.0f;
		position[2] = 0.0f;
	}

	SimpleZVertex(GLfloat x, GLfloat y, GLfloat z)
	{
		position[0] = x;
		position[1] = y;
		position[2] = z;
	}
};

// 32 bytes/vertex
struct SimpleFastVertex
{
	GLshort position[4];
	GLshort normal[4];
	GLshort texCoord[2];
	GLubyte color[4];
	GLshort attrib[4];
};

// new type 48 bytes/vertex
struct SimpleVertex
{
	Vector3f position;
	Vector3f normal;
	Vector2f texcoord;
	Color4f color;
};

// 128 bytes/vertex
struct SimpleSlowVertex
{
	GLfloat attrib[8][4];
};

/// <summary>SimpleSurface is a class representing a drawable object</summary>
/// A Simple Surface represents a geometrical object with prescribed properties.
/// It is consistent in the following respects:
/// - One type of mode (POINTS, LINES, LINE_LOOP, LINE_STRIP, TRIANGLES, TRIANGLE_STRIP
/// - Specified by a vertex array or an indexed vertex array
/// - A surface may be rendered multiple times with different programs/uniforms but not vice versa
struct SimpleSurface
{
	VertexType vertexType = VertexType::UNDECIDED;
	bool isIndexed = false;

	// Valid for both DrawArrays and DrawElements
	GLenum mode = 0;
	GLsizei count = 0;

	// glDrawArrays
	GLint first = 0;

	// Z Only mode
	GLint zFirst = 0;
	GLsizei zCount = 0;

	// glDrawElements
	// baseIndexOffset is the index offset into the indices vector (not respective of slow or fast vertices)
	// offset is the memory buffer offset (computed by BuildBuffers)
	GLenum type = 0;
	GLsizei firstIndex = 0;
	GLsizei firstZIndex = 0;
	mutable GLsizei baseZIndexBufferOffset = 0;
	mutable GLsizei baseIndexBufferOffset = 0;

	// Scene graph information
	GLuint groupId = 0;
	GLuint objectId = 0;
	GLuint programId = 0;
	GLuint mtlId = 0;
	GLuint mtllibId = 0;

	string mtlName;
	string mtllibName;
	string objectName;
	string groupName;
};

/// <summary>SimpleRenderer handles the needs of several different rendering approaches</summary>
/// It is designed to accomodate a variety of different rendering options.
/// Z Only. This outputs a 3-float position only for 12 bytes/vertex.
/// Slow Vertex. This outputs 8 generic 4 component float vertex attributes for 128 bytes/vertex.
/// Fast Vertex. This uses mostly integer inputs to reduce size to 32 bytes/vertex.
/// <code>RenderZOnly()</code> renders all surfaces as Z only.
/// <code>Render()</code> renders all surfaces as slow/fast vertex according to how data was input.
template <typename IndexType, GLenum GLIndexType>
class SimpleRenderer
{
  private:
	GLuint arrayBuffer = 0;		   // memory structure [ZONLY, FAST VERTICES, SLOW VERTICES]
	GLuint elementArrayBuffer = 0; // memory structure [ZONLY, FAST VERTICES, SLOW VERTICES]

	vector<GLubyte> vertexMemoryBuffer;
	vector<GLubyte> indexMemoryBuffer;

	vector<SimpleZVertex> zVertices;
	vector<SimpleFastVertex> fastVertices;
	vector<SimpleSlowVertex> slowVertices;
	vector<IndexType> Indices;
	vector<IndexType> zIndices;
	vector<SimpleSurface> surfaces;

	struct BUFFERINFO
	{
		GLsizei zVertexOffset;
		GLsizei zVertexSize;
		GLsizei fastVertexOffset;
		GLsizei fastVertexSize;
		GLsizei slowVertexOffset;
		GLsizei slowVertexSize;

		GLsizei zIndexOffset;
		GLsizei zIndexSize;
		GLsizei IndexOffset;
		GLsizei IndexSize;

		GLsizei vertexBufferSizeInBytes;
		GLsizei indexBufferSizeInBytes;
	};
	BUFFERINFO bufferInfo;

	SimpleFastVertex currentFastVertex;
	SimpleSlowVertex currentSlowVertex;

	IndexType baseZIndex = 0;
	IndexType baseFastIndex = 0;
	IndexType baseSlowIndex = 0;

	GLuint currentMtlLibId = 0;
	GLuint currentMtlId = 0;
	GLuint currentGroupId = 0;
	GLuint currentObjectId = 0;
	GLuint currentProgramId = 0;
	string currentMtlName;
	string currentMtlLibName;
	string currentObjectName;
	string currentGroupName;

	VertexType lastVertexType = VertexType::UNDECIDED;

	// Vertex Array Objects
	GLuint zVAO = 0;
	GLuint slowVAO = 0;
	GLuint fastVAO = 0;

	bool isMakingSurface = false;
	unsigned currentSurface = 0;

	void BuildMemoryBuffers();
	void HandleVertexTypeChange(VertexType vertexType);
	void EmitVertex();
	void ZVertex(GLfloat x, GLfloat y, GLfloat z);

  public:
	SimpleRenderer();
	~SimpleRenderer();

	map<string, SimpleProgramPtr> programs;

	void SetCurrentMtlLibId(GLuint value) { currentMtlLibId = value; }
	void SetCurrentMtlId(GLuint value) { currentMtlId = value; }
	void SetCurrentObjectId(GLuint value) { currentObjectId = value; }
	void SetCurrentGroupId(GLuint value) { currentGroupId = value; }
	void SetCurrentProgramId(GLuint value) { currentProgramId = value; }

	GLuint GetCurrentMtlLibId() const { return currentMtlLibId; }
	GLuint GetCurrentMtlId() const { return currentMtlId; }
	GLuint GetCurrentObjectId() const { return currentObjectId; }
	GLuint GetCurrentGroupId() const { return currentGroupId; }
	GLuint GetCurrentProgramId() const { return currentProgramId; }

	void SetCurrentMtlName(const string &name) { currentMtlName = name; }
	void SetCurrentMtlLibName(const string &name) { currentMtlLibName = name; }
	void SetCurrentObjectName(const string &name) { currentObjectName = name; }
	void SetCurrentGroupName(const string &name) { currentGroupName = name; }

	const string &GetCurrentObjectName() const { return currentObjectName; }
	const string &GetCurrentMtlLibName() const { return currentMtlLibName; }
	const string &GetCurrentMtlName() const { return currentMtlName; }
	const string &GetCurrentGroupName() const { return currentGroupName; }

	void ApplyIdToObjectNames(const string &objectName, GLuint id);
	void ApplyIdToGroupNames(const string &groupName, GLuint id);
	void ApplyIdToMtlLibNames(const string &mtllibName, GLuint id);
	void ApplyIdToMtlNames(const string &mtlName, GLuint id);
	void AssignUniqueGroupIds();

	void AssignMaterialIds(SimpleMaterialSystem &materials);

	void Begin(GLenum mode, bool isIndexed = false);
	void End();
	void NewObject();

	void Index(IndexType index);
	void Index(vector<IndexType> indices);

	void VertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void Vertex4s(GLshort x, GLshort y, GLshort z, GLshort w);
	void Normal4s(GLshort x, GLshort y, GLshort z, GLshort w);
	void TexCoord2s(GLshort s, GLshort t);
	void Color4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
	void Attrib4s(GLshort x, GLshort y, GLshort z, GLshort w);

	void BuildBuffers();
	void BindBuffers();
	void Reset();
	void Render();
	void RenderIf(const string &objectName, const string &groupName, const string &mtllibName, const string &mtlName, bool onlyRenderZ = false);
	void RenderIf(GLuint objectId = 0, GLuint groupId = 0, GLuint mtllibId = 0, GLuint mtlId = 0, bool onlyRenderZ = false);
	void RenderZOnly();

	//void Configure(Renderer *pRenderer);
	int vertexCount = 0;
	int triangleCount = 0;
};

extern template class SimpleRenderer<GLbyte, GL_BYTE>;
extern template class SimpleRenderer<GLubyte, GL_UNSIGNED_BYTE>;
extern template class SimpleRenderer<GLshort, GL_SHORT>;
extern template class SimpleRenderer<GLushort, GL_UNSIGNED_SHORT>;
extern template class SimpleRenderer<GLint, GL_INT>;
extern template class SimpleRenderer<GLuint, GL_UNSIGNED_INT>;

using SimpleRenderer_GLubyte = SimpleRenderer<GLubyte, GL_UNSIGNED_BYTE>;
using SimpleRenderer_GLushort = SimpleRenderer<GLushort, GL_UNSIGNED_SHORT>;
using SimpleRenderer_GLuint = SimpleRenderer<GLuint, GL_UNSIGNED_INT>;
} // namespace Fluxions

#endif
