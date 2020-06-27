#include "fluxions_renderer_pch.hpp"
#include <fluxions_gte_matrix_math.hpp>
#include <fluxions_renderer_gles30.hpp>
#include <fluxions_renderer_context.hpp>
#include <hatchetfish_stopwatch.hpp>

namespace Fluxions {
	//////////////////////////////////////////////////////////////////////
	// RendererGLES30 ////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	RendererGLES30::RendererGLES30() {}

	RendererGLES30::~RendererGLES30() {}

	void RendererGLES30::init(const std::string& name,
							  RendererObject* pparent) {
		RendererObject::init(name, pparent);

		invalidate_caches();

		for (int i = 0; i < 32; i++) {
			int id = 16 + i;
			textureUnits.add(id);
		}
	}

	void RendererGLES30::kill() {
		if (pSSG)
			pSSG = nullptr;

		if (pRendererConfig)
			pRendererConfig = nullptr;

		if (pRendererProgram)
			pRendererProgram = nullptr;

		RendererObject::kill();
	}

	const char* RendererGLES30::type() const {
		return "RendererGLES30";
	}

	void RendererGLES30::invalidate_caches() {
		scene = SCENEINFO();
		skybox = SKYBOXINFO();
		viz = VIZINFO();
		post = POSTINFO();
		post_units.clear();
		pSSG = nullptr;
		pRendererConfig = nullptr;
		pRendererProgram = nullptr;

		ssgUbCamera.invalidate_cache();
		ssgUbEnvironment.invalidate_cache();
		ssgUbMaterials.invalidate_cache();
		ssgUbDirToLights.invalidate_cache();
		ssgUbPointLights.invalidate_cache();
		ssgUbAnisoLights.invalidate_cache();
	}

	void RendererGLES30::setSceneGraph(SimpleSceneGraph* pSSG_) {
		if (pSSG == pSSG_) return;
		pSSG = pSSG_;
		scene.buffersBuilt = false;
	}

	SimpleSceneGraph* RendererGLES30::getSceneGraph() {
		return pSSG;
	}

	bool RendererGLES30::applyRenderConfig() {
		Hf::StopWatch stopwatch;
		if (!validate()) return false;

		if (pRendererProgram->isLinked() == false)
			return false;

		Fluxions::RendererFramebuffer* writeFBO = pRendererConfig->writeFBO;
		if (writeFBO && writeFBO->usable()) {
			writeFBO->use();
			glViewport(0, 0, writeFBO->width(), writeFBO->height());
		}

		pRendererProgram->use();

		for (RendererFramebuffer* fbo : pRendererConfig->readFBOs) {
			if (fbo->unusable()) continue;
			for (auto& [type, rt] : fbo->renderTargets) {
				rt.unit = -1;
				if (rt.target == GL_RENDERBUFFER) continue;
				if (rt.mapName.empty() || rt.pGpuTexture->unusable()) continue;
				if (!pRendererProgram->activeUniforms.count(rt.mapName)) continue;
				rt.unit = getTexUnit();
				rt.pGpuTexture->bind(rt.unit);
				pRendererProgram->uniform1i(rt.mapName, rt.unit);
			}
			if (FxCheckLogErrors()) {
				HFLOGERROR("Could not configure READ FBO textures");
			}
		}

		if (!writeFBO) {
			glViewport(pRendererConfig->viewportRect.x,
					   pRendererConfig->viewportRect.y,
					   pRendererConfig->viewportRect.w,
					   pRendererConfig->viewportRect.h);
		}

		if (pRendererConfig->enableScissorTest) {
			glScissor(pRendererConfig->scissorRect.x,
					  pRendererConfig->scissorRect.y,
					  pRendererConfig->scissorRect.w,
					  pRendererConfig->scissorRect.h);
			glEnable(GL_SCISSOR_TEST);
		}

		if (pRendererConfig->enableDepthTest) {
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(pRendererConfig->depthComparisonTest);
		}

		if (pRendererConfig->enableSRGB) {
			glEnable(GL_FRAMEBUFFER_SRGB);
		}
		else {
			glDisable(GL_FRAMEBUFFER_SRGB);
		}

		if (pRendererConfig->clearColorBuffer) {
			glClearColor(pRendererConfig->clearColor.r, pRendererConfig->clearColor.g, pRendererConfig->clearColor.b, pRendererConfig->clearColor.a);
		}

		if (pRendererConfig->getClearBits())
			glClear(pRendererConfig->getClearBits());

		Matrix4f projectionMatrix_;
		Matrix4f cameraMatrix_;
		Matrix4f worldMatrix_;
		float fov = pSSG->camera.fov;
		float width = pSSG->camera.width;

		if (pRendererConfig->useSceneProjection) {
			projectionMatrix_.LoadIdentity();
			projectionMatrix_.PerspectiveY(
				pSSG->camera.fov,
				pRendererConfig->viewportRect.aspectRatiof(),
				pSSG->camera.imageNearZ,
				pSSG->camera.imageFarZ);
		}
		else {
			projectionMatrix_ = pRendererConfig->viewportProjectionMatrix;
			fov = pRendererConfig->viewportFovInDegrees;
			width = pRendererConfig->viewportWidth;
		}

		if (pRendererConfig->useSceneCamera) {
			cameraMatrix_ *= pRendererConfig->preCameraMatrix;
			cameraMatrix_ *= pSSG->camera.viewMatrix();
			cameraMatrix_ *= pRendererConfig->postCameraMatrix;
		}
		else {
			cameraMatrix_ *= pRendererConfig->preCameraMatrix;
			cameraMatrix_ *= pRendererConfig->postCameraMatrix;
		}

		if (pRendererConfig->renderSceneGraph) {
			pSSG->camera.actualViewMatrix = cameraMatrix_;
			pSSG->camera.actualFov = fov;
			pSSG->camera.actualWidth = width;
		}

		pRendererConfig->projectionMatrix = projectionMatrix_;
		pRendererConfig->cameraMatrix = cameraMatrix_;

		ssgUbCamera.ublock.ProjectionCameraMatrix = projectionMatrix_ * cameraMatrix_;
		ssgUbCamera.ublock.ProjectionMatrix = projectionMatrix_;
		ssgUbCamera.ublock.CameraMatrix = cameraMatrix_;
		ssgUbCamera.ublock.CameraPosition = cameraMatrix_.AsInverse().col4();
		ssgUbCamera.update();

		updateUniformBlocks();

		GLuint program = pRendererProgram->getProgram();
		ssgUbCamera.use(program);
		ssgUbEnvironment.use(program);
		ssgUbMaterials.use(program);
		ssgUbPointLights.use(program);
		ssgUbDirToLights.use(program);
		ssgUbDirToShadowMatrices.use(program);
		ssgUbPointShadowMatrices.use(program);
		ssgUbAnisoShadowMatrices.use(program);

		pRendererProgram->uniformMatrix4f("ProjectionMatrix", projectionMatrix_);
		pRendererProgram->uniformMatrix4f("CameraMatrix", cameraMatrix_);
		pRendererProgram->uniform4f("CameraPosition", (cameraMatrix_.AsInverse()).col4().ptr());
		pRendererProgram->uniformMatrix4f("WorldMatrix", worldMatrix_);
		pRendererProgram->uniform1f("ToneMapExposure", pRendererConfig->renderPostToneMapExposure);
		pRendererProgram->uniform1f("ToneMapGamma", pRendererConfig->renderPostToneMapGamma);
		pRendererProgram->uniform1f("FilmicHighlights", pRendererConfig->renderPostFilmicHighlights);
		pRendererProgram->uniform1f("FilmicShadows", pRendererConfig->renderPostFilmicShadows);

		//for (auto& [map, t] : pRendererConfig->textures) {
		//	if (!pRendererProgram->activeUniforms.count(map) || !t->usable()) {
		//		t->unit = -1;
		//		continue;
		//	}
		//	t->unit = getTexUnit();
		//	t->bind(t->unit);

		//	FxBindSampler(t->unit, t->samplerId);

		//	pRendererProgram->uniform1i(map, t->unit);
		//}
		pRendererConfig->metrics_apply_ms = stopwatch.Stop_msf();
		return true;
	}

	bool RendererGLES30::saveGLState() {
		gles30StateSnapshot.save();
		return true;
	}

	bool RendererGLES30::restoreGLState() {
		RendererFramebuffer* writeFBO = pRendererConfig->writeFBO;
		if (writeFBO) {
			writeFBO->unbind();
		}

		//for (auto& [map, t] : pRendererConfig->textures) {
		//	if (t->unit < 0) continue;
		//	t->unbind();
		//	FxBindSampler(t->unit, 0);
		//	freeTexUnit(t->unit);
		//	t->unit = -1;
		//	pRendererProgram->uniform1i(map, 0);
		//}

		for (RendererFramebuffer* fbo : pRendererConfig->readFBOs) {
			if (fbo->unusable()) continue;
			for (auto& [type, rt] : fbo->renderTargets) {
				if (rt.unit < 0) continue;
				rt.pGpuTexture->unbind();
				freeTexUnit(rt.unit);
				rt.unit = -1;
				pRendererProgram->uniform1i(rt.mapName, 0);
			}
		}

		gles30StateSnapshot.restore();

		return true;
	}

	void RendererGLES30::setRenderConfig(RendererConfig* rc) {
		if (!rc) return;

		pRendererConfig = rc;
		pRendererProgram = nullptr;

		if (pRendererContext && pRendererContext->programs.count(rc->rc_program)) {
			pRendererProgram = &pRendererContext->programs[rc->rc_program];
		}
		//else if (pRendererConfig->useZOnly) {
		//	if (pRendererConfig->zShaderProgram != nullptr) {
		//		pRendererProgram = pRendererConfig->zShaderProgram;
		//	}
		//}
		//else if (pRendererConfig->shaderProgram != nullptr) {
		//	pRendererProgram = pRendererConfig->shaderProgram;
		//}
	}

	RendererConfig* RendererGLES30::getRenderConfig() {
		return pRendererConfig;
	}

	void RendererGLES30::setContext(RendererContext* pcontext) {
		pRendererContext = pcontext;
	}

	void RendererGLES30::updateUniformBlocks() {
		static std::string blockname;
		blockname = ssgUbEnvironment.uniformBlockName();
		if (pRendererProgram->activeUniformBlocks.count(blockname)) {
			BaseEnvironment* bEnvironment = (BaseEnvironment*)&pSSG->environment;
			ssgUbEnvironment.uniforms = *bEnvironment;
			ssgUbEnvironment.update();
		}

		blockname = ssgUbMaterials.uniformBlockName();
		if (pRendererProgram->activeUniformBlocks.count(blockname)) {
			unsigned i = 0;
			for (const auto& mtl : pSSG->materials.mtls) {
				if (i >= ssgUbMaterials.size()) break;
				const BaseMaterial* bm = &mtl;
				ssgUbMaterials.uniforms[i++] = *bm;
			}
			ssgUbMaterials.update();
		}

		blockname = ssgUbDirToLights.uniformBlockName();
		if (pRendererProgram->activeUniformBlocks.count(blockname)) {
			unsigned i = 0;
			for (const auto& [k, dl] : pSSG->dirToLights) {
				if (i >= ssgUbDirToLights.size()) break;
				ssgUbDirToLights.uniforms[i++] = dl.ublock;
			}
			ssgUbDirToLights.update();
		}

		blockname = ssgUbDirToShadowMatrices.uniformBlockName();
		if (pRendererProgram->activeUniformBlocks.count(blockname)) {
			unsigned i = 0;
			for (const auto& [k, dl] : pSSG->dirToLights) {
				if (i >= ssgUbDirToShadowMatrices.size()) break;
				ssgUbDirToShadowMatrices.uniforms[i++] = dl.projviewMatrix;
			}
			ssgUbDirToShadowMatrices.update();
		}

		blockname = ssgUbPointLights.uniformBlockName();
		if (pRendererProgram->activeUniformBlocks.count(blockname)) {
			unsigned i = 0;
			for (const auto& [k, pl] : pSSG->pointLights) {
				if (i >= ssgUbPointLights.size()) break;
				const BasePointLight* bPointLight = &pl.ublock;
				ssgUbPointLights.uniforms[i++] = *bPointLight;
			}
			ssgUbPointLights.update();
		}

		blockname = ssgUbAnisoLights.uniformBlockName();
		if (pRendererProgram->activeUniformBlocks.count(blockname)) {
			unsigned i = 0;
			for (const auto& [k, pl] : pSSG->anisoLights) {
				if (i >= ssgUbAnisoLights.size()) break;
				const BaseAnisoLight* bAnisoLight = &pl.ublock;
				ssgUbAnisoLights.uniforms[i++] = *bAnisoLight;
			}
			ssgUbAnisoLights.update();
		}

		blockname = ssgUbAnisoShadowMatrices.uniformBlockName();
		if (pRendererProgram->activeUniformBlocks.count(blockname)) {
			unsigned i = 0;
			for (const auto& [k, al] : pSSG->anisoLights) {
				if (i >= ssgUbAnisoShadowMatrices.size()) break;
				ssgUbAnisoShadowMatrices.uniforms[i++] = al.projviewMatrix;
			}
			ssgUbAnisoShadowMatrices.update();
		}
	}

	void RendererGLES30::render() {
		if (!validate()) return;
		if (pRendererConfig->isCubeMap)
			renderCubeImages();
		else
			renderSingleImage();
	}

	class BufferObject {
	public:
		GLuint buffer = 0;
		GLenum target = 0;

	public:
		BufferObject(GLenum target_, GLsizeiptr size, const void* data, GLenum usage) :
			target(target_) {
			FxCreateBuffer(target, &buffer, size, data, usage);
		}

		~BufferObject() {
			Delete();
		}

		void Bind() {
			glBindBuffer(target, buffer);
		}

		void Delete() {
			FxDeleteBuffer(&buffer);
		}
	};

	class VertexArrayObject {
	private:
		GLuint vao = 0;
		GLuint abo = 0;
		GLuint eabo = 0;

		struct Surface {
			GLenum mode;
			GLsizei count;
			void* offset;
			GLenum type;
		};

		std::vector<Surface> surfaces;

	public:
		VertexArrayObject(const SimpleGeometryMesh& mesh, GLuint program, GLuint abo, GLuint eabo) {
			this->abo = abo;
			this->eabo = eabo;

			if (FxCreateVertexArray(&vao)) {
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER, abo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
				unsigned attribCount = 0;
				for (unsigned i = 0; i < 8; i++) {
					if (mesh.isAttribEnabled(i)) {
						const char* name = mesh.getAttribName(i);
						if (name == nullptr)
							continue;
						GLint loc = glGetAttribLocation(program, name);
						if (loc < 0) {
							continue;
						}
						glVertexAttribPointer(loc, 4, GL_FLOAT, mesh.isAttribNormalized(i), sizeof(SimpleGeometryMesh::Vertex), cast_to_pointer(mesh.getVertexOffset(i)));
						glEnableVertexAttribArray(loc);
						attribCount++;
					}
				}

				if (attribCount == 0) {
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
					FxDeleteVertexArray(&vao);
					return;
				}

				GLenum type = GL_UNSIGNED_INT;
				surfaces.resize(mesh.Surfaces.size());
				unsigned i = 0;
				for (const auto& meshSurface : mesh.Surfaces) {
					surfaces[i].mode = meshSurface.mode;
					surfaces[i].count = meshSurface.count;
					surfaces[i].offset = cast_to_pointer(meshSurface.first * sizeof(unsigned));
					surfaces[i].type = type;
					i++;
				}

				glBindVertexArray(0);
			}
		}

		~VertexArrayObject() {
			FxDeleteVertexArray(&vao);
		}

		void Draw() {
			if (vao == 0)
				return;

			glBindVertexArray(vao);
			for (auto& surface : surfaces) {
				glDrawElements(surface.mode, surface.count, surface.type, surface.offset);
			}
			glBindVertexArray(0);
		}

		inline void* cast_to_pointer(size_t i) const noexcept {
			return (void*)i;
		}
	};

	void RendererGLES30::renderMesh(SimpleGeometryMesh& mesh, const Matrix4f& modelViewMatrix) {
		if (!validate())
			return;

		pRendererProgram->use();
		pRendererProgram->uniformMatrix4f("WorldMatrix", modelViewMatrix);

		// create a vbo
		BufferObject abo_(GL_ARRAY_BUFFER, (GLsizei)mesh.getVertexDataSize(), mesh.getVertexData(), GL_STATIC_DRAW);
		BufferObject eabo_(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)mesh.getIndexDataSize(), mesh.getIndexData(), GL_STATIC_DRAW);

		VertexArrayObject vao(mesh, pRendererProgram->getProgram(), abo_.buffer, eabo_.buffer);
		vao.Draw();
	}

	void RendererGLES30::renderCubeImages() {
		if (!scene.buffersBuilt)
			buildBuffers();

		GLsizei s = pRendererConfig->viewportRect.w;

		pRendererConfig->viewportRect.x = 0;
		pRendererConfig->viewportRect.y = s;
		pRendererConfig->defaultCubeFace = 1;
		glClear(GL_DEPTH_BUFFER_BIT);
		renderSingleImage();

		pRendererConfig->viewportRect.x = 2 * s;
		pRendererConfig->viewportRect.y = s;
		pRendererConfig->defaultCubeFace = 0;
		glClear(GL_DEPTH_BUFFER_BIT);
		renderSingleImage();

		pRendererConfig->viewportRect.x = s;
		pRendererConfig->viewportRect.y = 2 * s;
		pRendererConfig->defaultCubeFace = 2;
		glClear(GL_DEPTH_BUFFER_BIT);
		renderSingleImage();

		pRendererConfig->viewportRect.x = s;
		pRendererConfig->viewportRect.y = 0;
		pRendererConfig->defaultCubeFace = 3;
		glClear(GL_DEPTH_BUFFER_BIT);
		renderSingleImage();

		pRendererConfig->viewportRect.x = s;
		pRendererConfig->viewportRect.y = s;
		pRendererConfig->defaultCubeFace = 4;
		glClear(GL_DEPTH_BUFFER_BIT);
		renderSingleImage();

		pRendererConfig->viewportRect.x = 3 * s;
		pRendererConfig->viewportRect.y = s;
		pRendererConfig->defaultCubeFace = 5;
		glClear(GL_DEPTH_BUFFER_BIT);
		renderSingleImage();
	}

	void RendererGLES30::renderSingleImage() {
		saveGLState();

		if (!applyRenderConfig()) return;

		if (pRendererConfig->renderSkyBox) {
			_renderSkyBox();
		}

		if (pRendererConfig->renderSceneGraph) {
			_renderSceneGraph();
		}

		if (pRendererConfig->renderPost) {
			_renderPost();
		}

		if (pRendererConfig->renderVIZ) {
			_renderVIZ();
		}

		restoreGLState();
	}

	void RendererGLES30::applyGlobalSettingsToCurrentProgram() {
		pRendererProgram->uniform1i("ShaderDebugChoice", this->pRendererConfig->shaderDebugChoice);
		pRendererProgram->uniform1i("ShaderDebugLight", this->pRendererConfig->shaderDebugLight);
		pRendererProgram->uniform1i("ShaderDebugSphl", this->pRendererConfig->shaderDebugSphl);

		//if (scene.locs.enviroCubeMap >= 0)
		//	glUniform1i(scene.locs.enviroCubeMap, pSSG->environment.enviroColorMapUnit);
		//if (scene.locs.enviroCubeMapAmount >= 0)
		//	glUniform1f(scene.locs.enviroCubeMapAmount, 0.0);
		//if (scene.locs.pbskyCubeMap >= 0)
		//	glUniform1i(scene.locs.pbskyCubeMap, pSSG->environment.pbskyColorMapUnit);
		//if (scene.locs.sunDirTo >= 0)
		//	glUniform3fv(scene.locs.sunDirTo, 1, pSSG->environment.curSunDirTo.const_ptr());
		//if (scene.locs.sunColor >= 0)
		//	glUniform3fv(scene.locs.sunColor, 1, pSSG->environment.sunColor.const_ptr());
		//if (scene.locs.sunSize >= 0)
		//	glUniform1f(scene.locs.sunSize, pSSG->environment.sunSize);
		//if (scene.locs.sunShadowMap >= 0)
		//	glUniform1i(scene.locs.sunShadowMap, pSSG->environment.sunDepthMapUnit);
		//if (scene.locs.sunColorMap >= 0)
		//	glUniform1i(scene.locs.sunColorMap, pSSG->environment.sunColorMapUnit);
	}

	void RendererGLES30::applySpheresToCurrentProgram() {
		//std::vector<float> spherePositions;
		//std::vector<float> sphereKe;
		//int numSpheres = 0;
		//for (auto sphIt = pSSG->spheres.begin(); sphIt != pSSG->spheres.end(); sphIt++) {
		//	if (spherePositions.size() > 8)
		//		break;
		//	Vector4f pos(0, 0, 0, 1);
		//	Vector4f radius(1, 0, 0, 1);
		//	pos = sphIt->second.transform * pos;
		//	radius = sphIt->second.transform * radius;
		//	radius = radius - pos;
		//	float length = radius.length();
		//	pos.w = length;
		//	SimpleMaterial* mtl = pSSG->materialSystem.SetLibraryMaterial(sphIt->second.mtllibName, sphIt->second.mtlName);
		//	// only push spheres that are emissive...
		//	if (mtl) {
		//		spherePositions.push_back(pos.x);
		//		spherePositions.push_back(pos.y);
		//		spherePositions.push_back(pos.z);
		//		spherePositions.push_back(pos.w);

		//		Color3f Ke = mtl->Ke;
		//		sphereKe.push_back(Ke.r);
		//		sphereKe.push_back(Ke.g);
		//		sphereKe.push_back(Ke.b);
		//		sphereKe.push_back(1.0f);
		//		numSpheres++;
		//	}
		//}
		//if (numSpheres > 0) {
		//	if (scene.locs.sphere_array >= 0)
		//		glUniform4fv(scene.locs.sphere_array, numSpheres, &spherePositions[0]);
		//	if (scene.locs.sphere_Ke >= 0)
		//		glUniform4fv(scene.locs.sphere_Ke, numSpheres, &sphereKe[0]);
		//}
		//if (scene.locs.sphere_count >= 0)
		//	glUniform1i(scene.locs.sphere_count, numSpheres);
	}

	void RendererGLES30::render(RendererProgram* program_, bool useMaterials, bool useMaps, bool useZOnly) {
		if (!useMaterials && !useMaps) {
			scene.renderer.Render();
			return;
		}

		int drawCount = 0;
		int errorCount = 0;
		for (int i = 0; i < MaxMaterials; i++) {
			_sceneEnableCurrentTextures(i);
			for (auto& [id, group] : pSSG->geometryGroups) {
				if (scene.mtlIdLoc >= 0)
					glUniform1i(scene.mtlIdLoc, i);
				if (scene.worldMatrixLoc >= 0) {
					scene.worldMatrix = std::move(group.worldMatrix());
					glUniformMatrix4fv(scene.worldMatrixLoc, 1, GL_FALSE, scene.worldMatrix.const_ptr());
				}
				int count = scene.renderer.RenderIf(group.objectId, i, false);
				if (count) {
					if (FxCheckLogErrors()) {
						errorCount++;
					}
					drawCount += count;
				}
			}
			_sceneDisableCurrentTextures(i);
		}
		if (!drawCount) {
			HFLOGDEBUGFIRSTRUNCOUNTMSG(5, "0 objects drawn!");
		}
		if (errorCount) {
			HFLOGDEBUGFIRSTRUNCOUNTMSG(5, "%d errors!", errorCount);
		}
		return;
	}

	void RendererGLES30::applyMaterialToCurrentProgram(SimpleMaterial& mtl, bool useMaps) {
		//GLuint unit = 1;

		//if (useMaps) {
		//	if (!mtl.map_Ka.empty())
		//		scene.currentTextures["map_Ka"] = pSSG->materialSystem.GetTextureMap(mtl.map_Ka);
		//	if (!mtl.map_Kd.empty())
		//		scene.currentTextures["map_Kd"] = pSSG->materialSystem.GetTextureMap(mtl.map_Kd);
		//	if (!mtl.map_Ks.empty())
		//		scene.currentTextures["map_Ks"] = pSSG->materialSystem.GetTextureMap(mtl.map_Ks);
		//	if (!mtl.map_Ke.empty())
		//		scene.currentTextures["map_Ke"] = pSSG->materialSystem.GetTextureMap(mtl.map_Ke);
		//	if (!mtl.map_Ns.empty())
		//		scene.currentTextures["map_Ns"] = pSSG->materialSystem.GetTextureMap(mtl.map_Ns);
		//	if (!mtl.map_Ns.empty())
		//		scene.currentTextures["map_Ni"] = pSSG->materialSystem.GetTextureMap(mtl.map_Ni);
		//	if (!mtl.map_Tf.empty())
		//		scene.currentTextures["map_Tf"] = pSSG->materialSystem.GetTextureMap(mtl.map_Tf);
		//	if (!mtl.map_Tr.empty())
		//		scene.currentTextures["map_Tr"] = pSSG->materialSystem.GetTextureMap(mtl.map_Tr);
		//	if (!mtl.map_bump.empty())
		//		scene.currentTextures["map_bump"] = pSSG->materialSystem.GetTextureMap(mtl.map_bump);
		//	if (!mtl.map_normal.empty())
		//		scene.currentTextures["map_normal"] = pSSG->materialSystem.GetTextureMap(mtl.map_normal);
		//	if (!mtl.PBmap.empty())
		//		scene.currentTextures["PBmap"] = pSSG->materialSystem.GetTextureMap(mtl.PBmap);

		//	for (auto& [mapName, pMap] : scene.currentTextures) {
		//		//for (auto tmapIt = currentTextures.begin(); tmapIt != currentTextures.end(); tmapIt++) {
		//		//	SimpleMap* pMap = tmapIt->second;
		//		if (pMap) {
		//			if (pMap->cached.unitId <= 0)
		//				pMap->cached.unitId = getTexUnit();
		//			// TODO: Renable texture mapping
		//			//pMap->cached.samplerId = pMap->cached.textureObject.getSamplerId();
		//			//pMap->cached.textureId = pMap->cached.textureObject.getTextureId();
		//			//pMap->cached.textureObject.bind(pMap->cached.unitId, false);
		//			//glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);

		//			//if (pMap->cached.samplerId == 0) {
		//			//	glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_WRAP_S, GL_REPEAT);
		//			//	glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_WRAP_T, GL_REPEAT);
		//			//	glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_WRAP_R, GL_REPEAT);
		//			//	glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//			//	glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//			//}

		//			pMap->cached.map_loc = -1;
		//			pMap->cached.map_mix_loc = -1;
		//			if (mapName == "map_Ka") {
		//				pMap->cached.map_loc = scene.locs.map_Ka;
		//				pMap->cached.map_mix_loc = scene.locs.map_Ka_mix;
		//			}
		//			else if (mapName == "map_Kd") {
		//				pMap->cached.map_loc = scene.locs.map_Kd;
		//				pMap->cached.map_mix_loc = scene.locs.map_Kd_mix;
		//			}
		//			else if (mapName == "map_Ks") {
		//				pMap->cached.map_loc = scene.locs.map_Ks;
		//				pMap->cached.map_mix_loc = scene.locs.map_Ks_mix;
		//			}
		//			else if (mapName == "map_Ke") {
		//				pMap->cached.map_loc = scene.locs.map_Ke;
		//				pMap->cached.map_mix_loc = scene.locs.map_Ke_mix;
		//			}
		//			else if (mapName == "map_Ni") {
		//				pMap->cached.map_loc = scene.locs.map_Ni;
		//				pMap->cached.map_mix_loc = scene.locs.map_Ni_mix;
		//			}
		//			else if (mapName == "map_Ns") {
		//				pMap->cached.map_loc = scene.locs.map_Ns;
		//				pMap->cached.map_mix_loc = scene.locs.map_Ns_mix;
		//			}
		//			else if (mapName == "map_Tf") {
		//				pMap->cached.map_loc = scene.locs.map_Tf;
		//				pMap->cached.map_mix_loc = scene.locs.map_Tf_mix;
		//			}
		//			else if (mapName == "map_Tr") {
		//				pMap->cached.map_loc = scene.locs.map_Tr;
		//				pMap->cached.map_mix_loc = scene.locs.map_Tr_mix;
		//			}
		//			else if (mapName == "map_bump") {
		//				pMap->cached.map_loc = scene.locs.map_bump;
		//				pMap->cached.map_mix_loc = scene.locs.map_bump_mix;
		//			}
		//			else if (mapName == "map_normal") {
		//				pMap->cached.map_loc = scene.locs.map_normal;
		//				pMap->cached.map_mix_loc = scene.locs.map_normal_mix;
		//			}
		//			else if (mapName == "PBmap") {
		//				pMap->cached.map_loc = scene.locs.PBmap;
		//				pMap->cached.map_mix_loc = scene.locs.PBmap_mix;
		//			}

		//			if (pMap->cached.unitId < 0 || pMap->cached.unitId >= g_MaxCombinedTextureUnits) {
		//				HFLOGERROR("pMap->unitId (%d) is out of range.", pMap->cached.unitId);
		//			}
		//			else {
		//				if (pMap->cached.map_loc >= 0)
		//					glUniform1i(pMap->cached.map_loc, pMap->cached.unitId);
		//				if (pMap->cached.map_mix_loc >= 0)
		//					glUniform1f(pMap->cached.map_mix_loc, 1.0f);
		//			}

		//			unit++;
		//		}
		//	}
		//}

		//// Apply Material Uniforms to the program_ shader
		//if (scene.locs.Ka >= 0)
		//	glUniform3fv(scene.locs.Ka, 1, &mtl.Ka.r);
		//if (scene.locs.Kd >= 0)
		//	glUniform3fv(scene.locs.Kd, 1, &mtl.Kd.r);
		//if (scene.locs.Ks >= 0)
		//	glUniform3fv(scene.locs.Ks, 1, &mtl.Ks.r);
		//if (scene.locs.Ke >= 0)
		//	glUniform3fv(scene.locs.Ke, 1, &mtl.Ke.r);
		//if (scene.locs.Ni >= 0)
		//	glUniform1fv(scene.locs.Ni, 1, &mtl.Ni);
		//if (scene.locs.Ns >= 0)
		//	glUniform1fv(scene.locs.Ns, 1, &mtl.Ns);
		//if (scene.locs.Tf >= 0)
		//	glUniform3fv(scene.locs.Tf, 1, &mtl.Tf.r);
		//if (scene.locs.Tr >= 0)
		//	glUniform1fv(scene.locs.Tr, 1, &mtl.Tr);
		//if (scene.locs.PBKdm >= 0)
		//	glUniform1fv(scene.locs.PBKdm, 1, &mtl.PBKdm);
		//if (scene.locs.PBKsm >= 0)
		//	glUniform1fv(scene.locs.PBKsm, 1, &mtl.PBKsm);
		//if (scene.locs.PBior >= 0)
		//	glUniform1fv(scene.locs.PBior, 1, &mtl.PBior);
		//if (scene.locs.PBGGXgamma >= 0)
		//	glUniform1fv(scene.locs.PBGGXgamma, 1, &mtl.PBGGXgamma);
		//if (scene.locs.PBm >= 0)
		//	glUniform1fv(scene.locs.PBm, 1, &mtl.PBm);
		//if (scene.locs.PBk >= 0)
		//	glUniform1fv(scene.locs.PBk, 1, &mtl.PBk);

		//// The better way to do this is to first find all the locations for the material, then apply them...
		//for (auto uniform : scene.locs.newLocationList) {
		//	// uniform.first = name
		//	// uniform.second = details
		//}
	}

	void RendererGLES30::_sceneEnableCurrentTextures(int materialId) {
		auto& units = scene.maps[materialId];
		if (units.count == 0) {
			auto& texture2Ds = pRendererContext->texture2Ds;
			if (pSSG->materials.mtls.size() <= materialId) return;
			SimpleMaterial& mtl = pSSG->materials.mtls[materialId];

			bool use_renderconfig_textures = !pRendererConfig->textures.empty();
			bool use_material_textures = !mtl.maps.empty();

			if (use_renderconfig_textures) {
				for (auto& [m, gput] : pRendererConfig->textures) {
					int loc = pRendererProgram->getUniformLocation(m);
					if (loc < 0) continue;
					GLuint target = gput->getTarget();
					GLuint texture = gput->getTexture();
					GLuint sampler = gput->samplerId;
					units.add();
					units.target(target);
					units.texture(texture);
					units.sampler(sampler);
					units.uniform_location(loc);
				}
			}

			scene.locs.getMaterialProgramLocations(*pRendererProgram);
			for (auto& shader_map_info : scene.locs.maps) {
				if (shader_map_info.mapNameLoc < 0) continue;

				if (use_material_textures) {
					std::string mapName;
					if (mtl.hasMap(shader_map_info.mapName))
						mapName = mtl.maps.at(shader_map_info.mapName);
					else if (mtl.hasMap(shader_map_info.altMtlMapName))
						mapName = mtl.maps.at(shader_map_info.altMtlMapName);
					else continue;

					if (texture2Ds.count(mapName)) {
						RendererTexture2D& tex2d = pRendererContext->texture2Ds.at(mapName);
						GLuint target = tex2d.getTarget();
						GLuint texture = tex2d.getTexture();
						GLuint sampler = 0; // TODO: Assign sampler
						GLint uniform = shader_map_info.mapNameLoc;

						if (target == GL_TEXTURE_2D && !sampler && pRendererContext->samplers.count("default2dSampler")) {
							sampler = pRendererContext->samplers["default2dsampler"].getId();
						}

						if (target == GL_TEXTURE_CUBE_MAP && !sampler && pRendererContext->samplers.count("defaultCubeSampler")) {
							sampler = pRendererContext->samplers["defaultCubeSampler"].getId();
						}

						units.add();
						units.target(target);
						units.texture(texture);
						units.uniform_location(uniform);
						units.sampler(sampler);
					}
				}
			}
		}

		units.first();
		for (int i = 0; i < units.count; i++) {
			units.unit(getTexUnit());
			int loc = units.uniform_location();
			if (loc >= 0) {
				glUniform1i(loc, units.unit());
			}
			FxBindTextureAndSampler(units.unit(), units.target(), units.texture(), units.sampler());
			units.next();
		}
	}

	void RendererGLES30::_sceneDisableCurrentTextures(int materialId) {
		auto& units = scene.maps[materialId];
		units.first();
		for (int i = 0; i < units.count; i++) {
			FxBindTextureAndSampler(units.unit(), units.target(), 0, 0);
			freeTexUnit(units.unit());
			int loc = units.uniform_location();
			if (loc >= 0) {
				glUniform1i(loc, 0);
			}
			units.next();
		}

		//// Turn off textures and reset program_ unit bindings to 0
		//for (auto tmapIt = scene.currentTextures.begin(); tmapIt != scene.currentTextures.end(); tmapIt++) {
		//	SimpleMap* pMap = tmapIt->second;
		//	if (pMap) {
		//		//FxBindTexture(pMap->cached.unitId, pMap->cached.textureObject.getTarget(), 0);
		//		// TODO: Bind Texture

		//		glUniform1i(pMap->cached.map_loc, 0);
		//		glUniform1f(pMap->cached.map_mix_loc, 0.0f);

		//		if (pMap->cached.unitId != 0) {
		//			freeTexUnit(pMap->cached.unitId);
		//			pMap->cached.unitId = 0;
		//		}
		//	}
		//}
		//FxSetActiveTexture(GL_TEXTURE0);

		//scene.currentTextures.clear();
	}

	int RendererGLES30::getTexUnit() {
		return textureUnits.create();
	}

	void RendererGLES30::freeTexUnit(int id) {
		textureUnits.erase(id);
	}

	bool RendererGLES30::_initSkyBox() {
		if (skybox.abo && skybox.eabo && pSkyboxCube) return true;

		constexpr GLfloat size = 50.0f;
		GLfloat buffer[] = {
			-size, size, -size, -1.0f, 1.0f, -1.0f,
			size, size, -size, 1.0f, 1.0f, -1.0f,
			size, -size, -size, 1.0f, -1.0f, -1.0f,
			-size, -size, -size, -1.0f, -1.0f, -1.0f,
			size, size, size, 1.0f, 1.0f, 1.0f,
			-size, size, size, -1.0f, 1.0f, 1.0f,
			-size, -size, size, -1.0f, -1.0f, 1.0f,
			size, -size, size, 1.0f, -1.0f, 1.0f };

		GLushort indices[] = {
			// FACE 0
			7, 4, 1,
			1, 2, 7,
			// FACE 1
			3, 0, 5,
			5, 6, 3,
			// FACE 2
			1, 4, 5,
			5, 0, 1,
			// FACE 3
			7, 2, 3,
			3, 6, 7,
			// FACE 5
			6, 5, 4,
			4, 7, 6,
			// FACE 4
			2, 1, 0,
			0, 3, 2 };

		if (skybox.abo == 0) {
			FxCreateBuffer(GL_ARRAY_BUFFER, &skybox.abo, sizeof(buffer), buffer, GL_STATIC_DRAW);
			FxCreateBuffer(GL_ELEMENT_ARRAY_BUFFER, &skybox.eabo, sizeof(indices), indices, GL_STATIC_DRAW);
		}

		if (!pSkyboxCube && !renderskyboxname.empty()) {
			pSkyboxCube = &pRendererContext->textureCubes[renderskyboxname];
		}

		return (skybox.abo && skybox.eabo && pSkyboxCube);
	}

	void RendererGLES30::_renderSkyBox() {
		Hf::StopWatch stopwatch;
		if (!_initSkyBox()) return;

		static const std::string VERTEX_LOCATION{ "aPosition" };
		static const std::string TEXCOORD_LOCATION{ "aTexCoord" };
		int vloc = skybox.vloc >= 0 ? skybox.vloc : pRendererProgram->getAttribLocation(VERTEX_LOCATION);
		int tloc = skybox.tloc >= 0 ? skybox.tloc : pRendererProgram->getAttribLocation(TEXCOORD_LOCATION);

		if (!pRendererConfig->textures.empty()) {
			pRendererConfig->textures[0].second->bind(0);
			pRendererProgram->uniform1i("MapEnviroCube", 0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, skybox.abo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox.eabo);
		if (vloc >= 0) glVertexAttribPointer(vloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void*)0);
		if (tloc >= 0) glVertexAttribPointer(tloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void*)12);
		if (vloc >= 0) glEnableVertexAttribArray(vloc);
		if (tloc >= 0) glEnableVertexAttribArray(tloc);
		if (vloc >= 0) glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		if (vloc >= 0) glDisableVertexAttribArray(vloc);
		if (tloc >= 0) glDisableVertexAttribArray(tloc);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (!pRendererConfig->textures.empty()) {
			pRendererConfig->textures[0].second->unbind();
			pRendererProgram->uniform1i("MapEnviroCube", 0);
		}

		pRendererConfig->metrics_skybox_ms = stopwatch.Stop_msf();
	}

	bool RendererGLES30::_initPost() {
		if (post.usable_) return true;
		if (!pRendererConfig) return false;

		post.program = pRendererConfig->rc_program_ptr->getProgram();

		if (!post.abo) {
			constexpr float w = 1.0f;
			constexpr float h = 1.0f;
			constexpr float x = -1.0f;
			constexpr float y = -1.0f;
			constexpr float z = 0.0f;
			constexpr float s1 = 0.0f;
			constexpr float t1 = 0.0f;
			constexpr float s2 = 1.0f;
			constexpr float t2 = 1.0f;
			GLfloat buffer[] = {
				x, h, z, s1, t2, 0.0f,
				x, y, z, s1, t1, 0.0f,
				w, h, z, s2, t2, 0.0f,
				w, y, z, s2, t1, 0.0f
			};
			FxCreateBuffer(GL_ARRAY_BUFFER, &post.abo, sizeof(buffer), buffer, GL_STATIC_DRAW);
		}

		const std::string VERTEX_LOCATION{ "aPosition" };
		const std::string TEXCOORD_LOCATION{ "aTexCoord" };
		post.vloc = pRendererProgram->getAttribLocation(VERTEX_LOCATION);
		post.tloc = pRendererProgram->getAttribLocation(TEXCOORD_LOCATION);

		post.usable_ = true;
		if (!pRendererConfig->rc_program_ptr) post.usable_ = false;
		if (!post.abo) post.usable_ = false;
		if (!post.program) post.usable_ = false;
		if (post.vloc < 0) post.usable_ = false;
		return post.usable_;
	}


	void RendererGLES30::_renderPost() {
		Hf::StopWatch stopwatch;
		if (!_initPost()) return;

		int i = 0;
		for (auto& [k, tex] : pRendererConfig->textures) {
			tex->bind(i);
			pRendererProgram->uniform1i(k, i);
			i++;
		}

		glBindBuffer(GL_ARRAY_BUFFER, post.abo);
		glVertexAttribPointer(post.vloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void*)0);
		if (post.tloc >= 0) glVertexAttribPointer(post.tloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void*)12);
		if (post.vloc >= 0) glEnableVertexAttribArray(post.vloc);
		if (post.tloc >= 0) glEnableVertexAttribArray(post.tloc);
		if (post.vloc >= 0) glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		if (post.vloc >= 0) glDisableVertexAttribArray(post.vloc);
		if (post.tloc >= 0) glDisableVertexAttribArray(post.tloc);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		i = 0;
		for (auto& [k, tex] : pRendererConfig->textures) {
			tex->unbind();
			pRendererProgram->uniform1i(k, 0);
			i++;
		}

		pRendererConfig->metrics_posttime_ms = stopwatch.Stop_msf();
	}

	void RendererGLES30::_renderSceneGraph() {
		Hf::StopWatch stopwatch;
		if (!scene.buffersBuilt) buildBuffers();

		if (scene.mtlIdLoc < 0) scene.mtlIdLoc = pRendererProgram->getUniformLocation("MtlID");
		if (scene.worldMatrixLoc < 0) scene.worldMatrixLoc = pRendererProgram->getUniformLocation("WorldMatrix");

		applyGlobalSettingsToCurrentProgram();
		applySpheresToCurrentProgram();

		if (pRendererConfig->isCubeMap) {
			//projectionMatrix.LoadIdentity();
			//projectionMatrix.Perspective(90.0f, 1.0f,
			//							 pRendererConfig->viewportZNear,
			//							 pRendererConfig->viewportZFar);
			//Vector4f cameraPosition(0, 0, 0, 1);
			//cameraPosition = cameraMatrix_ * cameraPosition;
			//if (pRendererConfig->defaultCubeFace >= 0 && pRendererConfig->defaultCubeFace < 6) {
			//	cameraMatrix_.LoadIdentity();
			//	cameraMatrix_.CubeMatrix(GL_TEXTURE_CUBE_MAP_POSITIVE_X + pRendererConfig->defaultCubeFace);
			//	cameraMatrix_.Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z);
			//	render(pRendererProgram, pRendererConfig->useMaterials, pRendererConfig->useMaps, pRendererConfig->useZOnly, projectionMatrix, cameraMatrix_);
			//}
			//else {
			//	for (int i = 0; i < 6; i++) {
			//		cameraMatrix_.LoadIdentity();
			//		cameraMatrix_.Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z);
			//		cameraMatrix_.CubeMatrix(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
			//		render(pRendererProgram, pRendererConfig->useMaterials, pRendererConfig->useMaps, pRendererConfig->useZOnly, projectionMatrix, cameraMatrix_);
			//	}
			//}
		}
		else {
			render(pRendererProgram, pRendererConfig->useMaterials, pRendererConfig->useMaps, pRendererConfig->useZOnly);
		}

		pRendererConfig->metrics_scene_ms = stopwatch.Stop_msf();
	}

	void RendererGLES30::_vizBBox(const BoundingBoxf& bbox, Matrix4f& worldMatrix, Color3f color) {
		Vector3f a = bbox.minBounds;
		Vector3f b = bbox.maxBounds;
		Vector3f vertices[8] = {
			{a.x, a.y, a.z},
			{b.x, a.y, a.z},
			{b.x, b.y, a.z},
			{a.x, b.y, a.z},
			{a.x, a.y, b.z},
			{b.x, a.y, b.z},
			{b.x, b.y, b.z},
			{a.x, b.y, b.z}
		};

		for (int i = 0; i < 8; i++) {
			vertices[i] = worldMatrix * vertices[i];
		}

		int indices[24] = {
			0, 1, 1, 2, 2, 3, 3, 0, // front
			4, 5, 5, 6, 6, 7, 7, 4, // back
			1, 5, 6, 2, 4, 0, 3, 7, // remaining lines
		};
		viz.renderer.NewObject();
		viz.renderer.Begin(GL_LINES);
		viz.renderer.Color3f(color);
		viz.renderer.Normal3f({ 0.0f, 0.0f, 0.0f });
		viz.renderer.TexCoord2f({ 0.0f, 0.0f });
		for (int i = 0; i < 24; i++) {
			viz.renderer.Position3f(vertices[indices[i]]);
		}
		viz.renderer.End();
	}

	void RendererGLES30::_vizBBoxCubeMap(const BoundingBoxf& bbox, Matrix4f& worldMatrix) {
		Vector3f a = bbox.minBounds;
		Vector3f b = bbox.maxBounds;
		Vector3f vertices[8] = {
			{a.x, a.y, a.z},
			{b.x, a.y, a.z},
			{b.x, b.y, a.z},
			{a.x, b.y, a.z},
			{a.x, a.y, b.z},
			{b.x, a.y, b.z},
			{b.x, b.y, b.z},
			{a.x, b.y, b.z}
		};

		Vector3f texcoords[8] = {
			{-1.0f, -1.0f, -1.0f },
			{ 1.0f, -1.0f, -1.0f },
			{ 1.0f,  1.0f, -1.0f },
			{-1.0f,  1.0f, -1.0f },
			{-1.0f, -1.0f,  1.0f },
			{ 1.0f, -1.0f,  1.0f },
			{ 1.0f,  1.0f,  1.0f },
			{-1.0f,  1.0f,  1.0f }
		};

		constexpr float lo = 0.2f;
		constexpr float hi = 1.0f;
		Color3f colors[6] = {
			{ hi, lo, lo },
			{ lo, hi, hi },
			{ lo, hi, lo },
			{ hi, lo, hi },
			{ lo, lo, hi },
			{ hi, hi, lo },
		};

		Vector3f normals[6] = {
			{ 1.0f,  0.0f,  0.0f},
			{-1.0f,  0.0f,  0.0f},
			{ 0.0f,  1.0f,  0.0f},
			{ 0.0f, -1.0f,  0.0f},
			{ 0.0f,  0.0f,  1.0f},
			{ 0.0f,  0.0f, -1.0f}
		};

		for (int i = 0; i < 8; i++) {
			vertices[i] = worldMatrix * vertices[i];
		}

		int indices[36] = {
			1, 5, 6, 6, 2, 1, // +X right
			0, 3, 7, 7, 4, 0, // -X left
			3, 2, 6, 6, 7, 3, // +Y up
			0, 4, 5, 5, 1, 0, // -Y down
			0, 1, 2, 2, 3, 0, // +Z front
			6, 5, 4, 4, 7, 6, // -Z back
		};

		viz.renderer.NewObject();
		viz.renderer.Begin(GL_TRIANGLES);
		viz.renderer.Normal3f({ 0.0f, 0.0f, 0.0f });
		int v = 0;
		for (int face = 0; face < 6; face++) {
			viz.renderer.Normal3f(normals[face]);
			viz.renderer.Color3f(colors[face]);
			for (int i = 0; i < 6; i++, v++) {
				viz.renderer.TexCoord3f(texcoords[indices[v]]);
				viz.renderer.Position3f(vertices[indices[v]]);
			}
		}
		viz.renderer.End();
	}

	void RendererGLES30::_vizDirToLight(Vector3f center, Vector3f dir, float radius, Color3f color) {
		viz.renderer.NewObject();
		viz.renderer.Begin(GL_LINES);
		viz.renderer.Color3f(color);
		viz.renderer.Normal3f({ 0.0f, 0.0f, 0.0f });
		viz.renderer.TexCoord2f({ 0.0f, 0.0f });
		const int SLICES = 16;
		const int STACKS = 8;
		float theta = 0.0f;
		const float dtheta = FX_F32_TWOPI / SLICES;
		const float dphi = FX_F32_PI / STACKS;
		Matrix4f invLookAt = Matrix4f::MakeLookAt(center + dir, center, { 0.0f, 1.0f, 0.0f });
		for (int stack = 0; stack < STACKS; stack++) {
			float phi = 0.0f;
			for (int slice = 0; slice <= SLICES; slice++) {
				Vector3f p;
				p.from_straight_theta_phi(theta, phi);
				viz.renderer.Position3f((radius * p) + center);
				p.from_straight_theta_phi(theta + dtheta, phi);
				viz.renderer.Position3f((radius * p) + center);
				p.from_straight_theta_phi(theta, phi);
				viz.renderer.Position3f((radius * p) + center);
				p.from_straight_theta_phi(theta, phi + dphi);
				viz.renderer.Position3f((radius * p) + center);
				phi += dphi;
			}
			theta += dtheta;
		}
		viz.renderer.End();
	}

	void RendererGLES30::_vizBall(const Vector3f& center, float radius, Color3f color) {
		viz.renderer.NewObject();
		viz.renderer.Begin(GL_LINES);
		viz.renderer.Color3f(color);
		viz.renderer.Normal3f({ 0.0f, 0.0f, 0.0f });
		viz.renderer.TexCoord2f({ 0.0f, 0.0f });
		const int SLICES = 16;
		const int STACKS = 8;
		float theta = 0.0f;
		const float dtheta = FX_F32_TWOPI / SLICES;
		const float dphi = FX_F32_PI / STACKS;
		for (int stack = 0; stack < STACKS; stack++) {
			float phi = 0.0f;
			for (int slice = 0; slice <= SLICES; slice++) {
				Vector3f p;
				p.from_straight_theta_phi(theta, phi);
				viz.renderer.Position3f((radius * p) + center);
				p.from_straight_theta_phi(theta + dtheta, phi);
				viz.renderer.Position3f((radius * p) + center);
				p.from_straight_theta_phi(theta, phi);
				viz.renderer.Position3f((radius * p) + center);
				p.from_straight_theta_phi(theta, phi + dphi);
				viz.renderer.Position3f((radius * p) + center);
				phi += dphi;
			}
			theta += dtheta;
		}
		viz.renderer.End();
	}

	bool RendererGLES30::_initVIZ() {
		if (viz.buffersBuilt) return true;

		viz.buffersBuilt = true;

		return viz.buffersBuilt;
	}

	void RendererGLES30::_renderVIZ() {
		constexpr unsigned VizSceneBoundingBoxId = 1;
		constexpr unsigned VizStartingId = 2;
		Hf::StopWatch stopwatch;
		if (!_initVIZ()) return;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		//viz.renderer.Render();
		viz.renderer.reset(true);

		// Render scene bounding box
		BoundingBoxf bbox = pSSG->getBoundingBox();
		Matrix4f identity;
		viz.renderer.SetCurrentObjectId(VizSceneBoundingBoxId);
		_vizBBox(bbox, identity, FxColors3::White);
		
		const static std::string maps[8] = {
			"AnisoLightColorMap0",
			"AnisoLightColorMap1",
			"AnisoLightColorMap2",
			"AnisoLightColorMap3",
			"AnisoLightColorMap4",
			"AnisoLightColorMap5",
			"AnisoLightColorMap6",
			"AnisoLightColorMap7"
		};
		unsigned objectId = VizStartingId;
		std::set<unsigned> sphlIds;
		int node = 1;
		for (auto& [k, n] : pSSG->nodes) {
			if (n->keyword() == std::string("sphl")) {
				viz.renderer.SetCurrentObjectId(objectId);
				objectId++;
				_vizBall(n->transform.col4().xyz(), 1.0f, FxColors3::Yellow);
				continue;
			}
			Matrix4f transform = n->transform * n->addlTransform;
			if (n->keyword() == std::string("anisoLight")) {
				int i = n->name_str().back() - '0';
				if (i < 0) i = 0;
				if (pRendererContext->textureCubes.count(maps[i])) {
					viz.renderer.SetCurrentObjectId(objectId);
					sphlIds.insert(objectId);
					objectId++;
					_vizBBoxCubeMap(n->bbox, transform);
				}
			}
			else {
				viz.renderer.SetCurrentObjectId(objectId);
				objectId++;
				_vizBBox(n->bbox, transform, FxColors3::Cyan);
			}
		}

		for (auto& [k, n] : pSSG->geometryGroups) {
			Matrix4f transform = n.transform * n.addlTransform;
			viz.renderer.SetCurrentObjectId(objectId);
			objectId++;
			_vizBBox(n.bbox, transform, FxColors3::Rose);
		}

		for (auto& [k, n] : pSSG->pointLights) {
			viz.renderer.SetCurrentObjectId(objectId);
			objectId++;
			_vizBall(n.ublock.position.xyz(), 0.5f, FxColors3::Yellow);
		}

		for (auto& [k, n] : pSSG->dirToLights) {
			Vector3f N = n.ublock.dirTo.xyz().normalize();
			Vector3f outThere = N * 95.0f;
			viz.renderer.SetCurrentObjectId(objectId);
			objectId++;
			viz.renderer.NewObject();
			viz.renderer.Begin(GL_LINES);
			viz.renderer.Color3f(n.color());
			viz.renderer.Position3f({ 0.0f, 0.0f, 0.0f });
			viz.renderer.Position3f(outThere);
			viz.renderer.End();
			_vizBall(outThere, 0.5f, n.color());
		}

		for (auto& [k, n] : pSSG->spheres) {
			viz.renderer.SetCurrentObjectId(objectId);
			objectId++;
			_vizBall(n.transform.col4().xyz(), 0.5f, FxColors3::Yellow);
		}

		viz.renderer.BuildBuffers();
		int sphlId = 0;
		for (unsigned i = 0; i < objectId; i++) {
			if (!viz.renderer.objectExists(i)) continue;

			if (sphlIds.count(i)) {
				pRendererContext->textureCubes[maps[sphlId]].bind(0);
				pRendererProgram->uniform1i("MapEnviroCube", 0);
				pRendererProgram->uniform1i("ShaderDebugChoice", 5);
				viz.renderer.RenderIf(i, -1, false);
				pRendererContext->textureCubes[maps[sphlId]].unbind();
				pRendererProgram->uniform1i("ShaderDebugChoice", 0);
				sphlId++;
			} else {
				viz.renderer.RenderIf(i, -1, false);
			}
		}

		pRendererConfig->metrics_viz_ms = stopwatch.Stop_msf();
	}

	void RendererGLES30::buildBuffers() {
		if (!validate()) return;
		scene.renderer.reset(true);
		//for (auto& [geoindex, geo] : pSSG->geometryGroups) {
			//scene.renderer.SetCurrentObjectId(geo.objectId);
			//scene.renderer.SetCurrentMtlLibId(geo.mtllibId);
			//scene.renderer.SetCurrentObjectName(geo.objectName);
			//scene.renderer.SetCurrentMtlLibName(geo.mtllibName);
			//scene.renderer.NewObject();
			//scene.renderer.DrawOBJ(pSSG->staticMeshes[geo.objectId]);
			//pSSG->staticMeshes[geo.objectId].Render(renderer);
			//HFLOGDEBUGFIRSTRUNCOUNTMSG(5, "NEED TO RENDER OBJ_STATIC_MODEL");
		//}
		for (auto& [id, mesh] : pSSG->staticMeshes) {
			scene.renderer.SetCurrentObjectId(id);
			scene.renderer.NewObject();
			scene.renderer.DrawOBJ(mesh);
		}
		//scene.renderer.AssignMaterialIds(pSSG->materialSystem);
		//scene.renderer.SetCurrentMtlLibName("");
		//scene.renderer.SetCurrentMtlLibId(0);
		scene.buffersBuilt = true;
	}
} // namespace Fluxions
