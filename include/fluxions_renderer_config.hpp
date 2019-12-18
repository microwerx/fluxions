#ifndef FLUXIONS_SIMPLE_RENDER_CONFIG_HPP
#define FLUXIONS_SIMPLE_RENDER_CONFIG_HPP

#include <fluxions_renderer_object.hpp>
#include <fluxions_renderer_program.hpp>
#include <fluxions_renderer_framebuffer_object.hpp>

namespace Fluxions
{
	class RendererConfig : public RendererObject {
	public:
		bool useMaps = true;
		bool useMaterials = true;
		bool useZOnly = false;
		bool useSceneCamera = true;
		bool isCubeMap = false;
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

		RendererProgramPtr zShaderProgram{ nullptr };
		RendererProgramPtr shaderProgram{ nullptr };

		bool renderToFBO{ false };
		RendererFramebufferObject fbo;
		using string_fboptr_pair = std::pair<std::string, RendererFramebufferObject*>;
		using string_fboptr_vector = std::vector<string_fboptr_pair>;
		string_fboptr_vector writeFBOs;
		string_fboptr_vector readFBOs;

		//std::string fbo;
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

		RendererConfig();
		~RendererConfig() override;

		void init(const std::string& name) override;
		void kill() override;

		GLbitfield getClearBits() const;
		bool check() const;
	};
} // namespace Fluxions

#endif