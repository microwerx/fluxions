#include "fluxions_pch.hpp"
#include <hatchetfish.hpp>
#include <fluxions_gte.hpp>
#include <fluxions_gte_scalar_math.hpp>
#include <fluxions_gl1gl2_tools.hpp>

GLint g_MaxCombinedTextureUnits = 0;
std::string g_CurrentDebugMessage;

struct GLTypeInfo {
	GLenum type;
	GLenum baseType;
	GLint components;
	GLsizei sizeOfType;
};

GLTypeInfo glutTypeInfo[] = {
	{GL_BYTE, GL_BYTE, 1, sizeof(GLbyte)},
	{GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, 1, sizeof(GLubyte)},
	{GL_SHORT, GL_SHORT, 1, sizeof(GLshort)},
	{GL_UNSIGNED_SHORT, GL_UNSIGNED_SHORT, 1, sizeof(GLushort)},
	{GL_INT, GL_INT, 1, sizeof(GLint)},
	{GL_UNSIGNED_INT, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_BOOL, GL_INT, 1, sizeof(GLint)},
	{GL_FLOAT, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_DOUBLE, GL_DOUBLE, 1, sizeof(GLdouble)},
	{GL_FLOAT_VEC2, GL_FLOAT, 2, sizeof(GLfloat)},
	{GL_FLOAT_VEC3, GL_FLOAT, 3, sizeof(GLfloat)},
	{GL_FLOAT_VEC4, GL_FLOAT, 4, sizeof(GLfloat)},
	{GL_FLOAT_MAT2, GL_FLOAT, 4, sizeof(GLfloat)},
	{GL_FLOAT_MAT3, GL_FLOAT, 9, sizeof(GLfloat)},
	{GL_FLOAT_MAT4, GL_FLOAT, 16, sizeof(GLfloat)},
	{GL_FLOAT_MAT2x3, GL_FLOAT, 6, sizeof(GLfloat)},
	{GL_FLOAT_MAT2x4, GL_FLOAT, 8, sizeof(GLfloat)},
	{GL_FLOAT_MAT3x2, GL_FLOAT, 6, sizeof(GLfloat)},
	{GL_FLOAT_MAT3x4, GL_FLOAT, 12, sizeof(GLfloat)},
	{GL_FLOAT_MAT4x2, GL_FLOAT, 8, sizeof(GLfloat)},
	{GL_FLOAT_MAT4x3, GL_FLOAT, 12, sizeof(GLfloat)},
	{GL_SAMPLER_1D, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_SAMPLER_2D, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_SAMPLER_3D, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_SAMPLER_CUBE, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_SAMPLER_1D_SHADOW, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_SAMPLER_2D_SHADOW, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_SAMPLER_2D_MULTISAMPLE, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_SAMPLER_2D_MULTISAMPLE_ARRAY, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_SAMPLER_1D_ARRAY, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_SAMPLER_2D_ARRAY, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_SAMPLER_1D_ARRAY_SHADOW, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_SAMPLER_2D_ARRAY_SHADOW, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_DOUBLE_VEC2, GL_DOUBLE, 2, sizeof(GLdouble)},
	{GL_DOUBLE_VEC3, GL_DOUBLE, 3, sizeof(GLdouble)},
	{GL_DOUBLE_VEC4, GL_DOUBLE, 4, sizeof(GLdouble)},
	{GL_DOUBLE_MAT2, GL_DOUBLE, 4, sizeof(GLdouble)},
	{GL_DOUBLE_MAT3, GL_DOUBLE, 9, sizeof(GLdouble)},
	{GL_DOUBLE_MAT4, GL_DOUBLE, 16, sizeof(GLdouble)},
	{GL_DOUBLE_MAT2x3, GL_DOUBLE, 6, sizeof(GLdouble)},
	{GL_DOUBLE_MAT2x4, GL_DOUBLE, 8, sizeof(GLdouble)},
	{GL_DOUBLE_MAT3x2, GL_DOUBLE, 6, sizeof(GLdouble)},
	{GL_DOUBLE_MAT3x4, GL_DOUBLE, 12, sizeof(GLdouble)},
	{GL_DOUBLE_MAT4x2, GL_DOUBLE, 8, sizeof(GLdouble)},
	{GL_DOUBLE_MAT4x3, GL_DOUBLE, 12, sizeof(GLdouble)},
	{GL_INT_SAMPLER_1D, GL_INT, 1, sizeof(GLint)},
	{GL_INT_SAMPLER_2D, GL_INT, 1, sizeof(GLint)},
	{GL_INT_SAMPLER_3D, GL_INT, 1, sizeof(GLint)},
	{GL_INT_SAMPLER_CUBE, GL_INT, 1, sizeof(GLint)},
	//{ GL_INT_SAMPLER_1D_SHADOW, GL_INT, 1, sizeof(GLint) },
	//{ GL_INT_SAMPLER_2D_SHADOW, GL_INT, 1, sizeof(GLint) },
	{GL_INT_SAMPLER_2D_MULTISAMPLE, GL_INT, 1, sizeof(GLint)},
	{GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, GL_INT, 1, sizeof(GLint)},
	{GL_INT_SAMPLER_1D_ARRAY, GL_INT, 1, sizeof(GLint)},
	{GL_INT_SAMPLER_2D_ARRAY, GL_INT, 1, sizeof(GLint)},
	//{ GL_INT_SAMPLER_1D_ARRAY_SHADOW, GL_INT, 1, sizeof(GLint) },
	//{ GL_UNSIGNED_INT_SAMPLER_2D_ARRAY_SHADOW, GL_INT, 1, sizeof(GLint) },
	{GL_UNSIGNED_INT_SAMPLER_1D, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_SAMPLER_2D, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_SAMPLER_3D, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_SAMPLER_CUBE, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	//{ GL_UNSIGNED_INT_SAMPLER_1D_SHADOW, GL_UNSIGNED_INT, 1, sizeof(GLuint) },
	//{ GL_UNSIGNED_INT_SAMPLER_2D_SHADOW, GL_UNSIGNED_INT, 1, sizeof(GLuint) },
	{GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_SAMPLER_1D_ARRAY, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_SAMPLER_2D_ARRAY, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	//{ GL_UNSIGNED_INT_SAMPLER_1D_ARRAY_SHADOW, GL_UNSIGNED_INT, 1, sizeof(GLuint) },
	//{ GL_UNSIGNED_INT_SAMPLER_2D_ARRAY_SHADOW, GL_UNSIGNED_INT, 1, sizeof(GLuint) },
	{GL_IMAGE_1D, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_IMAGE_2D, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_IMAGE_3D, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_IMAGE_2D_RECT, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_IMAGE_CUBE, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_IMAGE_BUFFER, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_IMAGE_1D_ARRAY, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_IMAGE_2D_ARRAY, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_IMAGE_2D_MULTISAMPLE, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_IMAGE_2D_MULTISAMPLE_ARRAY, GL_FLOAT, 1, sizeof(GLfloat)},
	{GL_INT_IMAGE_1D, GL_INT, 1, sizeof(GLint)},
	{GL_INT_IMAGE_2D, GL_INT, 1, sizeof(GLint)},
	{GL_INT_IMAGE_3D, GL_INT, 1, sizeof(GLint)},
	{GL_INT_IMAGE_2D_RECT, GL_INT, 1, sizeof(GLint)},
	{GL_INT_IMAGE_CUBE, GL_INT, 1, sizeof(GLint)},
	{GL_INT_IMAGE_BUFFER, GL_INT, 1, sizeof(GLint)},
	{GL_INT_IMAGE_1D_ARRAY, GL_INT, 1, sizeof(GLint)},
	{GL_INT_IMAGE_2D_ARRAY, GL_INT, 1, sizeof(GLint)},
	{GL_INT_IMAGE_2D_MULTISAMPLE, GL_INT, 1, sizeof(GLint)},
	{GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY, GL_INT, 1, sizeof(GLint)},
	{GL_UNSIGNED_INT_IMAGE_1D, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_IMAGE_2D, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_IMAGE_3D, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_IMAGE_2D_RECT, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_IMAGE_CUBE, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_IMAGE_BUFFER, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_IMAGE_1D_ARRAY, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_IMAGE_2D_ARRAY, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{GL_UNSIGNED_INT_ATOMIC_COUNTER, GL_UNSIGNED_INT, 1, sizeof(GLuint)},
	{0xffff, 0, 0, 0} };

void FxSetErrorMessage(const char* filename, int line, const char* format, ...) {
	std::ostringstream ostr;
	char buffer[4096];
	va_list args;
	va_start(args, format);
#ifdef __STDC_SECURE_LIB__
	int numBytes = vsnprintf_s(buffer, 4096, 4096, format, args);
#else
	int numBytes = vsnprintf(buffer, 4096, format, args);
#endif
	va_end(args);
	if (numBytes < 0) {
		g_CurrentDebugMessage = "<blank>";
	}
	else if (numBytes < 4096)
		buffer[numBytes] = '\0';
	ostr << "File (" << filename << "), Line (" << line << "): \"" << buffer << "\"";
	g_CurrentDebugMessage = ostr.str();
}

void FxSetDefaultErrorMessage() {
	FxSetErrorMessage("<unknown>", 0, "<nothing set for debugging>");
}

GLenum FxGetBaseType(GLenum type) {
	int i = 0;
	while (glutTypeInfo[i].type != 0xffff) {
		if (type == glutTypeInfo[i].type) {
			return glutTypeInfo[i].baseType;
		}
		i++;
	}
	return 0;
}

GLint FxGetTypeComponents(GLenum type) {
	int i = 0;
	while (glutTypeInfo[i].type != 0xffff) {
		if (type == glutTypeInfo[i].type) {
			return glutTypeInfo[i].components;
		}
		i++;
	}
	return 0;
}

GLsizei FxGetSizeOfType(GLenum type) {
	int i = 0;
	while (glutTypeInfo[i].type != 0xffff) {
		if (type == glutTypeInfo[i].type) {
			return glutTypeInfo[i].components * glutTypeInfo[i].sizeOfType;
		}
		i++;
	}
	return 0;
}

GLsizei FxGetSizeOfBaseType(GLenum type) {
	int i = 0;
	while (glutTypeInfo[i].type != 0xffff) {
		if (type == glutTypeInfo[i].type) {
			return glutTypeInfo[i].sizeOfType;
		}
		i++;
	}
	return 0;
}

const char* FxGetFramebufferStatusAsString(GLenum status) {
#define IFTOSTRING(thing, value) \
	if ((thing) == (value))      \
		return (#value);

	IFTOSTRING(status, GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
	IFTOSTRING(status, GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER);
	IFTOSTRING(status, GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER);
	IFTOSTRING(status, GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
	IFTOSTRING(status, GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE);
	IFTOSTRING(status, GL_FRAMEBUFFER_COMPLETE);
	return "unknown status";

#undef IFTOSTRING
}


GLint FxSetActiveTexture(GLint unit) {
	if (g_MaxCombinedTextureUnits == 0) {
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &g_MaxCombinedTextureUnits);
	}
	unit = unit >= GL_TEXTURE0 ? unit - GL_TEXTURE0 : unit;

	if (unit < 0 || unit >= g_MaxCombinedTextureUnits)
		return -1;

	glActiveTexture(GL_TEXTURE0 + unit);
	return unit;
}

bool FxBindDefaultTextureAndSampler(GLenum target) {
	return FxBindTextureAndSampler(0, target, 0, 0);
}

bool FxBindTextureAndSampler(GLint unit, GLenum target, GLuint texture, GLuint sampler) {
	unit = FxSetActiveTexture(unit);
	if (unit < 0)
		return false;

	glBindTexture(target, texture);
	glBindSampler(unit, sampler);
	return true;
}

bool FxBindTexture(GLint unit, GLenum target, GLuint texture) {
	unit = FxSetActiveTexture(unit);
	if (unit < 0)
		return false;

	glBindTexture(target, texture);
	return true;
}

bool FxBindSampler(GLint unit, GLuint sampler) {
	unit = FxSetActiveTexture(unit);
	if (unit < 0)
		return false;

	glBindSampler(unit, sampler);
	return true;
}

bool FxDebugBindTexture(GLenum target, GLuint texture) {
	while (glGetError() != GL_NO_ERROR);
	glBindTexture(target, texture);
	while (glGetError() != GL_NO_ERROR) {
		GLint id1, id2;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &id1);
		glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &id2);
		GLint id3 = glIsTexture(texture);
		HFLOGERROR("could not bind %d to %s [2D: %d, CM: %d, is: %d]", texture, Fluxions::glNameTranslator.getString(target), id1, id2, id3);
		return false;
	}

	return true;
}

bool FxCreateBuffer(GLenum target, unsigned* p, GLsizeiptr size, const void* data, unsigned usage) {
	if (!*p) {
		glGenBuffers(1, p);
		HFLOGDEBUG("buffer %d created", *p);
	}
	if (*p) {
		glBindBuffer(target, *p);
		if (data != nullptr) glBufferData(target, size, data, usage);
	}
	return *p != 0;
}

void FxDeleteBuffer(GLuint* p) {
	if (*p == 0) return;
	glDeleteBuffers(1, p);
	HFLOGDEBUG("buffer %d deleted", *p);
	*p = 0;
}

bool FxCreateProgram(GLuint* p) {
	if (*p) FxDeleteProgram(p);
	if (!*p) {
		*p = glCreateProgram();
		HFLOGDEBUG("program %d created", *p);
	}
	return *p != 0;
}

void FxDeleteProgram(GLuint* p) {
	if (*p == 0) return;
	glDeleteProgram(*p);
	HFLOGDEBUG("program %d deleted", *p);
	*p = 0;
}

bool FxCreateShader(GLenum shaderType, GLuint* p) {
	if (*p) FxDeleteShader(p);
	if (!*p) {
		*p = glCreateShader(shaderType);
		HFLOGDEBUG("shader %d created", *p);
	}
	return *p != 0;
}

void FxDeleteShader(GLuint* p) {
	if (*p == 0) return;
	glDeleteShader(*p);
	HFLOGDEBUG("shader %d deleted", *p);
	*p = 0;
}

bool FxCreateTexture(GLenum target, GLuint* p) {
	if (*p) FxDeleteTexture(p);
	if (!*p) {
		glGenTextures(1, p);
		glBindTexture(target, *p);
		HFLOGDEBUG("texture %d created", *p);
	}
	return *p != 0;
}

void FxDeleteTexture(GLuint* p) {
	if (*p == 0) return;
	glDeleteTextures(1, p);
	HFLOGDEBUG("texture %d deleted", *p);
	*p = 0;
}

bool FxCreateSampler(GLuint* p) {
	if (*p) FxDeleteSampler(p);
	if (!*p) {
		glGenSamplers(1, p);
		HFLOGDEBUG("sampler %d created", *p);
	}
	return *p != 0;
}

void FxDeleteSampler(GLuint* p) {
	if (*p == 0) return;
	glDeleteSamplers(1, p);
	HFLOGDEBUG("sampler %d deleted", *p);
	*p = 0;
}


bool FxCreateVertexArray(GLuint* p) {
	if (*p) FxDeleteVertexArray(p);
	glGenVertexArrays(1, p);
	HFLOGDEBUG("vao %d created", *p);
	glBindVertexArray(*p);
	return *p != 0;
}

void FxDeleteVertexArray(GLuint* p) {
	if (*p == 0) return;
	glDeleteVertexArrays(1, p);
	HFLOGDEBUG("vao %d deleted", *p);
	*p = 0;
}

bool FxCreateRenderbuffer(GLuint* p) {
	if (*p) {
		FxDeleteRenderbuffer(p);
	}
	glGenRenderbuffers(1, p);
	HFLOGDEBUG("renderbuffer %i created", *p);
	glBindRenderbuffer(GL_FRAMEBUFFER, *p);
	return *p != 0;
}

void FxDeleteRenderbuffer(GLuint* p) {
	if (*p == 0) return;
	HFLOGDEBUG("renderbuffer %i deleted", *p);
	glDeleteRenderbuffers(1, p);
	*p = 0;
}

bool FxCreateFramebuffer(GLuint* p) {
	if (*p) {
		FxDeleteFramebuffer(p);
	}
	glGenFramebuffers(1, p);
	HFLOGDEBUG("framebuffer %i created", *p);
	glBindFramebuffer(GL_FRAMEBUFFER, *p);
	return *p != 0;
}

void FxDeleteFramebuffer(GLuint* p) {
	if (*p == 0) return;
	HFLOGDEBUG("framebuffer %i deleted", *p);
	glDeleteFramebuffers(1, p);
	*p = 0;
}

GLuint FxCheckFramebufferStatus() {
	GLuint fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (fbo_status != GL_FRAMEBUFFER_COMPLETE) {
		std::string msg = FxGetFramebufferStatusAsString(fbo_status);
		HFLOGERROR("Framebuffer is not complete! --> %s", msg.c_str());
	}

	return fbo_status;
}

void FxClearScreenRgb(GLfloat r, GLfloat g, GLfloat b) {
	glClearColor(r, g, b, 1.0f);
	glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

FxSaveGraphicsState::FxSaveGraphicsState() {
	glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
	glActiveTexture(GL_TEXTURE0);
	glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
	glGetIntegerv(GL_BLEND_SRC_RGB, &last_blend_src_rgb);
	glGetIntegerv(GL_BLEND_DST_RGB, &last_blend_dst_rgb);
	glGetIntegerv(GL_BLEND_SRC_ALPHA, &last_blend_src_alpha);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &last_blend_dst_alpha);
	glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
	glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
	glGetIntegerv(GL_VIEWPORT, last_viewport);
	glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
	GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
	GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
	GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
}

FxSaveGraphicsState::~FxSaveGraphicsState() {
	glUseProgram(last_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glActiveTexture(last_active_texture);
	glBindVertexArray(last_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
	glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
	glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
	if (last_enable_blend)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
	if (last_enable_cull_face)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	if (last_enable_depth_test)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	if (last_enable_scissor_test)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

void FxGlutTestLitSolidTeapotScene(double fovy, double aspect) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	using Fluxions::Matrix4f;
	// gluPerspective(fovy, aspect, 0.01, 100.0);
	Matrix4f perspective;
	perspective.PerspectiveY((float)fovy, (float)aspect, 0.01f, 100.0f);
	glMultMatrixf(perspective.const_ptr());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	using Fluxions::Matrix4f;
	using Fluxions::Vector3f;
	Matrix4f lookAt;
	lookAt.LookAt(Vector3f(0.0f, 0.0f, 10.0f),
				  Vector3f(0.0f, 0.0f, 0.0f),
				  Vector3f(0.0f, 1.0f, 0.0f));
	glMultMatrixf(lookAt.const_ptr());

#ifdef FLUXIONS_USE_FREEGLUT
	glutSolidTeapot(1.0);
#endif

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
}

void FxGlutBitmapString(void* font, const char* str) {
	if (!font) return;
	int len = (int)strlen(str);
	int i = 0;

	for (i = 0; i < len; i++) {
#ifdef FLUXIONS_USE_FREEGLUT
		glutBitmapCharacter(font, str[i]);
#endif
	}
}

void FxGlutStrokeString(void* font, const char* str) {
	if (!font) return;
	int len = (int)strlen(str);
	int i = 0;

	for (i = 0; i < len; i++) {
#ifdef FLUXIONS_USE_FREEGLUT
		glutStrokeCharacter(font, str[i]);
#endif
	}
}

void FxGlutPrintString9x15(double x, double y, double screenWidth, int justification, const char* format, ...) {
	char buffer[256];

	va_list va;
	va_start(va, format);
#ifdef __STDC_SECURE_LIB__
	vsnprintf_s(buffer, sizeof(buffer), 200, format, va);
#else
	vsnprintf(buffer, 100, format, va);
#endif
	va_end(va);

	FxGlutPrintBitmapStringJustified(x, y, screenWidth, justification, (void*)(FX_GLUT_FONT::BITMAP_9_BY_15), buffer);
}

void FxGlutPrintBitmapStringJustified(double x, double y, double screenWidth, int justification, void* font, const char* format, ...) {
	char buffer[256];
	int pixelWidthOfString = 0;

	va_list va;
	va_start(va, format);
#ifdef __STDC_SECURE_LIB__
	vsnprintf_s(buffer, sizeof(buffer), 100, format, va);
#else
	vsnprintf(buffer, 100, format, va);
#endif
	va_end(va);

#ifdef FLUXIONS_USE_FREEGLUT
	pixelWidthOfString = glutBitmapLength(font, (const unsigned char*)buffer);
#endif

	if (justification == LEFT) {
		// left justified
		glRasterPos2d(x, y);
	}
	else if (justification == RIGHT) {
		// right justified
		glRasterPos2d((screenWidth - pixelWidthOfString), y);
	}
	else if (justification == CENTER) {
		glRasterPos2d((screenWidth - pixelWidthOfString) / 2.0f, y);
	}

	FxGlutBitmapString(font, buffer);
}

void FxGlutPrintStrokeStringJustified(double x, double y, double screenWidth, int justification, void* font, const char* format, ...) {
	char buffer[2048];
	int pixelWidthOfString = 0;

	va_list va;
	va_start(va, format);
#ifdef __STDC_SECURE_LIB__
	vsnprintf_s(buffer, sizeof(buffer), 2048, format, va);
#else
	vsnprintf(buffer, 2048, format, va);
#endif
	va_end(va);

#ifdef FLUXIONS_USE_FREEGLUT
	pixelWidthOfString = glutStrokeLength(font, (const unsigned char*)buffer);
#endif

	if (justification == LEFT) {
		// left justified
		glRasterPos2d(x, y);
	}
	else if (justification == RIGHT) {
		// right justified
		glRasterPos2d((screenWidth - pixelWidthOfString), y);
	}
	else if (justification == CENTER) {
		glRasterPos2d((screenWidth - pixelWidthOfString) / 2.0f, y);
	}

	FxGlutStrokeString(font, buffer);
}

void FxDrawGL1ThreeAxis(float length) {
	GLfloat v[36] = {
		0.0f, 0.0f, 0.0f,
		length, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, length, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, length,
		0.0f, 0.0f, 0.0f,
		-length, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -length, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -length };

	GLfloat colors[36] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f };

	glVertexPointer(3, GL_FLOAT, 0, v);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_LINES, 0, 6);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, 0);
}

void FxDrawGL1Camera(double fov, float r, float g, float b, float zfar, const float* inverseCameraMatrix, const float* projectionMatrix) {
	glPushMatrix(); // for camera viz
	glMultMatrixf(inverseCameraMatrix);

	FxDrawGL1SixAxis(1.0);

	//glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

	glColor4f(r, g, b, 1.0f);
	glPushMatrix();
	FxDrawGL1WireFrustumf(projectionMatrix, 3.14159f / zfar);
	glPopMatrix();

	glColor4f(r, g, b, 1.0f);
	glPushMatrix(); // for cone
	glTranslatef(0.0f, 0.0f, -1.0f);
	float angle = (float)(fov * FX_DEGREES_TO_RADIANS * 0.5);
	FxDrawGL1WireCone(sinf(angle), 1.0f, 32, 2);
	glPopMatrix(); // for cone

	glPushMatrix(); // for wire cube
	glTranslatef(0.0f, 0.0f, 0.5f);
	FxDrawGL1WireCube(1.0);
	glPopMatrix(); // for wire cube

	glPopMatrix(); // for camera viz
}

void FxDrawGL1SixAxis(float length) {
	GLfloat v[36] = {
		0.0f, 0.0f, 0.0f,
		length, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, length, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, length,
		0.0f, 0.0f, 0.0f,
		-length, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -length, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -length };

	GLfloat colors[36] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f };

	glVertexPointer(3, GL_FLOAT, 0, v);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_LINES, 0, 12);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, 0);
}

void FxDrawGL1WireFrustumd(const double* m) {
	float f32m[16];
	for (int i = 0; i < 16; i++) {
		f32m[i] = (float)m[i];
	}
	FxDrawGL1WireFrustumf(f32m);
}

struct Matrix {
	float m11, m21, m31, m41;
	float m12, m22, m32, m42;
	float m13, m23, m33, m43;
	float m14, m24, m34, m44;
};

struct Vector {
	float x, y, z, w;

	Vector scale(float amount) const {
		Vector out;
		out.x = x * amount;
		out.y = y * amount;
		out.z = z * amount;
		out.w = w * amount;
		return out;
	}

	Vector plus(Vector v) const {
		Vector out;
		out.x = x + v.x;
		out.y = y + v.y;
		out.z = z + v.z;
		out.w = w + v.w;
		return out;
	}

	Vector cross3(const Vector& V) const {
		Vector out;
		out.x = y * V.z - z * V.y;
		out.y = z * V.x - x * V.z;
		out.z = x * V.y - y * V.x;
		out.w = 0.0f;
		return out;
	}

	float dot3(const Vector& V) const {
		return x * V.x + y * V.y + z * V.z;
	}

	Vector norm3() const {
		Vector out;
		float invlen = 1.0f / sqrtf(x * x + y * y + z * z);
		out.x = x * invlen;
		out.y = y * invlen;
		out.z = z * invlen;
		out.w = 0.0f;
		return out;
	}
};

struct Plane {
	float a, b, c, d;

	void normalize() {
		float invlen = 1.0f / sqrtf(a * a + b * b + c * c);
		a *= invlen;
		b *= invlen;
		c *= invlen;
		d *= invlen;
	}

	Vector axis() {
		Vector out;
		out.x = a;
		out.y = b;
		out.z = c;
		out.w = 0.0f;
		return out;
	}
};

Vector ThreePlaneIntersection(Plane A, Plane B, Plane C) {
	Vector out;
	out.x = 0.0f;
	out.y = 0.0f;
	out.z = 0.0f;
	out.w = 1.0f;

	A.normalize();
	B.normalize();
	C.normalize();

	Vector a, b, c;
	a = A.axis();
	b = B.axis();
	c = C.axis();
	float mag = a.dot3(b.cross3(c));
	//if (fabs(mag) < 0.0001) {
	//	// coplanar somewhere
	//	return out;
	//}
	if (!isnormal(mag) || mag == 0.0f) {
		return out;
	}

	Vector v1 = b.cross3(c).scale(-A.d);
	Vector v2 = c.cross3(a).scale(-B.d);
	Vector v3 = a.cross3(b).scale(-C.d);
	out = (v1.plus(v2).plus(v3)).scale(1.0f / mag);

	return out;
}

void FxDrawGL1WireFrustumf(const float* m, float scale) {
	Matrix M;
	memcpy(&M, m, sizeof(float) * 16);

	Plane p[6];
	// Left clipping plane
	p[0].a = M.m41 + M.m11;
	p[0].b = M.m42 + M.m12;
	p[0].c = M.m43 + M.m13;
	p[0].d = M.m44 + M.m14;

	// Right clipping plane
	p[1].a = M.m41 - M.m11;
	p[1].b = M.m42 - M.m12;
	p[1].c = M.m43 - M.m13;
	p[1].d = M.m44 - M.m14;

	// Top clipping plane
	p[2].a = M.m41 - M.m21;
	p[2].b = M.m42 - M.m22;
	p[2].c = M.m43 - M.m23;
	p[2].d = M.m44 - M.m24;

	// Bottom clipping plane
	p[3].a = M.m41 + M.m21;
	p[3].b = M.m42 + M.m22;
	p[3].c = M.m43 + M.m23;
	p[3].d = M.m44 + M.m24;

	// Near clipping plane
	p[4].a = M.m41 + M.m31;
	p[4].b = M.m42 + M.m32;
	p[4].c = M.m43 + M.m33;
	p[4].d = M.m44 + M.m34;

	// Far clipping plane
	p[5].a = M.m41 - M.m31;
	p[5].b = M.m42 - M.m32;
	p[5].c = M.m43 - M.m33;
	p[5].d = M.m44 - M.m34;

	// Left, Bottom, Near
	Vector P1 = ThreePlaneIntersection(p[0], p[3], p[4]);
	// Left, Top, Near
	Vector P2 = ThreePlaneIntersection(p[0], p[2], p[4]);
	// Right, Top, Near
	Vector P3 = ThreePlaneIntersection(p[1], p[2], p[4]);
	// Right, Bottom, Near
	Vector P4 = ThreePlaneIntersection(p[1], p[3], p[4]);
	// Left, Bottom, Far
	Vector P5 = ThreePlaneIntersection(p[0], p[3], p[5]);
	// Left, Top, Far
	Vector P6 = ThreePlaneIntersection(p[0], p[2], p[5]);
	// Right, Top, Far
	Vector P7 = ThreePlaneIntersection(p[1], p[2], p[5]);
	// Right, Bottom, Far
	Vector P8 = ThreePlaneIntersection(p[1], p[3], p[5]);

	float nscale = scale;
	float fscale = scale;
	float v[24] = {
		nscale * P1.x, nscale * P1.y, nscale * P1.z,
		nscale * P2.x, nscale * P2.y, nscale * P2.z,
		nscale * P3.x, nscale * P3.y, nscale * P3.z,
		nscale * P4.x, nscale * P4.y, nscale * P4.z,
		//P5.X, P5.y, P5.z,
		//P6.X, P6.y, P6.z,
		//P7.X, P7.y, P7.z,
		//P8.X, P8.y, P8.z,
		fscale * P5.x, fscale * P5.y, fscale * P5.z,
		fscale * P6.x, fscale * P6.y, fscale * P6.z,
		fscale * P7.x, fscale * P7.y, fscale * P7.z,
		fscale * P8.x, fscale * P8.y, fscale * P8.z };

	int indices[] = {
		// near
		0, 1, 2, 3,
		// far
		4, 5, 6, 7,
		// left
		1, 5, 4, 0,
		// right
		3, 7, 6, 2,
		// bottom
		3, 4, 7, 0,
		// top
		2, 6, 5, 1 };

	float colors[6][4] = {
		{1.0f, 1.0f, 0.0f, 0.1f},
		{0.0f, 0.0f, 1.0f, 0.1f},
		{0.0f, 1.0f, 1.0f, 0.1f},
		{1.0f, 0.0f, 0.0f, 0.1f},
		{1.0f, 0.0f, 1.0f, 0.1f},
		{0.0f, 1.0f, 0.0f, 0.1f} };

	//glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(4.0f);
	glVertexPointer(3, GL_FLOAT, 0, v);
	glEnableClientState(GL_VERTEX_ARRAY);
	for (int i = 0; i < 6; i++) {
		//glPointSize((i+1) * 2.0f);
		glColor4fv(colors[i]);
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, indices + i * 4);
		glDrawElements(GL_POINTS, 4, GL_UNSIGNED_INT, indices + i * 4);
		//glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, indices + i * 4);
	}
	//glDisable(GL_BLEND);
	//glDisable(GL_CULL_FACE);
	glPointSize(1.0f);
	//glDrawArrays(GL_LINE_LOOP, 0, 4);
	//glDrawArrays(GL_LINE_LOOP, 4, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);
}

void FxDrawGL1EnvironmentCube(GLfloat size, GLuint cubeMapTexId) {
	GLfloat v[] = {
		-size, size, -size,
		size, size, -size,
		size, -size, -size,
		-size, -size, -size,
		size, size, size,
		-size, size, size,
		-size, -size, size,
		size, -size, size };

	GLfloat texcoords[] = {
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f };

	//GLshort indices[] = {
	//	// FACE 0
	//	1,4,7,
	//	7,2,1,
	//	// FACE 1
	//	5,0,3,
	//	3,6,5,
	//	// FACE 2
	//	5,4,1,
	//	1,0,5,
	//	// FACE 3
	//	3,2,7,
	//	7,6,3,
	//	// FACE 5
	//	4,5,6,
	//	6,7,4,
	//	// FACE 4
	//	0,1,2,
	//	2,3,0
	//};

	GLshort indices[] = {
		// FACE 0
		7, 4, 1, //1,4,7,
		1, 2, 7, //7,2,1,
		// FACE 1
		3, 0, 5, //5,0,3,
		5, 6, 3, //3,6,5,
		// FACE 2
		1, 4, 5, //5,4,1,
		5, 0, 1, //1,0,5,
		// FACE 3
		7, 2, 3, //3,2,7,
		3, 6, 7, //7,6,3,
		// FACE 5
		6, 5, 4, //4,5,6,
		4, 7, 6, //6,7,4,
		// FACE 4
		2, 1, 0, //0,1,2,
		0, 3, 2  //2,3,0
	};

	glEnable(GL_CULL_FACE);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	FxDebugBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexId);
	glVertexPointer(3, GL_FLOAT, 0, v);
	glTexCoordPointer(3, GL_FLOAT, 0, texcoords);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(4, GL_FLOAT, 0, 0);
	glTexCoordPointer(4, GL_FLOAT, 0, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_CULL_FACE);
}

// FxDrawGL2UnwrappedCubeMap
// Renders an unwrapped cube map to a 2D screen
void FxDrawGL2UnwrappedCubeMap(GLuint x, GLuint y, GLuint size, GLint vloc, GLint tloc) {
	GLfloat xs0 = x + 0.0f;
	GLfloat xs1 = x + (GLfloat)size;
	GLfloat xs2 = x + size * 2.0f;
	GLfloat xs3 = x + size * 3.0f;
	GLfloat xs4 = x + size * 4.0f;
	GLfloat ys0 = y + 0.0f;
	GLfloat ys1 = y + (GLfloat)size;
	GLfloat ys2 = y + size * 2.0f;
	GLfloat ys3 = y + size * 3.0f;

	// GLfloat v[] = {
	//     xs0, ys1, 0.0f, // FACE 1 NEGATIVE X
	//     xs0, ys2, 0.0f,
	//     xs1, ys2, 0.0f,
	//     xs1, ys1, 0.0f,
	//     xs1, ys1, 0.0f, // FACE 4 POSITIVE Z
	//     xs1, ys2, 0.0f,
	//     xs2, ys2, 0.0f,
	//     xs2, ys1, 0.0f,
	//     xs2, ys1, 0.0f, // FACE 0 POSITIVE X
	//     xs2, ys2, 0.0f,
	//     xs3, ys2, 0.0f,
	//     xs3, ys1, 0.0f,
	//     xs3, ys1, 0.0f, // FACE 5 NEGATIVE Z
	//     xs3, ys2, 0.0f,
	//     xs4, ys2, 0.0f,
	//     xs4, ys1, 0.0f,
	//     xs1, ys2, 0.0f, // FACE 3 NEGATIVE Y
	//     xs1, ys3, 0.0f,
	//     xs2, ys3, 0.0f,
	//     xs2, ys2, 0.0f,
	//     xs1, ys0, 0.0f, // FACE 2 POSITIVE Y
	//     xs1, ys1, 0.0f,
	//     xs2, ys1, 0.0f,
	//     xs2, ys0, 0.0f};

	GLfloat x1 = -1.0f;
	GLfloat x2 = 1.0f;
	GLfloat y1 = -1.0f;
	GLfloat y2 = 1.0f;
	GLfloat z1 = -1.0f;
	GLfloat z2 = 1.0f;

	// GLfloat texcoords[] = {
	//     x1,
	//     y1,
	//     z1, // FACE 1 NEGATIVE X
	//     x1,
	//     y2,
	//     z1,
	//     x1,
	//     y2,
	//     z2,
	//     x1,
	//     y1,
	//     z2,
	//     x1,
	//     y1,
	//     z2, // FACE 4 POSITIVE Z
	//     x1,
	//     y2,
	//     z2,
	//     x2,
	//     y2,
	//     z2,
	//     x2,
	//     y1,
	//     z2,
	//     x2,
	//     y1,
	//     z2, // FACE 0 POSITIVE X
	//     x2,
	//     y2,
	//     z2,
	//     x2,
	//     y2,
	//     z1,
	//     x2,
	//     y1,
	//     z1,
	//     x2,
	//     y1,
	//     z1, // FACE 5 NEGATIVE Z
	//     x2,
	//     y2,
	//     z1,
	//     x1,
	//     y2,
	//     z1,
	//     x1,
	//     y1,
	//     z1,
	//     x1,
	//     y1,
	//     z1, // FACE 3 NEGATIVE Y
	//     x1,
	//     y1,
	//     z2,
	//     x2,
	//     y1,
	//     z2,
	//     x2,
	//     y1,
	//     z1,
	//     x1,
	//     y2,
	//     z2, // FACE 2 POSITIVE Y
	//     x1,
	//     y2,
	//     z1,
	//     x2,
	//     y2,
	//     z1,
	//     x2,
	//     y2,
	//     z2,
	// };

	GLfloat buffer[] = {
		xs0, ys1, 0.0f, x1, y1, z1, // FACE 1 NEGATIVE X
		xs0, ys2, 0.0f, x1, y2, z1,
		xs1, ys2, 0.0f, x1, y2, z2,
		xs1, ys1, 0.0f, x1, y1, z2,
		xs1, ys1, 0.0f, x1, y1, z2, // FACE 4 POSITIVE Z
		xs1, ys2, 0.0f, x1, y2, z2,
		xs2, ys2, 0.0f, x2, y2, z2,
		xs2, ys1, 0.0f, x2, y1, z2,
		xs2, ys1, 0.0f, x2, y1, z2, // FACE 0 POSITIVE X
		xs2, ys2, 0.0f, x2, y2, z2,
		xs3, ys2, 0.0f, x2, y2, z1,
		xs3, ys1, 0.0f, x2, y1, z1,
		xs3, ys1, 0.0f, x2, y1, z1, // FACE 5 NEGATIVE Z
		xs3, ys2, 0.0f, x2, y2, z1,
		xs4, ys2, 0.0f, x1, y2, z1,
		xs4, ys1, 0.0f, x1, y1, z1,
		xs1, ys0, 0.0f, x1, y1, z1, // FACE 2 NEGATIVE Y
		xs1, ys1, 0.0f, x1, y1, z2,
		xs2, ys1, 0.0f, x2, y1, z2,
		xs2, ys0, 0.0f, x2, y1, z1,
		xs1, ys2, 0.0f, x1, y2, z2, // FACE 3 POSITIVE Y
		xs1, ys3, 0.0f, x1, y2, z1,
		xs2, ys3, 0.0f, x2, y2, z1,
		xs2, ys2, 0.0f, x2, y2, z2,

		//xs1, ys2, 0.0f, x1, y1, z1,			// FACE 3 NEGATIVE Y
		//xs1, ys3, 0.0f, x1, y1, z2,
		//xs2, ys3, 0.0f, x2, y1, z2,
		//xs2, ys2, 0.0f, x2, y1, z1,
		//xs1, ys0, 0.0f, x1, y2, z2,			// FACE 2 POSITIVE Y
		//xs1, ys1, 0.0f, x1, y2, z1,
		//xs2, ys1, 0.0f, x2, y2, z1,
		//xs2, ys0, 0.0f, x2, y2, z2,

	};

	GLushort indices[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20 };

	static GLuint abo = 0;
	static GLuint eabo = 0;

	if (abo == 0) {
		FxCreateBuffer(GL_ARRAY_BUFFER, &abo, sizeof(buffer), buffer, GL_STATIC_DRAW);
		FxCreateBuffer(GL_ELEMENT_ARRAY_BUFFER, &eabo, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, abo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
	glVertexAttribPointer(vloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void*)0);
	glVertexAttribPointer(tloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void*)12);
	if (vloc >= 0)
		glEnableVertexAttribArray(vloc);
	if (tloc >= 0)
		glEnableVertexAttribArray(tloc);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	if (vloc >= 0)
		glDisableVertexAttribArray(vloc);
	if (tloc >= 0)
		glDisableVertexAttribArray(tloc);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

// FxDrawGL2CubeMap
// Renders an unwrapped cube map to a 2D screen
void FxDrawGL2CubeMap(GLdouble x, GLdouble y, GLdouble z, GLdouble size, GLint vloc, GLint tloc) {
	GLfloat x1 = (float)(x - size / 2.0);
	GLfloat y1 = (float)(y - size / 2.0);
	GLfloat z1 = (float)(z - size / 2.0);
	GLfloat x2 = (float)(x + size / 2.0);
	GLfloat y2 = (float)(y + size / 2.0);
	GLfloat z2 = (float)(z + size / 2.0);

	// GLfloat v[] = {
	//     x1, y1, z1, // FACE 1 NEGATIVE X
	//     x1, y1, z2,
	//     x1, y2, z2,
	//     x1, y2, z1,
	//     x1, y1, z2, // FACE 4 POSITIVE Z
	//     x2, y1, z2,
	//     x2, y2, z2,
	//     x1, y2, z2,
	//     x2, y1, z2, // FACE 0 POSITIVE X
	//     x2, y1, z1,
	//     x2, y2, z1,
	//     x2, y2, z2,
	//     x2, y1, z1, // FACE 5 NEGATIVE Z
	//     x1, y1, z1,
	//     x1, y2, z1,
	//     x2, y2, z1,
	//     x1, y1, z1, // FACE 3 NEGATIVE Y
	//     x2, y1, z1,
	//     x2, y1, z2,
	//     x1, y1, z2,
	//     x2, y2, z1, // FACE 2 POSITIVE Y
	//     x1, y2, z1,
	//     x1, y2, z2,
	//     x2, y2, z2};

	GLfloat tx1 = -1.0f;
	GLfloat tx2 = 1.0f;
	GLfloat ty1 = -1.0f;
	GLfloat ty2 = 1.0f;
	GLfloat tz1 = -1.0f;
	GLfloat tz2 = 1.0f;

	// GLfloat texcoords[] = {
	//     x1,
	//     y1,
	//     z1, // FACE 1 NEGATIVE X
	//     x1,
	//     y2,
	//     z1,
	//     x1,
	//     y2,
	//     z2,
	//     x1,
	//     y1,
	//     z2,
	//     x1,
	//     y1,
	//     z2, // FACE 4 POSITIVE Z
	//     x1,
	//     y2,
	//     z2,
	//     x2,
	//     y2,
	//     z2,
	//     x2,
	//     y1,
	//     z2,
	//     x2,
	//     y1,
	//     z2, // FACE 0 POSITIVE X
	//     x2,
	//     y2,
	//     z2,
	//     x2,
	//     y2,
	//     z1,
	//     x2,
	//     y1,
	//     z1,
	//     x2,
	//     y1,
	//     z1, // FACE 5 NEGATIVE Z
	//     x2,
	//     y2,
	//     z1,
	//     x1,
	//     y2,
	//     z1,
	//     x1,
	//     y1,
	//     z1,
	//     x1,
	//     y1,
	//     z1, // FACE 3 NEGATIVE Y
	//     x1,
	//     y1,
	//     z2,
	//     x2,
	//     y1,
	//     z2,
	//     x2,
	//     y1,
	//     z1,
	//     x1,
	//     y2,
	//     z2, // FACE 2 POSITIVE Y
	//     x1,
	//     y2,
	//     z1,
	//     x2,
	//     y2,
	//     z1,
	//     x2,
	//     y2,
	//     z2,
	// };

	GLfloat buffer[] = {
		x1, y1, z1, tx1, ty1, tz1, // FACE 1 NEGATIVE X
		x1, y2, z1, tx1, ty2, tz1,
		x1, y2, z2, tx1, ty2, tz2,
		x1, y1, z2, tx1, ty1, tz2,
		x1, y1, z2, tx1, ty1, tz2, // FACE 4 POSITIVE Z
		x1, y2, z2, tx1, ty2, tz2,
		x2, y2, z2, tx2, ty2, tz2,
		x2, y1, z2, tx2, ty1, tz2,
		x2, y1, z2, tx2, ty1, tz2, // FACE 0 POSITIVE X
		x2, y2, z2, tx2, ty2, tz2,
		x2, y2, z1, tx2, ty2, tz1,
		x2, y1, z1, tx2, ty1, tz1,
		x2, y1, z1, tx2, ty1, tz1, // FACE 5 NEGATIVE Z
		x2, y2, z1, tx2, ty2, tz1,
		x1, y2, z1, tx1, ty2, tz1,
		x1, y1, z1, tx1, ty1, tz1,
		x1, y1, z1, tx1, ty1, tz1, // FACE 2 NEGATIVE Y
		x1, y1, z2, tx1, ty1, tz2,
		x2, y1, z2, tx2, ty1, tz2,
		x2, y1, z1, tx2, ty1, tz1,
		x1, y2, z2, tx1, ty2, tz2, // FACE 3 POSITIVE Y
		x1, y2, z1, tx1, ty2, tz1,
		x2, y2, z1, tx2, ty2, tz1,
		x2, y2, z2, tx2, ty2, tz2 };

	GLushort indices[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20 };

	static GLuint abo = 0;
	static GLuint eabo = 0;

	if (abo == 0) {
		FxCreateBuffer(GL_ARRAY_BUFFER, &abo, sizeof(buffer), buffer, GL_STATIC_DRAW);
		FxCreateBuffer(GL_ELEMENT_ARRAY_BUFFER, &eabo, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	//glBindBuffer(GL_ARRAY_BUFFER, abo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
	//glVertexAttribPointer(vloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void *)0);
	//glVertexAttribPointer(tloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void *)12);
	const unsigned char* vptr = (unsigned char*)buffer;
	const unsigned char* tptr = vptr + 12;
	glVertexAttribPointer(vloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, vptr);
	glVertexAttribPointer(tloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, tptr);
	if (vloc >= 0)
		glEnableVertexAttribArray(vloc);
	if (tloc >= 0)
		glEnableVertexAttribArray(tloc);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices);
	if (vloc >= 0)
		glDisableVertexAttribArray(vloc);
	if (tloc >= 0)
		glDisableVertexAttribArray(tloc);
}

void FxDrawGL1WireSphere(float radius, unsigned slices, unsigned stacks) {
#ifdef FLUXIONS_USE_FREEGLUT
	glutWireSphere(radius, slices, stacks);
#endif
}

void FxDrawGL1WireCone(float base, float height, unsigned slices, unsigned stacks) {
#ifdef FLUXIONS_USE_FREEGLUT
	glutWireCone(base, height, slices, stacks);
#endif
}

void FxDrawGL1WireCube(float size) {
#ifdef FLUXIONS_USE_FREEGLUT
	glutWireCube(size);
#endif
}

void FxDrawGL1SolidCube(float size) {
#ifdef FLUXIONS_USE_FREEGLUT
	glutSolidCube(size);
#endif

}

void FxDrawGL1SolidSphere(float radius, unsigned slices, unsigned stacks) {
#ifdef FLUXIONS_USE_FREEGLUT
	glutSolidSphere(radius, slices, stacks);
#endif

}

void FxDrawGL1SolidCone(float base, float height, unsigned slices, unsigned stacks) {
#ifdef FLUXIONS_USE_FREEGLUT
	glutSolidCone(base, height, slices, stacks);
#endif

}

void FxDrawGL1SolidTeapot(float size) {
#ifdef FLUXIONS_USE_FREEGLUT
	glutSolidTeapot(size);
#endif
}

void FxDrawGL1Superquadric(float radius, unsigned slices, unsigned stacks, float n, float e) {
	float du = FX_F32_2_PI / slices;
	float dv = FX_F32_PI / stacks;
	float ds = 1.0f / slices;
	float dt = 1.0f / stacks;

	float t = 0.0f;
	float v = -FX_F32_PI_2;
	for (unsigned stack = 0; stack < stacks; stack++) {
		glBegin(GL_TRIANGLE_STRIP);
		float s = 0.0f;
		float u = -FX_F32_PI;
		for (unsigned slice = 0; slice <= slices; slice++) {
			float x, y, z;
			float nx, ny, nz;

			Fluxions::sqEllipsoid(radius, radius, radius, u, v, n, e, &x, &y, &z, &nx, &ny, &nz);
			glTexCoord2f(s, t);
			glNormal3f(nx, ny, nz);
			glVertex3f(x, y, z);

			Fluxions::sqEllipsoid(radius, radius, radius, u, v + dv, n, e, &x, &y, &z, &nx, &ny, &nz);
			glTexCoord2f(s, t);
			glNormal3f(nx, ny, nz);
			glVertex3f(x, y, z);

			s += ds;
			u += du;
		}
		glEnd();
		t += dt;
		v += dv;
	}
}
