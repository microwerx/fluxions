// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2019 Jonathan Metzgar
// All rights reserved.
//
// This program_ is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program_ is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program_.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com
#include "pch.hpp"
#include <fluxions_gte_matrix_math.hpp>
#include <fluxions_renderer_gles30.hpp>
#include <fluxions_renderer_context.hpp>
#include <hatchetfish_stopwatch.hpp>

namespace Fluxions
{
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
			textureUnits.Add(id);
		}
	}

	void RendererGLES30::kill() {
		if (pSSG)
			pSSG = nullptr;

		if (pRendererConfig)
			pRendererConfig = nullptr;

		if (pProgram)
			pProgram = nullptr;

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
		pProgram = nullptr;
	}

	void RendererGLES30::setSceneGraph(SimpleSceneGraph* pSSG_) {
		if (pSSG == pSSG_) return;
		pSSG = pSSG_;
		scene.areBuffersBuilt = false;
	}

	SimpleSceneGraph* RendererGLES30::getSceneGraph() {
		return pSSG;
	}

	bool RendererGLES30::applyRenderConfig() {
		Hf::StopWatch stopwatch;
		if (!validate()) return false;

		if (pProgram->isLinked() == false)
			return false;

		int writeFBOCount = 0;
		for (auto& [k, fbo] : pRendererConfig->writeFBOs) {
			if (fbo->unusable()) break;
			fbo->use();
			glViewport(0, 0, fbo->width(), fbo->height());
			writeFBOCount++;
		}

		pProgram->use();
		scene.locs.getMaterialProgramLocations(*pProgram);

		for (auto& [k, fbo] : pRendererConfig->readFBOs) {
			if (fbo->unusable()) continue;
			for (auto& [type, rt] : fbo->renderTargets) {
				rt.unit = -1;
				if (rt.target == GL_RENDERBUFFER) continue;
				if (rt.mapName.empty() || rt.pGpuTexture->unusable()) continue;
				if (!pProgram->activeUniforms.count(rt.mapName)) continue;
				rt.unit = getTexUnit();
				rt.pGpuTexture->bind(rt.unit);
				pProgram->applyUniform(rt.mapName, rt.unit);
			}
		}

		if (!writeFBOCount) {
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

		if (pRendererConfig->clearColorBuffer) {
			glClearColor(pRendererConfig->clearColor.r, pRendererConfig->clearColor.g, pRendererConfig->clearColor.b, pRendererConfig->clearColor.a);
		}

		if (pRendererConfig->getClearBits())
			glClear(pRendererConfig->getClearBits());

		Matrix4f projectionMatrix_;
		Matrix4f cameraMatrix_;
		Matrix4f worldMatrix_;

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
		}

		if (pRendererConfig->useSceneCamera) {
			cameraMatrix_ *= pRendererConfig->preCameraMatrix;
			cameraMatrix_ *= pSSG->camera.viewMatrix;
			cameraMatrix_ *= pRendererConfig->postCameraMatrix;
		}
		else {
			cameraMatrix_ *= pRendererConfig->preCameraMatrix;
			cameraMatrix_ *= pRendererConfig->postCameraMatrix;
		}

		pRendererConfig->projectionMatrix = projectionMatrix_;
		pRendererConfig->cameraMatrix = cameraMatrix_;

		pProgram->applyUniform("ProjectionMatrix", (RendererUniform)projectionMatrix_);
		pProgram->applyUniform("CameraPosition", (RendererUniform)(cameraMatrix_.AsInverse()).col4());
		pProgram->applyUniform("CameraMatrix", (RendererUniform)cameraMatrix_);
		pProgram->applyUniform("WorldMatrix", (RendererUniform)worldMatrix_);

		for (auto& [map, t] : pRendererConfig->textures) {
			if (!pProgram->activeUniforms.count(map) || !t->usable()) {
				t->unit = -1;
				continue;
			}
			t->unit = getTexUnit();
			t->bind(t->unit);
			pProgram->applyUniform(map, t->unit);
		}
		pRendererConfig->metrics_apply_ms = stopwatch.Stop_msf();
		return true;
	}

	bool RendererGLES30::saveGLState() {
		gles30StateSnapshot.save();
		return true;
	}

	bool RendererGLES30::restoreGLState() {
		pProgram = nullptr;

		for (auto& [k, fbo] : pRendererConfig->writeFBOs) {
			fbo->unbind();
		}

		for (auto& [map, t] : pRendererConfig->textures) {
			if (t->unit < 0) continue;
			t->unbind();
			freeTexUnit(t->unit);
			t->unit = -1;
		}

		for (auto& [k, fbo] : pRendererConfig->readFBOs) {
			if (fbo->unusable()) continue;
			for (auto& [type, rt] : fbo->renderTargets) {
				if (rt.unit < 0) continue;
				rt.pGpuTexture->unbind();
				freeTexUnit(rt.unit);
				rt.unit = -1;
			}
		}

		gles30StateSnapshot.restore();

		return true;
	}

	void RendererGLES30::setRenderConfig(RendererConfig* rc) {
		if (!rc) return;

		pRendererConfig = rc;
		pProgram = nullptr;

		if (pContext && pContext->programs.count(rc->rc_program)) {
			pProgram = &pContext->programs[rc->rc_program];
		}
		//else if (pRendererConfig->useZOnly) {
		//	if (pRendererConfig->zShaderProgram != nullptr) {
		//		pProgram = pRendererConfig->zShaderProgram;
		//	}
		//}
		//else if (pRendererConfig->shaderProgram != nullptr) {
		//	pProgram = pRendererConfig->shaderProgram;
		//}
	}

	RendererConfig* RendererGLES30::getRenderConfig() {
		return pRendererConfig;
	}

	void RendererGLES30::setContext(RendererContext* pcontext) {
		pContext = pcontext;
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
		BufferObject(GLenum target, GLsizei size, const void* data, GLenum usage) {
			glGenBuffers(1, &buffer);
			if (buffer > 0) {
				this->target = target;
				glBindBuffer(target, buffer);
				glBufferData(target, size, data, usage);
				glBindBuffer(target, buffer);
			}
		}

		~BufferObject() {
			if (buffer > 0)
				glDeleteBuffers(1, &buffer);
		}

		void Bind() {
			glBindBuffer(target, buffer);
		}

		void Delete() {
			if (buffer > 0)
				glDeleteBuffers(1, &buffer);
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

			glGenVertexArrays(1, &vao);
			if (vao > 0) {
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER, abo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
				unsigned attribCount = 0;
				for (unsigned i = 0; i < 8; i++) {
					if (mesh.IsAttribEnabled(i)) {
						const char* name = mesh.GetAttribName(i);
						if (name == nullptr)
							continue;
						GLint loc = glGetAttribLocation(program, name);
						if (loc < 0) {
							//Hf::Log.warning("%s(): Program %i does not have attrib %s", __FUNCTION__, program_, name);
							continue;
						}
						glVertexAttribPointer(loc, 4, GL_FLOAT, mesh.IsAttribNormalized(i), sizeof(SimpleGeometryMesh::Vertex), cast_to_pointer(mesh.GetVertexOffset(i)));
						glEnableVertexAttribArray(loc);
						attribCount++;
					}
				}

				if (attribCount == 0) {
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
					glDeleteVertexArrays(1, &vao);
					vao = 0;
					return;
				}

				GLenum type = 0;
				switch (sizeof(SimpleGeometryMesh::Index)) {
				case 1:
					type = GL_UNSIGNED_BYTE;
					break;
				case 2:
					type = GL_UNSIGNED_SHORT;
					break;
				case 4:
					type = GL_UNSIGNED_INT;
					break;
				}
				surfaces.resize(mesh.GetSurfaces().size());
				unsigned i = 0;
				for (const auto& meshSurface : mesh.GetSurfaces()) {
					surfaces[i].mode = meshSurface.type;
					surfaces[i].count = meshSurface.count;
					surfaces[i].offset = cast_to_pointer(meshSurface.first * mesh.GetIndexSize());
					surfaces[i].type = type;
					i++;
				}

				//glBindBuffer(GL_ARRAY_BUFFER, 0);
				//glVertexArrayElementBuffer(vao, eabo_);
				glBindVertexArray(0);
			}
		}

		~VertexArrayObject() {
			if (vao > 0) {
				glDeleteVertexArrays(1, &vao);
			}
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

		pProgram->use();
		pProgram->applyUniform("WorldMatrix", (RendererUniform)modelViewMatrix);

		// create a vbo
		BufferObject abo_(GL_ARRAY_BUFFER, (GLsizei)mesh.GetVertexDataSize(), mesh.GetVertexData(), GL_STATIC_DRAW);
		BufferObject eabo_(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)mesh.GetIndexDataSize(), mesh.GetIndexData(), GL_STATIC_DRAW);

		VertexArrayObject vao(mesh, pProgram->getProgram(), abo_.buffer, eabo_.buffer);
		vao.Draw();
	}

	void RendererGLES30::renderCubeImages() {
		if (!scene.areBuffersBuilt)
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

		if (pRendererConfig->renderVIZ) {
			_renderVIZ();
		}

		if (pRendererConfig->renderPost) {
			_renderPost();
		}

		restoreGLState();
	}

	void RendererGLES30::applyGlobalSettingsToCurrentProgram() {
		if (scene.locs.enviroCubeMap >= 0)
			glUniform1i(scene.locs.enviroCubeMap, pSSG->environment.enviroColorMapUnit);
		if (scene.locs.enviroCubeMapAmount >= 0)
			glUniform1f(scene.locs.enviroCubeMapAmount, 0.0);
		if (scene.locs.pbskyCubeMap >= 0)
			glUniform1i(scene.locs.pbskyCubeMap, pSSG->environment.pbskyColorMapUnit);
		if (scene.locs.sunDirTo >= 0)
			glUniform3fv(scene.locs.sunDirTo, 1, pSSG->environment.curSunDirTo.const_ptr());
		if (scene.locs.sunColor >= 0)
			glUniform3fv(scene.locs.sunColor, 1, pSSG->environment.sunColor.const_ptr());
		if (scene.locs.sunSize >= 0)
			glUniform1f(scene.locs.sunSize, pSSG->environment.sunSize);
		if (scene.locs.sunShadowMap >= 0)
			glUniform1i(scene.locs.sunShadowMap, pSSG->environment.sunDepthMapUnit);
		if (scene.locs.sunColorMap >= 0)
			glUniform1i(scene.locs.sunColorMap, pSSG->environment.sunColorMapUnit);
	}

	void RendererGLES30::applySpheresToCurrentProgram() {
		std::vector<float> spherePositions;
		std::vector<float> sphereKe;
		int numSpheres = 0;
		for (auto sphIt = pSSG->spheres.begin(); sphIt != pSSG->spheres.end(); sphIt++) {
			if (spherePositions.size() > 8)
				break;
			Vector4f pos(0, 0, 0, 1);
			Vector4f radius(1, 0, 0, 1);
			pos = sphIt->second.transform * pos;
			radius = sphIt->second.transform * radius;
			radius = radius - pos;
			float length = radius.length();
			pos.w = length;
			SimpleMaterial* mtl = pSSG->materials.SetLibraryMaterial(sphIt->second.mtllibName, sphIt->second.mtlName);
			// only push spheres that are emissive...
			if (mtl) {
				spherePositions.push_back(pos.x);
				spherePositions.push_back(pos.y);
				spherePositions.push_back(pos.z);
				spherePositions.push_back(pos.w);

				Color3f Ke = mtl->Ke;
				sphereKe.push_back(Ke.r);
				sphereKe.push_back(Ke.g);
				sphereKe.push_back(Ke.b);
				sphereKe.push_back(1.0f);
				numSpheres++;
			}
		}
		if (numSpheres > 0) {
			if (scene.locs.sphere_array >= 0)
				glUniform4fv(scene.locs.sphere_array, numSpheres, &spherePositions[0]);
			if (scene.locs.sphere_Ke >= 0)
				glUniform4fv(scene.locs.sphere_Ke, numSpheres, &sphereKe[0]);
		}
		if (scene.locs.sphere_count >= 0)
			glUniform1i(scene.locs.sphere_count, numSpheres);
	}

	void RendererGLES30::render(RendererProgram* program_, bool useMaterials, bool useMaps, bool useZOnly, Matrix4f& projectionMatrix_, Matrix4f& cameraMatrix_) {
		//Matrix4f inverseCameraMatrix = cameraMatrix_.AsInverse();
		//Vector4f cameraPosition(0, 0, 0, 1);
		//cameraPosition = inverseCameraMatrix * cameraPosition;

		//RendererUniform InverseCameraMatrix = cameraMatrix_.AsInverse();
		//RendererUniform CameraMatrix = cameraMatrix_;
		//RendererUniform ProjectionMatrix = projectionMatrix_;
		//RendererUniform CameraPosition = cameraPosition;

		//program_->applyUniform("ProjectionMatrix", ProjectionMatrix);
		//program_->applyUniform("CameraMatrix", CameraMatrix);
		//program_->applyUniform("InverseCameraMatrix", (RendererUniform)cameraMatrix_.AsInverse());
		//program_->applyUniform("CameraPosition", CameraPosition);

		// apply each material separately (use the idea that material state changes are worse than geometry ones
		for (auto& [index, mtllib] : pSSG->materials) {
			GLuint mtllibId = pSSG->materials.GetLibraryId(mtllib.name);
			pSSG->materials.SetLibrary(mtllib.name);

			// loop through each material
			for (auto& [mtlId, mtl] : mtllib.mtls) {
				std::string mtlName = pSSG->materials.GetMaterialName(mtlId);
				pSSG->materials.SetMaterial(mtlName);

				if (useMaterials) applyMaterialToCurrentProgram(mtl, useMaps);

				// loop through each geometry object
				for (auto& [geoId, geo] : pSSG->geometry) {
					GLuint objectId = geo.objectId;
					GLuint groupId = 0;
					scene.renderer.ApplyIdToMtlNames(mtlName, mtlId);

					// Apply object specific uniforms like transformation matrices
					RendererUniform WorldMatrix;
					//RendererUniform SunShadowBiasMatrix;
					//RendererUniform SunShadowProjectionMatrix;
					//RendererUniform SunShadowViewMatrix;
					//RendererUniform SunShadowInverseViewMatrix;
					Matrix4f totalTransform = geo.transform * geo.addlTransform;
					WorldMatrix = totalTransform;

					//SunShadowBiasMatrix = pSSG->environment.sunShadowBiasMatrix;
					//SunShadowProjectionMatrix = pSSG->environment.sunShadowProjectionMatrix;
					//SunShadowViewMatrix = pSSG->environment.sunShadowViewMatrix;
					//SunShadowInverseViewMatrix = pSSG->environment.sunShadowInverseViewMatrix;

					program_->applyUniform("WorldMatrix", WorldMatrix);
					//program_->applyUniform("SunShadowBiasMatrix", SunShadowBiasMatrix);
					//program_->applyUniform("SunShadowProjectionMatrix", SunShadowProjectionMatrix);
					//program_->applyUniform("SunShadowViewMatrix", SunShadowViewMatrix);
					//program_->applyUniform("SunShadowInverseViewMatrix", SunShadowInverseViewMatrix);

					// Now iterate through each object and render it with this material
					if (objectId && groupId && mtllibId && mtlId)
						scene.renderer.RenderIf(objectId, groupId, mtllibId, mtlId, useZOnly);
					else
						scene.renderer.RenderIf(geo.objectName, "", geo.mtllibName, mtlName, useZOnly);
				}

				_sceneDisableCurrentTextures();
			}
		}
	}

	void RendererGLES30::applyMaterialToCurrentProgram(SimpleMaterial& mtl, bool useMaps) {
		GLuint unit = 1;

		if (useMaps) {
			if (!mtl.map_Ka.empty())
				scene.currentTextures["map_Ka"] = pSSG->materials.GetTextureMap(mtl.map_Ka);
			if (!mtl.map_Kd.empty())
				scene.currentTextures["map_Kd"] = pSSG->materials.GetTextureMap(mtl.map_Kd);
			if (!mtl.map_Ks.empty())
				scene.currentTextures["map_Ks"] = pSSG->materials.GetTextureMap(mtl.map_Ks);
			if (!mtl.map_Ke.empty())
				scene.currentTextures["map_Ke"] = pSSG->materials.GetTextureMap(mtl.map_Ke);
			if (!mtl.map_Ns.empty())
				scene.currentTextures["map_Ns"] = pSSG->materials.GetTextureMap(mtl.map_Ns);
			if (!mtl.map_Ns.empty())
				scene.currentTextures["map_Ni"] = pSSG->materials.GetTextureMap(mtl.map_Ni);
			if (!mtl.map_Tf.empty())
				scene.currentTextures["map_Tf"] = pSSG->materials.GetTextureMap(mtl.map_Tf);
			if (!mtl.map_Tr.empty())
				scene.currentTextures["map_Tr"] = pSSG->materials.GetTextureMap(mtl.map_Tr);
			if (!mtl.map_bump.empty())
				scene.currentTextures["map_bump"] = pSSG->materials.GetTextureMap(mtl.map_bump);
			if (!mtl.map_normal.empty())
				scene.currentTextures["map_normal"] = pSSG->materials.GetTextureMap(mtl.map_normal);
			if (!mtl.PBmap.empty())
				scene.currentTextures["PBmap"] = pSSG->materials.GetTextureMap(mtl.PBmap);

			for (auto& [mapName, pMap] : scene.currentTextures) {
				//for (auto tmapIt = currentTextures.begin(); tmapIt != currentTextures.end(); tmapIt++) {
				//	SimpleMap* pMap = tmapIt->second;
				if (pMap) {
					if (pMap->cached.unitId <= 0)
						pMap->cached.unitId = getTexUnit();
					// TODO: Renable texture mapping
					//pMap->cached.samplerId = pMap->cached.textureObject.getSamplerId();
					//pMap->cached.textureId = pMap->cached.textureObject.getTextureId();
					//pMap->cached.textureObject.bind(pMap->cached.unitId, false);
					//glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);

					//if (pMap->cached.samplerId == 0) {
					//	glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_WRAP_S, GL_REPEAT);
					//	glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_WRAP_T, GL_REPEAT);
					//	glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_WRAP_R, GL_REPEAT);
					//	glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					//	glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					//}

					pMap->cached.map_loc = -1;
					pMap->cached.map_mix_loc = -1;
					if (mapName == "map_Ka") {
						pMap->cached.map_loc = scene.locs.map_Ka;
						pMap->cached.map_mix_loc = scene.locs.map_Ka_mix;
					}
					else if (mapName == "map_Kd") {
						pMap->cached.map_loc = scene.locs.map_Kd;
						pMap->cached.map_mix_loc = scene.locs.map_Kd_mix;
					}
					else if (mapName == "map_Ks") {
						pMap->cached.map_loc = scene.locs.map_Ks;
						pMap->cached.map_mix_loc = scene.locs.map_Ks_mix;
					}
					else if (mapName == "map_Ke") {
						pMap->cached.map_loc = scene.locs.map_Ke;
						pMap->cached.map_mix_loc = scene.locs.map_Ke_mix;
					}
					else if (mapName == "map_Ni") {
						pMap->cached.map_loc = scene.locs.map_Ni;
						pMap->cached.map_mix_loc = scene.locs.map_Ni_mix;
					}
					else if (mapName == "map_Ns") {
						pMap->cached.map_loc = scene.locs.map_Ns;
						pMap->cached.map_mix_loc = scene.locs.map_Ns_mix;
					}
					else if (mapName == "map_Tf") {
						pMap->cached.map_loc = scene.locs.map_Tf;
						pMap->cached.map_mix_loc = scene.locs.map_Tf_mix;
					}
					else if (mapName == "map_Tr") {
						pMap->cached.map_loc = scene.locs.map_Tr;
						pMap->cached.map_mix_loc = scene.locs.map_Tr_mix;
					}
					else if (mapName == "map_bump") {
						pMap->cached.map_loc = scene.locs.map_bump;
						pMap->cached.map_mix_loc = scene.locs.map_bump_mix;
					}
					else if (mapName == "map_normal") {
						pMap->cached.map_loc = scene.locs.map_normal;
						pMap->cached.map_mix_loc = scene.locs.map_normal_mix;
					}
					else if (mapName == "PBmap") {
						pMap->cached.map_loc = scene.locs.PBmap;
						pMap->cached.map_mix_loc = scene.locs.PBmap_mix;
					}

					if (pMap->cached.unitId < 0 || pMap->cached.unitId >= g_MaxCombinedTextureUnits) {
						HFLOGERROR("pMap->unitId (%d) is out of range.", pMap->cached.unitId);
					}
					else {
						if (pMap->cached.map_loc >= 0)
							glUniform1i(pMap->cached.map_loc, pMap->cached.unitId);
						if (pMap->cached.map_mix_loc >= 0)
							glUniform1f(pMap->cached.map_mix_loc, 1.0f);
					}

					unit++;
				}
			}
		}

		// Apply Material Uniforms to the program_ shader
		if (scene.locs.Ka >= 0)
			glUniform3fv(scene.locs.Ka, 1, &mtl.Ka.r);
		if (scene.locs.Kd >= 0)
			glUniform3fv(scene.locs.Kd, 1, &mtl.Kd.r);
		if (scene.locs.Ks >= 0)
			glUniform3fv(scene.locs.Ks, 1, &mtl.Ks.r);
		if (scene.locs.Ke >= 0)
			glUniform3fv(scene.locs.Ke, 1, &mtl.Ke.r);
		if (scene.locs.Ni >= 0)
			glUniform1fv(scene.locs.Ni, 1, &mtl.Ni);
		if (scene.locs.Ns >= 0)
			glUniform1fv(scene.locs.Ns, 1, &mtl.Ns);
		if (scene.locs.Tf >= 0)
			glUniform3fv(scene.locs.Tf, 1, &mtl.Tf.r);
		if (scene.locs.Tr >= 0)
			glUniform1fv(scene.locs.Tr, 1, &mtl.Tr);
		if (scene.locs.PBKdm >= 0)
			glUniform1fv(scene.locs.PBKdm, 1, &mtl.PBKdm);
		if (scene.locs.PBKsm >= 0)
			glUniform1fv(scene.locs.PBKsm, 1, &mtl.PBKsm);
		if (scene.locs.PBior >= 0)
			glUniform1fv(scene.locs.PBior, 1, &mtl.PBior);
		if (scene.locs.PBGGXgamma >= 0)
			glUniform1fv(scene.locs.PBGGXgamma, 1, &mtl.PBGGXgamma);
		if (scene.locs.PBm >= 0)
			glUniform1fv(scene.locs.PBm, 1, &mtl.PBm);
		if (scene.locs.PBk >= 0)
			glUniform1fv(scene.locs.PBk, 1, &mtl.PBk);

		// The better way to do this is to first find all the locations for the material, then apply them...
		for (auto uniform : scene.locs.newLocationList) {
			// uniform.first = name
			// uniform.second = details
		}
	}

	void RendererGLES30::_sceneDisableCurrentTextures() {
		// Turn off textures and reset program_ unit bindings to 0
		for (auto tmapIt = scene.currentTextures.begin(); tmapIt != scene.currentTextures.end(); tmapIt++) {
			SimpleMap* pMap = tmapIt->second;
			if (pMap) {
				//FxBindTexture(pMap->cached.unitId, pMap->cached.textureObject.getTarget(), 0);
				// TODO: Bind Texture

				glUniform1i(pMap->cached.map_loc, 0);
				glUniform1f(pMap->cached.map_mix_loc, 0.0f);

				if (pMap->cached.unitId != 0) {
					freeTexUnit(pMap->cached.unitId);
					pMap->cached.unitId = 0;
				}
			}
		}
		FxSetActiveTexture(GL_TEXTURE0);

		scene.currentTextures.clear();
	}

	int RendererGLES30::getTexUnit() {
		return textureUnits.Create();
	}

	void RendererGLES30::freeTexUnit(int id) {
		textureUnits.Delete(id);
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
			glGenBuffers(1, &skybox.abo);
			glGenBuffers(1, &skybox.eabo);
			glBindBuffer(GL_ARRAY_BUFFER, skybox.abo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox.eabo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		}

		if (!pSkyboxCube && !renderskyboxname.empty()) {
			pSkyboxCube = &pContext->textureCubes[renderskyboxname];
		}

		return (skybox.abo && skybox.eabo && pSkyboxCube);
	}

	void RendererGLES30::_renderSkyBox() {
		Hf::StopWatch stopwatch;
		if (!_initSkyBox()) return;

		//Matrix4f skyboxCameraMatrix;
		//Matrix4f skyboxProjectionMatrix;
		//Matrix4f skyboxWorldMatrix;

		//static const std::string CameraMatrix{ "CameraMatrix" };
		//static const std::string ProjectionMatrix{ "ProjectionMatrix" };
		//static const std::string WorldMatrix{ "WorldMatrix" };

		//skyboxProjectionMatrix.PerspectiveY(90.0, 1.0, 1.0, 1000.0);
		//pSkyboxCube->bind(0);
		pProgram->use();
		//pProgram->applyUniform(pSkyboxCube->uniformname, 0);
		//pProgram->applyUniform(CameraMatrix, pSSG->camera.viewMatrix);
		//pProgram->applyUniform(ProjectionMatrix, pRendererConfig->projectionMatrix);
		//pProgram->applyUniform(WorldMatrix, skyboxWorldMatrix);

		const std::string VERTEX_LOCATION{ "aPosition" };
		const std::string TEXCOORD_LOCATION{ "aTexCoord" };
		int vloc = pProgram->getAttribLocation(VERTEX_LOCATION);
		int tloc = pProgram->getAttribLocation(TEXCOORD_LOCATION);

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
		glUseProgram(0);
		//pSkyboxCube->unbind();
		pRendererConfig->metrics_skybox_ms = stopwatch.Stop_msf();
	}

	bool RendererGLES30::_initPost() {
		if (post.usable_) return true;
		if (!pRendererConfig) return false;

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
			FxCreateBuffer(GL_ARRAY_BUFFER, post.abo, sizeof(buffer), buffer, GL_STATIC_DRAW);
		}

		post.program = pRendererConfig->rc_program_ptr->getProgram();
		post_units.clear();
		for (auto& [k, fbo] : pRendererConfig->readFBOs) {
			if (fbo->unusable()) continue;
			for (auto& [target, rt] : fbo->renderTargets) {
				if (rt.mapName.empty() || !rt.pGpuTexture) continue;
				if (!pProgram->activeUniforms.count(rt.mapName)) continue;
				post_units.add();
				post_units.unit(getTexUnit());
				post_units.target(rt.target);
				post_units.texture(rt.pGpuTexture->getTexture());
				post_units.uniform_location(pProgram->activeUniforms[rt.mapName].index);
			}
		}

		const std::string VERTEX_LOCATION{ "aPosition" };
		const std::string TEXCOORD_LOCATION{ "aTexCoord" };
		post.vloc = pProgram->getAttribLocation(VERTEX_LOCATION);
		post.tloc = pProgram->getAttribLocation(TEXCOORD_LOCATION);

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
		while (glGetError()) HFLOGERROR("1 ERRORS");

		glUseProgram(post.program);
		post_units.first();
		for (int i = 0; i < post_units.count; i++) {
			glUniform1i(post_units.uniform_location(), post_units.unit());
			glActiveTexture(GL_TEXTURE0 + post_units.unit());
			glBindTexture(post_units.target(), post_units.texture());
			post_units.next();
		}

		pProgram->applyUniform("ToneMapExposure", pRendererConfig->renderPostToneMapExposure);
		pProgram->applyUniform("ToneMapGamma", pRendererConfig->renderPostToneMapGamma);
		pProgram->applyUniform("FilmicHighlights", pRendererConfig->renderPostFilmicHighlights);
		pProgram->applyUniform("FilmicShadows", pRendererConfig->renderPostFilmicShadows);

		glBindBuffer(GL_ARRAY_BUFFER, post.abo);
		glVertexAttribPointer(post.vloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void*)0);
		if (post.tloc >= 0) glVertexAttribPointer(post.tloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void*)12);
		if (post.vloc >= 0) glEnableVertexAttribArray(post.vloc);
		if (post.tloc >= 0) glEnableVertexAttribArray(post.tloc);
		if (post.vloc >= 0) glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		if (post.vloc >= 0) glDisableVertexAttribArray(post.vloc);
		if (post.tloc >= 0) glDisableVertexAttribArray(post.tloc);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		while (glGetError()) HFLOGERROR("3 ERRORS");

		post_units.first();
		for (int i = 0; i < post_units.count; i++) {
			glActiveTexture(GL_TEXTURE0 + post_units.unit());
			glBindTexture(post_units.target(), 0);
			post_units.next();
		}
		while (glGetError()) HFLOGERROR("4 ERRORS");
		glUseProgram(0);
		pRendererConfig->metrics_posttime_ms = stopwatch.Stop_msf();
	}

	void RendererGLES30::_renderSceneGraph() {
		Hf::StopWatch stopwatch;
		if (!scene.areBuffersBuilt) buildBuffers();

		pProgram->use();
		applyGlobalSettingsToCurrentProgram();
		applySpheresToCurrentProgram();

		if (pRendererConfig->isCubeMap) {
			//projectionMatrix.LoadIdentity();
			//projectionMatrix.Perspective(90.0f, 1.0f,
			//							 pRendererConfig->viewportZNear,
			//							 pRendererConfig->viewportZFar);
			//Vector4f cameraPosition(0, 0, 0, 1);
			//cameraPosition = cameraMatrix * cameraPosition;
			//if (pRendererConfig->defaultCubeFace >= 0 && pRendererConfig->defaultCubeFace < 6) {
			//	cameraMatrix.LoadIdentity();
			//	cameraMatrix.CubeMatrix(GL_TEXTURE_CUBE_MAP_POSITIVE_X + pRendererConfig->defaultCubeFace);
			//	cameraMatrix.Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z);
			//	render(pProgram, pRendererConfig->useMaterials, pRendererConfig->useMaps, pRendererConfig->useZOnly, projectionMatrix, cameraMatrix);
			//}
			//else {
			//	for (int i = 0; i < 6; i++) {
			//		cameraMatrix.LoadIdentity();
			//		cameraMatrix.Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z);
			//		cameraMatrix.CubeMatrix(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
			//		render(pProgram, pRendererConfig->useMaterials, pRendererConfig->useMaps, pRendererConfig->useZOnly, projectionMatrix, cameraMatrix);
			//	}
			//}
		}
		else {
			Matrix4f cameraMatrix;
			Matrix4f projectionMatrix;
			render(pProgram, pRendererConfig->useMaterials, pRendererConfig->useMaps, pRendererConfig->useZOnly, projectionMatrix, cameraMatrix);
		}

		glUseProgram(0);
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

		BoundingBoxf bbox = pSSG->GetBoundingBox();
		_vizBBox(bbox, Matrix4f::MakeIdentity(), FxColors3::White);

		for (auto& [k, n] : pSSG->nodes) {
			if (n->keyword() == "sphl") {
				_vizBall(n->transform.col4().xyz(), 1.0f, FxColors3::Yellow);
				continue;
			}
			_vizBBox(n->bbox, n->transform * n->addlTransform, FxColors3::Cyan);
		}

		for (auto& [k, n] : pSSG->geometry) {
			_vizBBox(n.bbox, n.transform * n.addlTransform, FxColors3::Rose);
		}

		for (auto& [k, n] : pSSG->pointLights) {
			_vizBall(n.position, 0.5f, FxColors3::Yellow);
		}

		for (auto& [k, n] : pSSG->spheres) {
			_vizBall(n.transform.col4().xyz(), 0.5f, FxColors3::Yellow);
		}

		viz.buffersBuilt = true;

		return viz.buffersBuilt;
	}

	void RendererGLES30::_renderVIZ() {
		Hf::StopWatch stopwatch;
		if (!_initVIZ()) return;
		viz.renderer.Render();
		pRendererConfig->metrics_viz_ms = stopwatch.Stop_msf();
	}

	void RendererGLES30::buildBuffers() {
		if (!validate()) return;
		scene.renderer.reset();
		for (auto& [geoindex, geo] : pSSG->geometry) {
			scene.renderer.SetCurrentObjectId(geo.objectId);
			scene.renderer.SetCurrentMtlLibId(geo.mtllibId);
			scene.renderer.SetCurrentObjectName(geo.objectName);
			scene.renderer.SetCurrentMtlLibName(geo.mtllibName);
			scene.renderer.NewObject();
			scene.renderer.DrawOBJ(pSSG->geometryObjects[geo.objectId]);
			//pSSG->geometryObjects[geo.objectId].Render(renderer);
			//HFLOGDEBUGFIRSTRUNCOUNTMSG(5, "NEED TO RENDER OBJ_STATIC_MODEL");
		}
		scene.renderer.AssignMaterialIds(pSSG->materials);
		scene.renderer.SetCurrentMtlLibName("");
		scene.renderer.SetCurrentMtlLibId(0);
		scene.areBuffersBuilt = true;
	}
} // namespace Fluxions