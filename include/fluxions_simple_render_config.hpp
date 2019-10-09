#ifndef FLUXIONS_SIMPLE_RENDER_CONFIG_HPP
#define FLUXIONS_SIMPLE_RENDER_CONFIG_HPP

namespace Fluxions
{
	struct SimpleRenderConfiguration
	{
		bool useMaps = true;
		bool useMaterials = true;
		bool useZOnly = false;
		bool useSceneCamera = true;
		bool isCubeMap = false;
		bool renderToFBO = false;
		bool recomputeProjectionMatrix = true;
		bool renderSkyBox = true;
		int defaultCubeFace = -1;
		int shaderDebugChoice = 0;
		int shaderDebugLight = -1;
		int shaderDebugSphl = -1;
		bool enableDebugView = false;
		bool clearStencilBuffer = false;
		bool enableZColorBuffer = false;
		bool enableCullFace = false;
		GLenum cullFaceMode = GL_BACK;

		bool enableSRGB = true;

		bool clearDepthBuffer = false;
		bool enableDepthTest = true;
		GLenum depthComparisonTest = GL_LESS;

		bool enableBlend = false;
		GLenum blendFuncSrcFactor = GL_SRC_ALPHA;
		GLenum blendFuncDstFactor = GL_ONE_MINUS_SRC_ALPHA;

		bool setViewport = true;
		bool setScissor = false;

		SimpleProgramPtr zShaderProgram = nullptr;
		SimpleProgramPtr shaderProgram = nullptr;

		FramebufferObject fbo;
		bool fbo_gen_color = false;
		bool fbo_gen_depth = false;
		GLint fbo_color_map = 0;
		GLint fbo_depth_map = 0;

		GLfloat fov = 45.0f;
		//GLsizei X = 0;
		//GLsizei y = 0;
		//GLsizei width = 64;
		//GLsizei height = 64;
		Recti viewportRect = Recti(0, 0, 64, 64);
		Recti scissorRect = viewportRect;
		GLfloat znear = 0.01f;
		GLfloat zfar = 1000.0f;

		bool clearColorBuffer = false;
		Color4f clearColor;

		Matrix4f projectionMatrix;
		Matrix4f preCameraMatrix;
		Matrix4f postCameraMatrix;
		Matrix4f cameraMatrix;   // the actual camera matrix used...
		Vector4f cameraPosition; // the actual camera position used...

		GLbitfield GetClearBits() const;
		bool check() const;
	};
} // namespace Fluxions

#endif