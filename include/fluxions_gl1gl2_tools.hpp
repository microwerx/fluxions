// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2019 Jonathan Metzgar
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
#ifndef FLUXIONS_GL1GL2_TOOLS_HPP
#define FLUXIONS_GL1GL2_TOOLS_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_opengl.hpp>

GLenum FxGetBaseType(GLenum type);
GLint FxGetTypeComponents(GLenum type);
GLsizei FxGetSizeOfType(GLenum type);
GLsizei FxGetSizeOfBaseType(GLenum type);

extern GLint g_MaxCombinedTextureUnits;
extern std::string g_CurrentDebugMessage;

void FxSetErrorMessage(const char* filename, int line, const char* format, ...);
void FxSetDefaultErrorMessage();

GLint FxSetActiveTexture(GLint unit);
bool FxBindDefaultTextureAndSampler(GLenum target);
bool FxBindTextureAndSampler(GLint unit, GLenum target, GLuint texture, GLuint sampler);
bool FxBindTexture(GLint unit, GLenum target, GLuint texture);
bool FxBindSampler(GLint unit, GLuint sampler);
bool FxDebugBindTexture(GLenum target, GLuint texture);

//bool FxCreateBuffer(GLenum target, unsigned& abo, unsigned* p, GLsizeiptr size, const void* data, unsigned usage);
bool FxCreateBuffer(GLenum target, unsigned* p, GLsizeiptr size, const void* data, unsigned usage);
void FxDeleteBuffer(GLuint* p);

void FxCreateVertexArray(GLuint* p);
void FxDeleteVertexArray(GLuint* p);

void FxCreateRenderbuffer(GLuint* p);
void FxDeleteRenderbuffer(GLuint* p);
void FxCreateFramebuffer(GLuint* p);
void FxDeleteFramebuffer(GLuint* p);
GLuint FxCheckFramebufferStatus();
const char* FxGetFramebufferStatusAsString(GLenum status);

void FxClearScreenRgb(GLfloat r, GLfloat g, GLfloat b);

void FxGlutBitmapString(void* font, const char* str);
void FxGlutStrokeString(void* font, const char* str);

enum FX_GLUT_JUSTIFICATION {
	LEFT,
	RIGHT,
	CENTER
};

enum FX_GLUT_FONT {
	STROKE_ROMAN = 0,
	STROKE_MONO_ROMAN = 1,
	BITMAP_9_BY_15 = 2,
	BITMAP_8_BY_13 = 3,
	BITMAP_TIMES_ROMAN_10 = 4,
	BITMAP_TIMES_ROMAN_24 = 5,
	BITMAP_HELVETICA_10 = 6,
	BITMAP_HELVETICA_12 = 7,
	BITMAP_HELVETICA_18 = 8
};

// justification: LEFT = 0, RIGHT = 1, CENTER = 2
void FxGlutPrintString9x15(double x, double y, double screenWidth, int justification, const char* format, ...);

// justification: LEFT = 0, RIGHT = 1, CENTER = 2
void FxGlutPrintBitmapStringJustified(double x, double y, double screenWidth, int justification, void* font, const char* format, ...);

// justification: LEFT = 0, RIGHT = 1, CENTER = 2
void FxGlutPrintStrokeStringJustified(double x, double y, double screenWidth, int justification, void* font, const char* format, ...);

void FxDrawGL1ThreeAxis(float length);
void FxDrawGL1SixAxis(float length);
void FxDrawGL1WireFrustumf(const float* m, float scale = 0.1f);
void FxDrawGL1WireFrustumd(const double* m);
void FxDrawGL1Camera(double fov, float r, float g, float b, float zfar, const float* inverseCameraMatrix, const float* projectionMatrix);

void FxGlutTestLitSolidTeapotScene(double fovy, double aspect);

void FxDrawGL1EnvironmentCube(GLfloat size, GLuint cubeMapTexId);
void FxDrawGL2CubeMap(GLdouble x, GLdouble y, GLdouble z, GLdouble size, GLint vloc, GLint tloc);
void FxDrawGL2UnwrappedCubeMap(GLuint x, GLuint y, GLuint size, GLint vloc, GLint tloc);

void FxDrawGL1WireCube(float size);
void FxDrawGL1WireSphere(float radius, unsigned slices, unsigned stacks);
void FxDrawGL1WireCone(float base, float height, unsigned slices, unsigned stacks);
void FxDrawGL1SolidCube(float size);
void FxDrawGL1SolidSphere(float radius, unsigned slices, unsigned stacks);
void FxDrawGL1SolidCone(float base, float height, unsigned slices, unsigned stacks);
void FxDrawGL1SolidTeapot(float size);
void FxDrawGL1Superquadric(float radius, unsigned slices, unsigned stacks, float n = 1.0f, float e = 1.0f);

#endif
