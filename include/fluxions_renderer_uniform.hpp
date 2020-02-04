#ifndef FLUXIONS_RENDERER_UNIFORM_HPP
#define FLUXIONS_RENDERER_UNIFORM_HPP

#include <fluxions_base.hpp>
#include <fluxions_gl1gl2_tools.hpp>
#include <fluxions_gte_matrix_math.hpp>

namespace Fluxions {
	// FIXME: Support uniform blocks in the future

	//struct RendererUniformBlock
	//{
	//	GLuint blockIndex = 0;
	//	GLuint blockBinding = 0;
	//	GLint blockSize = 0;
	//	GLint blockOffset = 0;
	//};

	//struct RendererUniformBlockSystem
	//{
	//	GLuint uniformBlockBufferId = 0;
	//	GLuint blockIndexId = 0;
	//	std::vector<GLbyte> buffer;

	//	RendererUniformBlockSystem();
	//	~RendererUniformBlockSystem();

	//	void Create(GLsizei totalMemory, GLuint blockIndex);
	//	void Delete();
	//	void Update();
	//	void UpdateSubData(GLsizei offset, GLsizei size, GLbyte* data);
	//};

	class RendererUniform {
	public:
		RendererUniform() { changeType(GL_INT, 1, GL_FALSE, NULL); }
		~RendererUniform() { buffer.clear(); }

		RendererUniform(GLint intValue) { changeType(GL_INT, 1, GL_FALSE, (GLubyte*)&intValue); }
		RendererUniform(GLuint uintValue) { changeType(GL_UNSIGNED_INT, 1, GL_FALSE, (GLubyte*)&uintValue); }
		RendererUniform(GLfloat floatValue) { changeType(GL_FLOAT, 1, GL_FALSE, (GLubyte*)&floatValue); }
		RendererUniform(GLdouble doubleValue) { changeType(GL_DOUBLE, 1, GL_FALSE, (GLubyte*)&doubleValue); }
		RendererUniform(GLsizei count, GLfloat* v) { changeType(GL_FLOAT, count, GL_FALSE, (GLubyte*)v); }
		RendererUniform(GLsizei count, GLdouble* v) { changeType(GL_DOUBLE, count, GL_FALSE, (GLubyte*)v); }
		RendererUniform(GLenum newType, GLint newCount, GLboolean newTranspose, const GLubyte* newData) { changeType(newType, newCount, newTranspose, newData); }
		RendererUniform(GLenum newType, GLint newCount, GLboolean newTranspose, const GLint* newData) { changeType(newType, newCount, newTranspose, reinterpret_cast<const GLubyte*>(newData)); }
		RendererUniform(GLenum newType, GLint newCount, GLboolean newTranspose, const GLuint* newData) { changeType(newType, newCount, newTranspose, reinterpret_cast<const GLubyte*>(newData)); }
		RendererUniform(GLenum newType, GLint newCount, GLboolean newTranspose, const GLfloat* newData) { changeType(newType, newCount, newTranspose, reinterpret_cast<const GLubyte*>(newData)); }

		GLint location = -1;
		GLenum type = 0;
		GLenum baseType = 0;
		GLsizei count = 0;
		GLsizei sizeOfType = 0;
		GLsizei sizeInBytes = 0;
		GLboolean transpose = 0;
		GLint components = 1;

		std::vector<GLubyte> buffer;

		inline const RendererUniform& operator=(const RendererUniform& uniform) {
			// name = uniform.name;
			changeType(uniform.type, uniform.count, uniform.transpose, uniform.buffer.data());
			return *this;
		}

		inline RendererUniform& operator=(const TMatrix2<GLfloat>& m) {
			UniformMatrix2fv(m.const_ptr());
			return *this;
		}
		inline RendererUniform& operator=(const TMatrix3<GLfloat>& m) {
			UniformMatrix3fv(m.const_ptr());
			return *this;
		}
		inline RendererUniform& operator=(const TMatrix4<GLfloat>& m) {
			UniformMatrix4fv(m.const_ptr());
			return *this;
		}
		//inline RendererUniform & operator=(const TMatrix2x3<GLfloat> &ptr) { UniformMatrix2x3fv(ptr.ptr); return *this; }
		//inline RendererUniform & operator=(const TMatrix2x4<GLfloat> &ptr) { UniformMatrix2x3fv(ptr.ptr); return *this; }
		//inline RendererUniform & operator=(const TMatrix3x2<GLfloat> &ptr) { UniformMatrix2x3fv(ptr.ptr); return *this; }
		//inline RendererUniform & operator=(const TMatrix3x4<GLfloat> &ptr) { UniformMatrix2x3fv(ptr.ptr); return *this; }
		//inline RendererUniform & operator=(const TMatrix4x2<GLfloat> &ptr) { UniformMatrix2x3fv(ptr.ptr); return *this; }
		//inline RendererUniform & operator=(const TMatrix4x3<GLfloat> &ptr) { UniformMatrix2x3fv(ptr.ptr); return *this; }

		inline RendererUniform& operator=(const TMatrix2<double>& m) { return *this = (TMatrix2<float>)(m); }
		inline RendererUniform& operator=(const TMatrix3<double>& m) { return *this = (TMatrix3<float>)(m); }
		inline RendererUniform& operator=(const TMatrix4<double>& m) { return *this = (TMatrix4<float>)(m); }
		//inline RendererUniform & operator=(const TMatrix2x3<double> &ptr) { return *this = (TMatrix2x3<float>)(ptr); }
		//inline RendererUniform & operator=(const TMatrix2x4<double> &ptr) { return *this = (TMatrix2x4<float>)(ptr); }
		//inline RendererUniform & operator=(const TMatrix3x2<double> &ptr) { return *this = (TMatrix3x2<float>)(ptr); }
		//inline RendererUniform & operator=(const TMatrix3x4<double> &ptr) { return *this = (TMatrix3x4<float>)(ptr); }
		//inline RendererUniform & operator=(const TMatrix4x2<double> &ptr) { return *this = (TMatrix4x2<float>)(ptr); }
		//inline RendererUniform & operator=(const TMatrix4x3<double> &ptr) { return *this = (TMatrix4x3<float>)(ptr); }

		inline RendererUniform& operator=(const TVector2<GLfloat>& v) {
			Uniform2fv(v.const_ptr());
			return *this;
		}

		inline RendererUniform& operator=(const TVector3<GLfloat>& v) {
			Uniform3fv(v.const_ptr());
			return *this;
		}

		inline RendererUniform& operator=(const TVector4<GLfloat>& v) {
			Uniform4fv(v.const_ptr());
			return *this;
		}

		inline RendererUniform& operator=(const TVector2<double>& v) { return *this = (TVector2<float>)(v); }
		inline RendererUniform& operator=(const TVector3<double>& v) { return *this = (TVector3<float>)(v); }
		inline RendererUniform& operator=(const TVector4<double>& v) { return *this = (TVector4<float>)(v); }

		inline RendererUniform& operator=(const TVector2<GLint>& v) {
			Uniform2iv(v.const_ptr());
			return *this;
		}

		inline RendererUniform& operator=(const TVector3<GLint>& v) {
			Uniform3iv(v.const_ptr());
			return *this;
		}

		inline RendererUniform& operator=(const TVector4<GLint>& v) {
			Uniform4iv(v.const_ptr());
			return *this;
		}

		inline RendererUniform& operator=(const TVector2<GLuint>& v) {
			Uniform2uiv(v.const_ptr());
			return *this;
		}

		inline RendererUniform& operator=(const TVector3<GLuint>& v) {
			Uniform3uiv(v.const_ptr());
			return *this;
		}

		inline RendererUniform& operator=(const TVector4<GLuint>& v) {
			Uniform4uiv(v.const_ptr());
			return *this;
		}

		RendererUniform(const TMatrix2<GLfloat>& m) { UniformMatrix2fv(m.const_ptr()); }
		RendererUniform(const TMatrix3<GLfloat>& m) { UniformMatrix3fv(m.const_ptr()); }
		RendererUniform(const TMatrix4<GLfloat>& m) { UniformMatrix4fv(m.const_ptr()); }
		//RendererUniform(const TMatrix2x3<GLfloat> & m) { UniformMatrix2x3fv(m.const_ptr()); }
		//RendererUniform(const TMatrix2x4<GLfloat> & m) { UniformMatrix2x4fv(m.const_ptr()); }
		//RendererUniform(const TMatrix3x2<GLfloat> & m) { UniformMatrix3x2fv(m.const_ptr()); }
		//RendererUniform(const TMatrix3x4<GLfloat> & m) { UniformMatrix3x4fv(m.const_ptr()); }
		//RendererUniform(const TMatrix4x2<GLfloat> & m) { UniformMatrix4x2fv(m.const_ptr()); }
		//RendererUniform(const TMatrix4x3<GLfloat> & m) { UniformMatrix4x3fv(m.const_ptr()); }

		RendererUniform(const TMatrix2<double>& m) { UniformMatrix2fv(((TMatrix2<float>)(m)).const_ptr()); }
		RendererUniform(const TMatrix3<double>& m) { UniformMatrix3fv(((TMatrix3<float>)(m)).const_ptr()); }
		RendererUniform(const TMatrix4<double>& m) { UniformMatrix4fv(((TMatrix4<float>)(m)).const_ptr()); }
		//RendererUniform(const TMatrix2x3<double> & m) { UniformMatrix2x3fv(((TMatrix2x3<float>)(m)).const_ptr()); }
		//RendererUniform(const TMatrix2x4<double> & m) { UniformMatrix2x4fv(((TMatrix2x4<float>)(m)).const_ptr()); }
		//RendererUniform(const TMatrix3x2<double> & m) { UniformMatrix3x2fv(((TMatrix3x2<float>)(m)).const_ptr()); }
		//RendererUniform(const TMatrix3x4<double> & m) { UniformMatrix3x4fv(((TMatrix3x4<float>)(m)).const_ptr()); }
		//RendererUniform(const TMatrix4x2<double> & m) { UniformMatrix4x2fv(((TMatrix4x2<float>)(m)).const_ptr()); }
		//RendererUniform(const TMatrix4x3<double> & m) { UniformMatrix4x3fv(((TMatrix4x3<float>)(m)).const_ptr()); }

		RendererUniform(const TVector2<GLfloat>& v) { Uniform2fv(v.const_ptr()); }
		RendererUniform(const TVector3<GLfloat>& v) { Uniform3fv(v.const_ptr()); }
		RendererUniform(const TVector4<GLfloat>& v) { Uniform4fv(v.const_ptr()); }

		RendererUniform(const TColor3<GLfloat>& v) { Uniform3fv(v.const_ptr()); }
		RendererUniform(const TColor4<GLfloat>& v) { Uniform4fv(v.const_ptr()); }

		RendererUniform(const TVector2<double>& v) {
			TVector2<float> u = (TVector2<float>)v;
			Uniform2fv(&u.x);
		}

		RendererUniform(const TVector3<double>& v) {
			TVector3<float> u = (TVector3<float>)v;
			Uniform2fv(&u.x);
		}

		RendererUniform(const TVector4<double>& v) {
			TVector4<float> u = (TVector4<float>)v;
			Uniform2fv(&u.x);
		}

		RendererUniform(const TVector2<GLint>& v) { Uniform2iv(v.const_ptr()); }
		RendererUniform(const TVector3<GLint>& v) { Uniform3iv(v.const_ptr()); }
		RendererUniform(const TVector4<GLint>& v) { Uniform4iv(v.const_ptr()); }

		RendererUniform(const TVector2<GLuint>& v) { Uniform2uiv(v.const_ptr()); }
		RendererUniform(const TVector3<GLuint>& v) { Uniform3uiv(v.const_ptr()); }
		RendererUniform(const TVector4<GLuint>& v) { Uniform4uiv(v.const_ptr()); }

		//RendererUniform(TMatrix2<GLfloat> &&m) { UniformMatrix2fv(m.const_ptr()); }
		//RendererUniform(TMatrix3<GLfloat> &&m) { UniformMatrix3fv(m.const_ptr()); }
		//RendererUniform(TMatrix4<GLfloat> &&m) { UniformMatrix4fv(m.ptr()); }
		//RendererUniform(TMatrix2x3<GLfloat> && ptr) { UniformMatrix2x3fv(ptr.ptr); }
		//RendererUniform(TMatrix2x4<GLfloat> && ptr) { UniformMatrix2x4fv(ptr.ptr); }
		//RendererUniform(TMatrix3x2<GLfloat> && ptr) { UniformMatrix3x2fv(ptr.ptr); }
		//RendererUniform(TMatrix3x4<GLfloat> && ptr) { UniformMatrix3x4fv(ptr.ptr); }
		//RendererUniform(TMatrix4x2<GLfloat> && ptr) { UniformMatrix4x2fv(ptr.ptr); }
		//RendererUniform(TMatrix4x3<GLfloat> && ptr) { UniformMatrix4x3fv(ptr.ptr); }

		//RendererUniform(TMatrix2<double> &&m) { UniformMatrix2fv(((TMatrix2<float>)(m)).const_ptr()); }
		//RendererUniform(TMatrix3<double> &&m) { UniformMatrix3fv(((TMatrix3<float>)(m)).const_ptr()); }
		//RendererUniform(TMatrix4<double> &&m) { UniformMatrix4fv(((TMatrix4<float>)(m)).const_ptr()); }
		//RendererUniform(TMatrix2x3<double> && ptr) { UniformMatrix2x3fv(((TMatrix2x3<float>)(m)).const_ptr()); }
		//RendererUniform(TMatrix2x4<double> && ptr) { UniformMatrix2x4fv(((TMatrix2x4<float>)(m)).const_ptr()); }
		//RendererUniform(TMatrix3x2<double> && ptr) { UniformMatrix3x2fv(((TMatrix3x2<float>)(m)).const_ptr()); }
		//RendererUniform(TMatrix3x4<double> && ptr) { UniformMatrix3x4fv(((TMatrix3x4<float>)(m)).const_ptr()); }
		//RendererUniform(TMatrix4x2<double> && ptr) { UniformMatrix4x2fv(((TMatrix4x2<float>)(m)).const_ptr()); }
		//RendererUniform(TMatrix4x3<double> && ptr) { UniformMatrix4x3fv(((TMatrix4x3<float>)(m)).const_ptr()); }

		//RendererUniform(TVector2<GLfloat> &&v) { Uniform2fv(v.const_ptr()); }
		//RendererUniform(TVector3<GLfloat> &&v) { Uniform3fv(v.const_ptr()); }
		//RendererUniform(TVector4<GLfloat> &&v) { Uniform4fv(v.const_ptr()); }

		//RendererUniform(TVector2<double> &&v) { Uniform2fv(((TVector2<float>)(v)).v()); }
		//RendererUniform(TVector3<double> &&v) { Uniform3fv(((TVector3<float>)(v)).v()); }
		//RendererUniform(TVector4<double> &&v) { Uniform4fv(((TVector4<float>)(v)).v()); }

		//RendererUniform(TVector2<GLint> &&v) { Uniform2iv(v.const_ptr()); }
		//RendererUniform(TVector3<GLint> &&v) { Uniform3iv(v.const_ptr()); }
		//RendererUniform(TVector4<GLint> &&v) { Uniform4iv(v.const_ptr()); }

		//RendererUniform(TVector2<GLuint> &&v) { Uniform2uiv(v.const_ptr()); }
		//RendererUniform(TVector3<GLuint> &&v) { Uniform3uiv(v.const_ptr()); }
		//RendererUniform(TVector4<GLuint> &&v) { Uniform4uiv(v.const_ptr()); }

		inline void setProgramUniform(GLint uniformLocation) const {
			if (uniformLocation < 0)
				return;
			GLenum whichType = 0;
			if (type == GL_FLOAT_MAT2 || type == GL_FLOAT_MAT3 || type == GL_FLOAT_MAT4 || type == GL_FLOAT_MAT2x3 || type == GL_FLOAT_MAT2x4 || type == GL_FLOAT_MAT3x2 || type == GL_FLOAT_MAT3x4 || type == GL_FLOAT_MAT4x2 || type == GL_FLOAT_MAT4x3 || type == GL_DOUBLE_MAT2 || type == GL_DOUBLE_MAT3 || type == GL_DOUBLE_MAT4 || type == GL_DOUBLE_MAT2x3 || type == GL_DOUBLE_MAT2x4 || type == GL_DOUBLE_MAT3x2 || type == GL_DOUBLE_MAT3x4 || type == GL_DOUBLE_MAT4x2 || type == GL_DOUBLE_MAT4x3) {
				whichType = type;
			}
			else if (baseType == GL_FLOAT) {
				if (components == 1)
					whichType = GL_FLOAT;
				else if (components == 2)
					whichType = GL_FLOAT_VEC2;
				else if (components == 3)
					whichType = GL_FLOAT_VEC3;
				else if (components == 4)
					whichType = GL_FLOAT_VEC4;
			}
			else if (baseType == GL_INT) {
				if (components == 1)
					whichType = GL_INT;
				else if (components == 2)
					whichType = GL_INT_VEC2;
				else if (components == 3)
					whichType = GL_INT_VEC3;
				else if (components == 4)
					whichType = GL_INT_VEC4;
			}
			else if (baseType == GL_UNSIGNED_INT) {
				if (components == 1)
					whichType = GL_UNSIGNED_INT;
				else if (components == 2)
					whichType = GL_UNSIGNED_INT_VEC2;
				else if (components == 3)
					whichType = GL_UNSIGNED_INT_VEC3;
				else if (components == 4)
					whichType = GL_UNSIGNED_INT_VEC4;
			}
			switch (whichType) {
			case GL_FLOAT:
				glUniform1fv(uniformLocation, count, (GLfloat*)&buffer[0]);
				break;
			case GL_FLOAT_VEC2:
				glUniform2fv(uniformLocation, count, (GLfloat*)&buffer[0]);
				break;
			case GL_FLOAT_VEC3:
				glUniform3fv(uniformLocation, count, (GLfloat*)&buffer[0]);
				break;
			case GL_FLOAT_VEC4:
				glUniform4fv(uniformLocation, count, (GLfloat*)&buffer[0]);
				break;
			case GL_INT:
				glUniform1iv(uniformLocation, count, (GLint*)&buffer[0]);
				break;
			case GL_INT_VEC2:
				glUniform2iv(uniformLocation, count, (GLint*)&buffer[0]);
				break;
			case GL_INT_VEC3:
				glUniform3iv(uniformLocation, count, (GLint*)&buffer[0]);
				break;
			case GL_INT_VEC4:
				glUniform4iv(uniformLocation, count, (GLint*)&buffer[0]);
				break;
			case GL_UNSIGNED_INT:
				glUniform1uiv(uniformLocation, count, (GLuint*)&buffer[0]);
				break;
			case GL_UNSIGNED_INT_VEC2:
				glUniform2uiv(uniformLocation, count, (GLuint*)&buffer[0]);
				break;
			case GL_UNSIGNED_INT_VEC3:
				glUniform3uiv(uniformLocation, count, (GLuint*)&buffer[0]);
				break;
			case GL_UNSIGNED_INT_VEC4:
				glUniform4uiv(uniformLocation, count, (GLuint*)&buffer[0]);
				break;
			case GL_FLOAT_MAT2:
				glUniformMatrix2fv(uniformLocation, count, transpose, (GLfloat*)&buffer[0]);
				break;
			case GL_FLOAT_MAT3:
				glUniformMatrix3fv(uniformLocation, count, transpose, (GLfloat*)&buffer[0]);
				break;
			case GL_FLOAT_MAT4:
				glUniformMatrix4fv(uniformLocation, count, transpose, (GLfloat*)&buffer[0]);
				break;
			case GL_FLOAT_MAT2x3:
				glUniformMatrix2x3fv(uniformLocation, count, transpose, (GLfloat*)&buffer[0]);
				break;
			case GL_FLOAT_MAT2x4:
				glUniformMatrix2x4fv(uniformLocation, count, transpose, (GLfloat*)&buffer[0]);
				break;
			case GL_FLOAT_MAT3x2:
				glUniformMatrix3x2fv(uniformLocation, count, transpose, (GLfloat*)&buffer[0]);
				break;
			case GL_FLOAT_MAT3x4:
				glUniformMatrix3x4fv(uniformLocation, count, transpose, (GLfloat*)&buffer[0]);
				break;
			case GL_FLOAT_MAT4x2:
				glUniformMatrix4x2fv(uniformLocation, count, transpose, (GLfloat*)&buffer[0]);
				break;
			case GL_FLOAT_MAT4x3:
				glUniformMatrix4x3fv(uniformLocation, count, transpose, (GLfloat*)&buffer[0]);
				break;
			case GL_DOUBLE_MAT2:
				glUniformMatrix2dv(uniformLocation, count, transpose, (GLdouble*)&buffer[0]);
				break;
			case GL_DOUBLE_MAT3:
				glUniformMatrix3dv(uniformLocation, count, transpose, (GLdouble*)&buffer[0]);
				break;
			case GL_DOUBLE_MAT4:
				glUniformMatrix4dv(uniformLocation, count, transpose, (GLdouble*)&buffer[0]);
				break;
			case GL_DOUBLE_MAT2x3:
				glUniformMatrix2x3dv(uniformLocation, count, transpose, (GLdouble*)&buffer[0]);
				break;
			case GL_DOUBLE_MAT2x4:
				glUniformMatrix2x4dv(uniformLocation, count, transpose, (GLdouble*)&buffer[0]);
				break;
			case GL_DOUBLE_MAT3x2:
				glUniformMatrix3x2dv(uniformLocation, count, transpose, (GLdouble*)&buffer[0]);
				break;
			case GL_DOUBLE_MAT3x4:
				glUniformMatrix3x4dv(uniformLocation, count, transpose, (GLdouble*)&buffer[0]);
				break;
			case GL_DOUBLE_MAT4x2:
				glUniformMatrix4x2dv(uniformLocation, count, transpose, (GLdouble*)&buffer[0]);
				break;
			case GL_DOUBLE_MAT4x3:
				glUniformMatrix4x3dv(uniformLocation, count, transpose, (GLdouble*)&buffer[0]);
				break;
			}
		}

		inline void changeType(GLenum newType, GLint newCount, GLboolean newTranspose, const GLubyte* newData) {
			if (newData == NULL)
				return;
			if (type != newType) {
				location = -1;
			}
			type = newType;
			baseType = FxGetBaseType(newType);
			components = FxGetTypeComponents(newType);
			sizeOfType = FxGetSizeOfType(newType);
			sizeInBytes = newCount * sizeOfType;
			count = newCount;
			transpose = newTranspose;
			buffer.resize(sizeInBytes, 0);
			memcpy(&buffer[0], newData, sizeInBytes);
		}

		inline void Uniform1fv(GLenum newType, int newCount, const GLfloat* values) { changeType(newType, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform1fv(int newCount, const GLfloat* values) { changeType(GL_FLOAT, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform2fv(int newCount, const GLfloat* values) { changeType(GL_FLOAT_VEC2, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform3fv(int newCount, const GLfloat* values) { changeType(GL_FLOAT_VEC3, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform4fv(int newCount, const GLfloat* values) { changeType(GL_FLOAT_VEC4, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform1iv(int newCount, const GLint* values) { changeType(GL_INT, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform2iv(int newCount, const GLint* values) { changeType(GL_INT_VEC2, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform3iv(int newCount, const GLint* values) { changeType(GL_INT_VEC3, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform4iv(int newCount, const GLint* values) { changeType(GL_INT_VEC4, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform1uiv(int newCount, const GLuint* values) { changeType(GL_UNSIGNED_INT, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform2uiv(int newCount, const GLuint* values) { changeType(GL_UNSIGNED_INT_VEC2, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform3uiv(int newCount, const GLuint* values) { changeType(GL_UNSIGNED_INT_VEC3, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform4uiv(int newCount, const GLuint* values) { changeType(GL_UNSIGNED_INT_VEC4, newCount, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }

		inline void Uniform1fv(const GLfloat* values) { changeType(GL_FLOAT, 1, false, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform2fv(const GLfloat* values) { changeType(GL_FLOAT_VEC2, 1, false, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform3fv(const GLfloat* values) { changeType(GL_FLOAT_VEC3, 1, false, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform4fv(const GLfloat* values) { changeType(GL_FLOAT_VEC4, 1, false, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform1iv(const GLint* values) { changeType(GL_INT, 1, false, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform2iv(const GLint* values) { changeType(GL_INT_VEC2, 1, false, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform3iv(const GLint* values) { changeType(GL_INT_VEC3, 1, false, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform4iv(const GLint* values) { changeType(GL_INT_VEC4, 1, false, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform1uiv(const GLuint* values) { changeType(GL_UNSIGNED_INT, 1, false, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform2uiv(const GLuint* values) { changeType(GL_UNSIGNED_INT_VEC2, 1, false, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform3uiv(const GLuint* values) { changeType(GL_UNSIGNED_INT_VEC3, 1, false, reinterpret_cast<const GLubyte*>(values)); }
		inline void Uniform4uiv(const GLuint* values) { changeType(GL_UNSIGNED_INT_VEC4, 1, false, reinterpret_cast<const GLubyte*>(values)); }

		inline void UniformMatrix2fv(int newCount, GLboolean newTranspose, const GLfloat* values) { changeType(GL_FLOAT_MAT2, newCount, newTranspose, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix3fv(int newCount, GLboolean newTranspose, const GLfloat* values) { changeType(GL_FLOAT_MAT3, newCount, newTranspose, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix4fv(int newCount, GLboolean newTranspose, const GLfloat* values) { changeType(GL_FLOAT_MAT4, newCount, newTranspose, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix2x3fv(int newCount, GLboolean newTranspose, const GLfloat* values) { changeType(GL_FLOAT_MAT2x3, newCount, newTranspose, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix2x4fv(int newCount, GLboolean newTranspose, const GLfloat* values) { changeType(GL_FLOAT_MAT2x4, newCount, newTranspose, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix3x2fv(int newCount, GLboolean newTranspose, const GLfloat* values) { changeType(GL_FLOAT_MAT3x2, newCount, newTranspose, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix3x4fv(int newCount, GLboolean newTranspose, const GLfloat* values) { changeType(GL_FLOAT_MAT3x4, newCount, newTranspose, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix4x2fv(int newCount, GLboolean newTranspose, const GLfloat* values) { changeType(GL_FLOAT_MAT4x2, newCount, newTranspose, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix4x3fv(int newCount, GLboolean newTranspose, const GLfloat* values) { changeType(GL_FLOAT_MAT4x3, newCount, newTranspose, reinterpret_cast<const GLubyte*>(values)); }

		inline void UniformMatrix2fv(const GLfloat* values) { changeType(GL_FLOAT_MAT2, 1, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix3fv(const GLfloat* values) { changeType(GL_FLOAT_MAT3, 1, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix4fv(const GLfloat* values) { changeType(GL_FLOAT_MAT4, 1, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix2x3fv(const GLfloat* values) { changeType(GL_FLOAT_MAT2x3, 1, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix2x4fv(const GLfloat* values) { changeType(GL_FLOAT_MAT2x4, 1, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix3x2fv(const GLfloat* values) { changeType(GL_FLOAT_MAT3x2, 1, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix3x4fv(const GLfloat* values) { changeType(GL_FLOAT_MAT3x4, 1, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix4x2fv(const GLfloat* values) { changeType(GL_FLOAT_MAT4x2, 1, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformMatrix4x3fv(const GLfloat* values) { changeType(GL_FLOAT_MAT4x3, 1, GL_FALSE, reinterpret_cast<const GLubyte*>(values)); }

		inline void UniformTransposeMatrix2fv(const GLfloat* values) { changeType(GL_FLOAT_MAT2, 1, GL_TRUE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformTransposeMatrix3fv(const GLfloat* values) { changeType(GL_FLOAT_MAT3, 1, GL_TRUE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformTransposeMatrix4fv(const GLfloat* values) { changeType(GL_FLOAT_MAT4, 1, GL_TRUE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformTransposeMatrix2x3fv(const GLfloat* values) { changeType(GL_FLOAT_MAT2x3, 1, GL_TRUE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformTransposeMatrix2x4fv(const GLfloat* values) { changeType(GL_FLOAT_MAT2x4, 1, GL_TRUE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformTransposeMatrix3x2fv(const GLfloat* values) { changeType(GL_FLOAT_MAT3x2, 1, GL_TRUE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformTransposeMatrix3x4fv(const GLfloat* values) { changeType(GL_FLOAT_MAT3x4, 1, GL_TRUE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformTransposeMatrix4x2fv(const GLfloat* values) { changeType(GL_FLOAT_MAT4x2, 1, GL_TRUE, reinterpret_cast<const GLubyte*>(values)); }
		inline void UniformTransposeMatrix4x3fv(const GLfloat* values) { changeType(GL_FLOAT_MAT4x3, 1, GL_TRUE, reinterpret_cast<const GLubyte*>(values)); }

		template <typename T>
		T* getAs() {
			if (buffer.empty())
				return NULL;
			return reinterpret_cast<T*>(&buffer[0]);
		}
	}; // class RendererUniform

	using RendererUniformMap = std::map<std::string, RendererUniform>;
} // namespace Fluxions

#endif
