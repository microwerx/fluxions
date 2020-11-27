#ifndef FLUXIONS_RENDERER_GLES30_HPP
#define FLUXIONS_RENDERER_GLES30_HPP

#include <fluxions_renderer_base.hpp>
#include <fluxions_renderer_config.hpp>
#include <fluxions_renderer_gles30_snapshot.hpp>
#include <fluxions_renderer_texture_cube.hpp>
#include <fluxions_shader_program_locations.hpp>

namespace Fluxions {
	class RendererContext;

	struct unit_manager {
		static constexpr int MaxUnits = 16;
		int count{ 0 };
		int index{ 0 };
		int targets[MaxUnits]{ 0 };
		int units[MaxUnits]{ 0 };
		unsigned textures[MaxUnits]{ 0 };
		unsigned samplers[MaxUnits]{ 0 };
		int ulocs[MaxUnits]{ 0 };
		int umixlocs[MaxUnits]{ 0 };

		unit_manager() { clear(); }

		void unit(int u) { units[index] = u; }
		int unit() const { return units[index]; }
		int unit(int i) const { return units[i]; }

		void target(int t) { targets[index] = t; }
		int target() const { return targets[index]; }
		int target(int i) const { return targets[i]; }

		void uniform_location(int loc) { ulocs[index] = loc; }
		int uniform_location() const { return ulocs[index]; }
		int uniform_location(int i) const { return ulocs[i]; }

		void uniform_mix_location(int loc) { umixlocs[index] = loc; }
		int uniform_mix_location() const { return umixlocs[index]; }
		int uniform_mix_location(int i) const { return umixlocs[i]; }

		void texture(unsigned t) { textures[index] = t; }
		unsigned texture() const { return textures[index]; }
		unsigned texture(int i) const { return textures[i]; }

		void sampler(unsigned s) { samplers[index] = s; }
		unsigned sampler() const { return samplers[index]; }
		unsigned sampler(int i) const { return samplers[i]; }

		void add() { index = count; count++; }
		void add(int unit_, int target_, unsigned texture_, unsigned sampler_) {
			add();
			unit(unit_);
			target(target_);
			texture(texture_);
			sampler(sampler_);
		}

		bool empty() const { return count == 0; }
		int size() const { return count; }
		void first() { index = 0; }
		void next() { index++; }
		void clear() { memset((void*)this, 0, sizeof(this)); }
	};

	class RendererGLES30 : public RendererObject {
	public:
		RendererGLES30();
		~RendererGLES30();

		void init(const std::string& name, RendererObject* pparent = nullptr) override;
		void kill() override;
		const char* type() const override;
		void invalidate_caches() override;

		void buildBuffers();
		void render();
		void renderMesh(SimpleGeometryMesh& mesh, const Matrix4f& worldMatrix);

		// ApplyRenderConfig() returns false if there was a unrecoverable error
		bool applyRenderConfig();
		bool saveGLState();
		bool restoreGLState();

		void setSceneGraph(SimpleSceneGraph* pSSG_);
		SimpleSceneGraph* getSceneGraph();

		void setRenderConfig(RendererConfig* newRenderConfig);
		RendererConfig* getRenderConfig();

		void setContext(RendererContext* pcontext);

		bool validate() const {
			return (pSSG != nullptr
					&& pRendererContext != nullptr
					&& pRendererConfig != nullptr
					&& pRendererProgram != nullptr);
		}

		static constexpr int UPDATE_VIZ = 1;
		static constexpr int UPDATE_SSG = 2;

		void update(int flags) {
			if (flags & UPDATE_VIZ) {
				viz.buffersBuilt = false;
			}
			if (flags & UPDATE_SSG) {
				scene.buffersBuilt = false;
			}
		}

		int getTexUnit();
		void freeTexUnit(int id);

		std::string renderconfigname;
		std::string renderskyboxname;
		std::string rendercamera;
	private:
		std::string name_;
		RendererGLES30Snapshot gles30StateSnapshot;

		//SimpleSceneGraph emptySSG;
		SimpleSceneGraph* pSSG = nullptr;
		RendererContext* pRendererContext = nullptr;
		RendererConfig* pRendererConfig = nullptr;
		RendererProgram* pRendererProgram = nullptr;
		RendererTextureCube* pSkyboxCube{ nullptr };
		TSimpleResourceManager<int> textureUnits;

		UbCameraBlock ssgUbCamera;
		UbEnviromentBlock ssgUbEnvironment;
		UbMaterialBlock ssgUbMaterials;
		UbDirToLightBlock ssgUbDirToLights;
		UbPointLightBlock ssgUbPointLights;
		UbAnisoLightBlock ssgUbAnisoLights;
		UbDirToShadowMatrixBlock ssgUbDirToShadowMatrices;
		UbPointShadowMatrixBlock ssgUbPointShadowMatrices;
		UbAnisoShadowMatrixBlock ssgUbAnisoShadowMatrices;

		void updateUniformBlocks();

		//Matrix4f projectionMatrix;
		//Matrix4f cameraMatrix_;

		struct SCENEINFO {
			__ShaderProgramLocations locs;
			//std::map<std::string, SimpleMap*> currentTextures;
			unit_manager maps[MaxMaterials];
			SimpleRenderer_GLuint renderer;
			int mtlIdLoc{ -1 };
			int worldMatrixLoc{ -1 };
			Matrix4f worldMatrix;
			bool buffersBuilt{ false };
		} scene;

		struct SKYBOXINFO {
			GLuint abo = 0;
			GLuint eabo = 0;
			GLint tloc{ -1 };
			GLint vloc{ -1 };
			~SKYBOXINFO() { FxDeleteBuffer(&abo); FxDeleteBuffer(&eabo); }
		} skybox;

		struct VIZINFO {
			bool buffersBuilt = false;
			int kdLoc = -1;
			int worldMatrixLoc = -1;
			SimpleRenderer_GLuint renderer;
			std::set<int> viz_surfaces;
		} viz;

		struct POSTINFO {
			bool usable_ = false;
			unsigned abo = 0;
			unsigned program = 0;
			int vloc = -1;
			int tloc = -1;
			~POSTINFO() { FxDeleteBuffer(&abo); }
		} post;

		unit_manager post_units;

		void renderSingleImage();
		void renderCubeImages();
		void applyGlobalSettingsToCurrentProgram();
		void applySpheresToCurrentProgram();
		void render(RendererProgram* program, bool useMaterials, bool useMaps, bool useZOnly);
		void applyMaterialToCurrentProgram(SimpleMaterial& mtl, bool useMaps);
		void _sceneEnableCurrentTextures(int materialId);
		void _sceneDisableCurrentTextures(int materialId);

		bool _initSkyBox();
		void _renderSkyBox();
		bool _initPost();
		void _renderPost();
		void _renderSceneGraph();
		bool _initVIZ();
		void _vizBBox(const BoundingBoxf& bbox, Matrix4f& worldMatrix, Color3f color);
		void _vizBBoxCubeMap(const BoundingBoxf& bbox, Matrix4f& worldMatrix);
		void _vizDirToLight(Vector3f center, Vector3f dir, float radius, Color3f color);
		void _vizBall(const Vector3f& center, float radius, Color3f color);
		void _renderVIZ();
	};
} // namespace Fluxions

#endif
