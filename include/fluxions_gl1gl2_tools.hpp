#ifndef FLUXIONS_GL1GL2_TOOLS_HPP
#define FLUXIONS_GL1GL2_TOOLS_HPP

#include <fluxions_base.hpp>
#include <fluxions_opengl.hpp>

GLenum FxGetBaseType(GLenum type);
GLint FxGetTypeComponents(GLenum type);
GLsizei FxGetSizeOfType(GLenum type);
GLsizei FxGetSizeOfBaseType(GLenum type);

extern GLint g_MaxCombinedTextureUnits;
extern std::string g_CurrentDebugMessage;

void FxSetErrorMessage(const char* filename, int line, const char* format, ...);
void FxSetDefaultErrorMessage();
bool FxCheckLogErrors();

GLint FxSetActiveTexture(GLint unit);
bool FxBindDefaultTextureAndSampler(GLenum target);
bool FxBindTextureAndSampler(GLint unit, GLenum target, GLuint texture, GLuint sampler);
bool FxBindTexture(GLint unit, GLenum target, GLuint texture);
bool FxBindSampler(GLint unit, GLuint sampler);
bool FxDebugBindTexture(GLenum target, GLuint texture);

//bool FxCreateBuffer(GLenum target, unsigned& abo, unsigned* p, GLsizeiptr size, const void* data, unsigned usage);
bool FxCreateBuffer(GLenum target, unsigned* p, GLsizeiptr size, const void* data, unsigned usage);
void FxDeleteBuffer(GLuint* p);

bool FxCreateProgram(GLuint* p);
void FxDeleteProgram(GLuint* p);

bool FxCreateShader(GLenum shaderType, GLuint* p);
void FxDeleteShader(GLuint* p);

bool FxCreateTexture(GLenum target, GLuint* p);
void FxDeleteTexture(GLuint* p);

bool FxCreateSampler(GLuint* p);
void FxDeleteSampler(GLuint* p);

bool FxCreateVertexArray(GLuint* p);
void FxDeleteVertexArray(GLuint* p);

bool FxCreateRenderbuffer(GLuint* p);
void FxDeleteRenderbuffer(GLuint* p);

bool FxCreateFramebuffer(GLuint* p);
void FxDeleteFramebuffer(GLuint* p);
GLuint FxCheckFramebufferStatus();
const char* FxGetFramebufferStatusAsString(GLenum status);

inline void FxGenerateMipmap(GLenum target) { glGenerateMipmap(target); }

void FxClearScreenRgb(GLfloat r, GLfloat g, GLfloat b);

void FxGlutBitmapString(void* font, const char* str);
void FxGlutStrokeString(void* font, const char* str);

class FxSaveGraphicsState {
public:
	FxSaveGraphicsState();
	~FxSaveGraphicsState();
private:
	GLint last_active_texture{ 0 };
	GLint last_program{ 0 };
	GLint last_texture{ 0 };
	GLint last_array_buffer{ 0 };
	GLint last_element_array_buffer{ 0 };
	GLint last_vertex_array{ 0 };
	GLint last_blend_src_rgb{ 0 };
	GLint last_blend_dst_rgb{ 0 };
	GLint last_blend_src_alpha{ 0 };
	GLint last_blend_dst_alpha{ 0 };
	GLint last_blend_equation_rgb{ 0 };
	GLint last_blend_equation_alpha{ 0 };
	GLint last_viewport[4];
	GLint last_scissor_box[4];
	GLboolean last_enable_blend{ 0 };
	GLboolean last_enable_cull_face{ 0 };
	GLboolean last_enable_depth_test{ 0 };
	GLboolean last_enable_scissor_test{ 0 };
};

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
