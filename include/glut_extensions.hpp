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
#ifndef FLUXIONS_GLUT_EXTENSIONS_HPP
#define FLUXIONS_GLUT_EXTENSIONS_HPP

#define GLEW_STATIC
#include <GL/glew.h>
#ifdef WIN32
#include <GL/wglew.h>
#endif
#include <GL/freeglut.h>
#include <string>

GLenum glutGetBaseType(GLenum type);
GLint glutGetTypeComponents(GLenum type);
GLsizei glutGetSizeOfType(GLenum type);
GLsizei glutGetSizeOfBaseType(GLenum type);

extern GLint g_MaxCombinedTextureUnits;
extern std::string g_CurrentDebugMessage;

void glutSetErrorMessage(const char *filename, int line, const char *format, ...);
void glutSetDefaultErrorMessage();
GLint glutSetActiveTexture(GLint unit);
bool glutBindDefaultTextureAndSampler(GLenum target);
bool glutBindTextureAndSampler(GLint unit, GLenum target, GLuint texture, GLuint sampler);
bool glutBindTexture(GLint unit, GLenum target, GLuint texture);
bool glutBindSampler(GLint unit, GLuint sampler);
bool glutDebugBindTexture(GLenum target, GLuint texture);

void glutBitmapString(void *font, const char *string);
void glutStrokeString(void *font, const char *string);

enum GLUT_JUSTIFICATION
{
	LEFT,
	RIGHT,
	CENTER
};

// justification: LEFT = 0, RIGHT = 1, CENTER = 2
void glutPrintString9x15(double x, double y, double screenWidth, int justification, const char *format, ...);

// justification: LEFT = 0, RIGHT = 1, CENTER = 2
void glutPrintBitmapStringJustified(double x, double y, double screenWidth, int justification, void *font, const char *format, ...);
// justification: LEFT = 0, RIGHT = 1, CENTER = 2
void glutPrintStrokeStringJustified(double x, double y, double screenWidth, int justification, void *font, const char *format, ...);

void glutThreeAxis(float length);
void glutSixAxis(float length);
void glutWireFrustumf(float *m, float scale = 0.1f);
void glutWireFrustumd(double *m);
void glutCamera(double fov, float r, float g, float b, float zfar, float *inverseCameraMatrix, float *projectionMatrix);

void glutTestLitSolidTeapotScene(double fovy, double aspect);

void glutEnvironmentCube(GLfloat size, GLuint cubeMapTexId);
void glutCubeMap(GLdouble x, GLdouble y, GLdouble z, GLdouble size, GLint vloc, GLint tloc);
void glutUnwrappedCubeMap(GLuint x, GLuint y, GLuint size, GLint vloc, GLint tloc);

#endif
