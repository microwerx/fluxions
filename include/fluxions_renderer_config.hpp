#ifndef FLUXIONS_RENDERER_RENDER_CONFIG_HPP
#define FLUXIONS_RENDERER_RENDER_CONFIG_HPP

#include <fluxions_renderer_object.hpp>
#include <fluxions_renderer_program.hpp>
#include <fluxions_renderer_framebuffer.hpp>

namespace Fluxions {
	class RendererGpuTexture;

	class RendererConfig : public RendererObject {
	public:
		bool useMaps{ true };
		bool useMaterials{ true };
		bool useZOnly = false;
		bool isCubeMap = false;
		// bool recomputeProjectionMatrix = false;

		bool useSceneProjection = true;
		bool useSceneCamera = true;

		bool renderSkyBox = false;
		bool renderSceneGraph = false;
		bool renderVIZ = false;
		bool renderPost = false;

		float renderPostToneMapExposure = 0.0f;
		float renderPostToneMapGamma = 0.0f;
		float renderPostFilmicHighlights = 0.0f;
		float renderPostFilmicShadows = 0.0f;

		int defaultCubeFace = -1;
		int shaderDebugChoice = 0;
		int shaderDebugLight = -1;
		int shaderDebugSphl = -1;
		bool enableDebugView = false;
		bool clearStencilBuffer = false;
		bool enableZColorBuffer = false;
		bool enableCullFace = false;
		GLenum cullFaceMode = GL_BACK;

		bool enableSRGB = false;

		bool clearDepthBuffer = false;
		bool enableDepthTest = false;
		GLenum depthComparisonTest = GL_LESS;

		bool enableBlend = false;
		GLenum blendFuncSrcFactor = GL_SRC_ALPHA;
		GLenum blendFuncDstFactor = GL_ONE_MINUS_SRC_ALPHA;

		// DEPRECATED
		//RendererProgramPtr zShaderProgram{ nullptr };
		// DEPRECATED
		//RendererProgramPtr shaderProgram{ nullptr };

		std::string rc_program;
		RendererProgram* rc_program_ptr{ nullptr };

		std::string ssg_camera;
		std::string ssg_projection;

		//bool renderToFBO{ false };
		//RendererFramebuffer fbo;
		struct MapFbo {
			std::string map;
			RendererFramebuffer* fbo;
		};
		//using string_fboptr_pair = std::pair<std::string, RendererFramebuffer *>;
		//using string_fboptr_vector = std::vector<string_fboptr_pair>;
		//string_fboptr_vector writeFBOs;
		RendererFramebuffer* writeFBO{ nullptr };
		std::vector<RendererFramebuffer*> readFBOs;

		// textures
		std::vector<std::pair<std::string, RendererGpuTexture*>> textures;

		//std::string fbo;
		bool fbo_gen_color = false;
		bool fbo_gen_depth = false;
		GLint fbo_color_map = 0;
		GLint fbo_depth_map = 0;

		bool viewportAutoresize = false;
		Recti viewportRect = Recti(0, 0, 64, 64);
		GLfloat viewportFovInDegrees = 45.0f;
		GLfloat viewportWidth = 1.0;
		GLfloat viewportZNear = 1.0f;
		GLfloat viewportZFar = 100.0f;
		Matrix4f viewportProjectionMatrix;
		void updateViewport();

		bool enableScissorTest = false;
		Recti scissorRect = viewportRect;

		bool clearColorBuffer = false;
		Color4f clearColor;

		Matrix4f preCameraMatrix;
		Matrix4f postCameraMatrix;

		// Updated by renderer

		Matrix4f projectionMatrix; // the actual projection matrix used...
		Matrix4f cameraMatrix;	 // the actual camera matrix used...
		Vector4f cameraPosition;   // the actual camera position used...

		// METRICS
		float metrics_apply_ms{ 0.0f };
		float metrics_total_ms{ 0.0f };
		float metrics_skybox_ms{ 0.0f };
		float metrics_scene_ms{ 0.0f };
		float metrics_viz_ms{ 0.0f };
		float metrics_posttime_ms{ 0.0f };

		RendererConfig();
		~RendererConfig() override;

		void init(const std::string& name, RendererObject* pparent = nullptr) override;
		void kill() override;
		const char* type() const override;
		void invalidate_caches() override;

		GLbitfield getClearBits() const;
		bool check() const;
	};
} // namespace Fluxions

#endif
