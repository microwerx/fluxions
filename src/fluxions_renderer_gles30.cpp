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
#include <fluxions_renderer_gles30.hpp>
#include <fluxions_renderer_context.hpp>

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

		abo = 0;
		eabo = 0;
		pSSG = nullptr;
		pRenderConfig = nullptr;
		pProgram = nullptr;
		projectionMatrix.LoadIdentity();
		cameraMatrix.LoadIdentity();
	}

	void RendererGLES30::kill() {
		if (pSSG)
			pSSG = nullptr;

		if (pRenderConfig)
			pRenderConfig = nullptr;

		if (pProgram)
			pProgram = nullptr;

		if (abo) {
			glDeleteBuffers(1, &abo);
			abo = 0;
		}

		if (eabo) {
			glDeleteBuffers(1, &eabo);
			eabo = 0;
		}

		RendererObject::kill();
	}

	const char* RendererGLES30::type() const {
		return "RendererGLES30";
	}

	void RendererGLES30::setSceneGraph(SimpleSceneGraph* pSSG_) {
		if (pSSG == pSSG_) return;
		pSSG = pSSG_;
		areBuffersBuilt = false;
	}

	SimpleSceneGraph* RendererGLES30::getSceneGraph() {
		return pSSG;
	}

	bool RendererGLES30::applyRenderConfig() {
		if (!validate()) return false;

		if (pProgram->isLinked() == false)
			return false;

		if (pRenderConfig->renderToFBO)
			pRenderConfig->fbo.use();

		pProgram->use();
		locs.getMaterialProgramLocations(*pProgram);
		applyGlobalSettingsToCurrentProgram();
		applySpheresToCurrentProgram();

		if (pRenderConfig->enableDepthTest) {
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(pRenderConfig->depthComparisonTest);
		}

		if (pRenderConfig->clearColorBuffer) {
			glClearColor(pRenderConfig->clearColor.r, pRenderConfig->clearColor.g, pRenderConfig->clearColor.b, pRenderConfig->clearColor.a);
		}

		if (pRenderConfig->getClearBits())
			glClear(pRenderConfig->getClearBits());

		Matrix4f projectionMatrix_;
		Matrix4f cameraMatrix_;

		if (pRenderConfig->useSceneCamera) {
			projectionMatrix_.LoadIdentity();
			projectionMatrix_.PerspectiveY(
				(float)pSSG->camera.fov,
				pRenderConfig->viewportRect.aspectRatiof(),
				(float)pSSG->camera.imageNearZ,
				(float)pSSG->camera.imageFarZ);
			cameraMatrix_ = pRenderConfig->preCameraMatrix *
				pSSG->camera.viewMatrix * pRenderConfig->postCameraMatrix;
		}
		else {
			projectionMatrix_.LoadIdentity();
			projectionMatrix_.PerspectiveY((float)pRenderConfig->fov, pRenderConfig->viewportRect.aspectRatiof(), (float)pRenderConfig->znear, (float)pRenderConfig->zfar);
			cameraMatrix_ = pRenderConfig->preCameraMatrix * pRenderConfig->postCameraMatrix;
		}
		pRenderConfig->cameraMatrix = cameraMatrix_;

		glViewport(pRenderConfig->viewportRect.x, pRenderConfig->viewportRect.y, pRenderConfig->viewportRect.w, pRenderConfig->viewportRect.h);

		if (pRenderConfig->recomputeProjectionMatrix) {
			pRenderConfig->projectionMatrix.LoadIdentity();
			pRenderConfig->projectionMatrix.PerspectiveY(pRenderConfig->fov, (float)pRenderConfig->viewportRect.aspectRatio(), pRenderConfig->znear, pRenderConfig->zfar);
		}
		projectionMatrix_ = pRenderConfig->projectionMatrix;

		pProgram->applyUniform("CameraPosition", (RendererUniform)(cameraMatrix_.AsInverse()).col4());
		pProgram->applyUniform("CameraMatrix", (RendererUniform)cameraMatrix_);
		pProgram->applyUniform("ProjectionMatrix", (RendererUniform)projectionMatrix_);

		return true;
	}

	bool RendererGLES30::saveGLState() {
		gles30StateSnapshot.save();
		return true;
	}

	bool RendererGLES30::restoreGLState() {
		pProgram = nullptr;

		if (pRenderConfig->renderToFBO)
			pRenderConfig->fbo.restoreGLState();

		gles30StateSnapshot.restore();

		return true;
	}

	void RendererGLES30::setRenderConfig(RendererConfig* rc) {
		if (!rc) return;

		pRenderConfig = rc;
		pProgram = nullptr;

		if (pContext && pContext->programs.count(rc->rc_program)) {
			pProgram = &pContext->programs[rc->rc_program];
		}
		//else if (pRenderConfig->useZOnly) {
		//	if (pRenderConfig->zShaderProgram != nullptr) {
		//		pProgram = pRenderConfig->zShaderProgram;
		//	}
		//}
		//else if (pRenderConfig->shaderProgram != nullptr) {
		//	pProgram = pRenderConfig->shaderProgram;
		//}
	}

	RendererConfig* RendererGLES30::getRenderConfig() {
		return pRenderConfig;
	}

	void RendererGLES30::setContext(RendererContext* pcontext) {
		pContext = pcontext;
	}

	void RendererGLES30::render() {
		if (!validate()) return;
		if (pRenderConfig->isCubeMap)
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

		pRenderConfig->shaderProgram->use();
		pRenderConfig->shaderProgram->applyUniform("ModelViewMatrix", (RendererUniform)modelViewMatrix);
		pRenderConfig->shaderProgram->applyUniform("WorldMatrix", (RendererUniform)modelViewMatrix);

		// create a vbo
		BufferObject abo_(GL_ARRAY_BUFFER, (GLsizei)mesh.GetVertexDataSize(), mesh.GetVertexData(), GL_STATIC_DRAW);
		BufferObject eabo_(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)mesh.GetIndexDataSize(), mesh.GetIndexData(), GL_STATIC_DRAW);

		VertexArrayObject vao(mesh, pRenderConfig->shaderProgram->getProgram(), abo_.buffer, eabo_.buffer);
		vao.Draw();
	}

	void RendererGLES30::renderCubeImages() {
		if (!areBuffersBuilt)
			buildBuffers();

		GLsizei s = pRenderConfig->viewportRect.w;

		pRenderConfig->viewportRect.x = 0;
		pRenderConfig->viewportRect.y = s;
		pRenderConfig->defaultCubeFace = 1;
		glClear(GL_DEPTH_BUFFER_BIT);
		renderSingleImage();

		pRenderConfig->viewportRect.x = 2 * s;
		pRenderConfig->viewportRect.y = s;
		pRenderConfig->defaultCubeFace = 0;
		glClear(GL_DEPTH_BUFFER_BIT);
		renderSingleImage();

		pRenderConfig->viewportRect.x = s;
		pRenderConfig->viewportRect.y = 2 * s;
		pRenderConfig->defaultCubeFace = 2;
		glClear(GL_DEPTH_BUFFER_BIT);
		renderSingleImage();

		pRenderConfig->viewportRect.x = s;
		pRenderConfig->viewportRect.y = 0;
		pRenderConfig->defaultCubeFace = 3;
		glClear(GL_DEPTH_BUFFER_BIT);
		renderSingleImage();

		pRenderConfig->viewportRect.x = s;
		pRenderConfig->viewportRect.y = s;
		pRenderConfig->defaultCubeFace = 4;
		glClear(GL_DEPTH_BUFFER_BIT);
		renderSingleImage();

		pRenderConfig->viewportRect.x = 3 * s;
		pRenderConfig->viewportRect.y = s;
		pRenderConfig->defaultCubeFace = 5;
		glClear(GL_DEPTH_BUFFER_BIT);
		renderSingleImage();
	}

	void RendererGLES30::renderSingleImage() {
		if (!areBuffersBuilt)
			buildBuffers();

		saveGLState();

		if (!applyRenderConfig())
			return;

		if (pRenderConfig->isCubeMap) {
			projectionMatrix.LoadIdentity();
			projectionMatrix.Perspective(90.0f, 1.0f, pRenderConfig->znear, pRenderConfig->zfar);
			Vector4f cameraPosition(0, 0, 0, 1);
			cameraPosition = cameraMatrix * cameraPosition;
			if (pRenderConfig->defaultCubeFace >= 0 && pRenderConfig->defaultCubeFace < 6) {
				cameraMatrix.LoadIdentity();
				cameraMatrix.CubeMatrix(GL_TEXTURE_CUBE_MAP_POSITIVE_X + pRenderConfig->defaultCubeFace);
				cameraMatrix.Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z);
				render(pProgram, pRenderConfig->useMaterials, pRenderConfig->useMaps, pRenderConfig->useZOnly, projectionMatrix, cameraMatrix);
			}
			else {
				for (int i = 0; i < 6; i++) {
					cameraMatrix.LoadIdentity();
					cameraMatrix.Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z);
					cameraMatrix.CubeMatrix(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
					render(pProgram, pRenderConfig->useMaterials, pRenderConfig->useMaps, pRenderConfig->useZOnly, projectionMatrix, cameraMatrix);
				}
			}
		}
		else {
			render(pProgram, pRenderConfig->useMaterials, pRenderConfig->useMaps, pRenderConfig->useZOnly, projectionMatrix, cameraMatrix);
		}

		glUseProgram(0);

		if (pRenderConfig->renderSkyBox) {
			glEnable(GL_DEPTH_TEST);
			renderSkyBox();
			glDisable(GL_DEPTH_TEST);
		}

		restoreGLState();
	}

	void RendererGLES30::applyGlobalSettingsToCurrentProgram() {
		if (locs.enviroCubeMap >= 0)
			glUniform1i(locs.enviroCubeMap, pSSG->environment.enviroColorMapUnit);
		if (locs.enviroCubeMapAmount >= 0)
			glUniform1f(locs.enviroCubeMapAmount, 0.0);
		if (locs.pbskyCubeMap >= 0)
			glUniform1i(locs.pbskyCubeMap, pSSG->environment.pbskyColorMapUnit);
		if (locs.sunDirTo >= 0)
			glUniform3fv(locs.sunDirTo, 1, pSSG->environment.curSunDirTo.const_ptr());
		if (locs.sunColor >= 0)
			glUniform3fv(locs.sunColor, 1, pSSG->environment.sunColor.const_ptr());
		if (locs.sunSize >= 0)
			glUniform1f(locs.sunSize, pSSG->environment.sunSize);
		if (locs.sunShadowMap >= 0)
			glUniform1i(locs.sunShadowMap, pSSG->environment.sunDepthMapUnit);
		if (locs.sunColorMap >= 0)
			glUniform1i(locs.sunColorMap, pSSG->environment.sunColorMapUnit);
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
			if (locs.sphere_array >= 0)
				glUniform4fv(locs.sphere_array, numSpheres, &spherePositions[0]);
			if (locs.sphere_Ke >= 0)
				glUniform4fv(locs.sphere_Ke, numSpheres, &sphereKe[0]);
		}
		if (locs.sphere_count >= 0)
			glUniform1i(locs.sphere_count, numSpheres);
	}

	void RendererGLES30::render(RendererProgram* program_, bool useMaterials, bool useMaps, bool useZOnly, Matrix4f& projectionMatrix_, Matrix4f& cameraMatrix_) {
		Matrix4f inverseCameraMatrix = cameraMatrix_.AsInverse();
		Vector4f cameraPosition(0, 0, 0, 1);
		cameraPosition = inverseCameraMatrix * cameraPosition;

		RendererUniform InverseCameraMatrix = cameraMatrix_.AsInverse();
		RendererUniform CameraMatrix = cameraMatrix_;
		RendererUniform ProjectionMatrix = projectionMatrix_;
		RendererUniform CameraPosition = cameraPosition;

		program_->applyUniform("ProjectionMatrix", ProjectionMatrix);
		program_->applyUniform("CameraMatrix", CameraMatrix);
		program_->applyUniform("InverseCameraMatrix", (RendererUniform)cameraMatrix_.AsInverse());
		program_->applyUniform("CameraPosition", CameraPosition);

		// apply each material separately (use the idea that material state changes are worse than geometry ones
		for (auto libIt = pSSG->materials.begin(); libIt != pSSG->materials.end(); libIt++) {
			SimpleMaterialLibrary& mtllib = libIt->second;
			std::string mtllibName = mtllib.name;
			GLuint mtllibId = pSSG->materials.GetLibraryId(mtllib.name);
			pSSG->materials.SetLibrary(mtllib.name);

			// loop through each material
			for (auto mtlIt = mtllib.mtls.begin(); mtlIt != mtllib.mtls.end(); mtlIt++) {
				GLuint mtlId = mtlIt->first;
				std::string mtlName = pSSG->materials.GetMaterialName(mtlId);
				SimpleMaterial& mtl = mtlIt->second;
				pSSG->materials.SetMaterial(mtlName);

				if (useMaterials)
					applyMaterialToCurrentProgram(mtl, useMaps);

				// loop through each geometry object
				for (auto geoIt = pSSG->geometry.begin(); geoIt != pSSG->geometry.end(); geoIt++) {
					SimpleGeometryGroup& geo = geoIt->second;
					GLuint objectId = geo.objectId;
					GLuint groupId = 0;
					renderer.ApplyIdToMtlNames(mtlName, mtlIt->first);

					// Apply object specific uniforms like transformation matrices
					RendererUniform ModelViewMatrix;
					RendererUniform SunShadowBiasMatrix;
					RendererUniform SunShadowProjectionMatrix;
					RendererUniform SunShadowViewMatrix;
					RendererUniform SunShadowInverseViewMatrix;
					Matrix4f totalTransform = geo.transform * geo.addlTransform;
					ModelViewMatrix = totalTransform;

					SunShadowBiasMatrix = pSSG->environment.sunShadowBiasMatrix;
					SunShadowProjectionMatrix = pSSG->environment.sunShadowProjectionMatrix;
					SunShadowViewMatrix = pSSG->environment.sunShadowViewMatrix;
					SunShadowInverseViewMatrix = pSSG->environment.sunShadowInverseViewMatrix;

					program_->applyUniform("ModelViewMatrix", ModelViewMatrix);
					program_->applyUniform("SunShadowBiasMatrix", SunShadowBiasMatrix);
					program_->applyUniform("SunShadowProjectionMatrix", SunShadowProjectionMatrix);
					program_->applyUniform("SunShadowViewMatrix", SunShadowViewMatrix);
					program_->applyUniform("SunShadowInverseViewMatrix", SunShadowInverseViewMatrix);

					// Now iterate through each object and render it with this material
					if (objectId && groupId && mtllibId && mtlId)
						renderer.RenderIf(objectId, groupId, mtllibId, mtlId, useZOnly);
					else
						renderer.RenderIf(geo.objectName, "", geo.mtllibName, mtlName, useZOnly);
				}

				disableCurrentTextures();
			}
		}
	}

	void RendererGLES30::applyMaterialToCurrentProgram(SimpleMaterial& mtl, bool useMaps) {
		GLuint unit = 1;

		if (useMaps) {
			if (!mtl.map_Ka.empty())
				currentTextures["map_Ka"] = pSSG->materials.GetTextureMap(mtl.map_Ka);
			if (!mtl.map_Kd.empty())
				currentTextures["map_Kd"] = pSSG->materials.GetTextureMap(mtl.map_Kd);
			if (!mtl.map_Ks.empty())
				currentTextures["map_Ks"] = pSSG->materials.GetTextureMap(mtl.map_Ks);
			if (!mtl.map_Ke.empty())
				currentTextures["map_Ke"] = pSSG->materials.GetTextureMap(mtl.map_Ke);
			if (!mtl.map_Ns.empty())
				currentTextures["map_Ns"] = pSSG->materials.GetTextureMap(mtl.map_Ns);
			if (!mtl.map_Ns.empty())
				currentTextures["map_Ni"] = pSSG->materials.GetTextureMap(mtl.map_Ni);
			if (!mtl.map_Tf.empty())
				currentTextures["map_Tf"] = pSSG->materials.GetTextureMap(mtl.map_Tf);
			if (!mtl.map_Tr.empty())
				currentTextures["map_Tr"] = pSSG->materials.GetTextureMap(mtl.map_Tr);
			if (!mtl.map_bump.empty())
				currentTextures["map_bump"] = pSSG->materials.GetTextureMap(mtl.map_bump);
			if (!mtl.map_normal.empty())
				currentTextures["map_normal"] = pSSG->materials.GetTextureMap(mtl.map_normal);
			if (!mtl.PBmap.empty())
				currentTextures["PBmap"] = pSSG->materials.GetTextureMap(mtl.PBmap);

			for (auto tmapIt = currentTextures.begin(); tmapIt != currentTextures.end(); tmapIt++) {
				SimpleMap* pMap = tmapIt->second;
				if (pMap) {
					if (pMap->cached.unitId <= 0)
						pMap->cached.unitId = getTexUnit();
					pMap->cached.samplerId = pMap->cached.textureObject.getSamplerId();
					pMap->cached.textureId = pMap->cached.textureObject.getTextureId();
					pMap->cached.textureObject.bind(pMap->cached.unitId, false);
					glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);

					if (pMap->cached.samplerId == 0) {
						glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_WRAP_T, GL_REPEAT);
						glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_WRAP_R, GL_REPEAT);
						glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameterf(pMap->cached.textureObject.getTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					}

					pMap->cached.map_loc = -1;
					pMap->cached.map_mix_loc = -1;
					if (tmapIt->first == "map_Ka") {
						pMap->cached.map_loc = locs.map_Ka;
						pMap->cached.map_mix_loc = locs.map_Ka_mix;
					}
					else if (tmapIt->first == "map_Kd") {
						pMap->cached.map_loc = locs.map_Kd;
						pMap->cached.map_mix_loc = locs.map_Kd_mix;
					}
					else if (tmapIt->first == "map_Ks") {
						pMap->cached.map_loc = locs.map_Ks;
						pMap->cached.map_mix_loc = locs.map_Ks_mix;
					}
					else if (tmapIt->first == "map_Ke") {
						pMap->cached.map_loc = locs.map_Ke;
						pMap->cached.map_mix_loc = locs.map_Ke_mix;
					}
					else if (tmapIt->first == "map_Ni") {
						pMap->cached.map_loc = locs.map_Ni;
						pMap->cached.map_mix_loc = locs.map_Ni_mix;
					}
					else if (tmapIt->first == "map_Ns") {
						pMap->cached.map_loc = locs.map_Ns;
						pMap->cached.map_mix_loc = locs.map_Ns_mix;
					}
					else if (tmapIt->first == "map_Tf") {
						pMap->cached.map_loc = locs.map_Tf;
						pMap->cached.map_mix_loc = locs.map_Tf_mix;
					}
					else if (tmapIt->first == "map_Tr") {
						pMap->cached.map_loc = locs.map_Tr;
						pMap->cached.map_mix_loc = locs.map_Tr_mix;
					}
					else if (tmapIt->first == "map_bump") {
						pMap->cached.map_loc = locs.map_bump;
						pMap->cached.map_mix_loc = locs.map_bump_mix;
					}
					else if (tmapIt->first == "map_normal") {
						pMap->cached.map_loc = locs.map_normal;
						pMap->cached.map_mix_loc = locs.map_normal_mix;
					}
					else if (tmapIt->first == "PBmap") {
						pMap->cached.map_loc = locs.PBmap;
						pMap->cached.map_mix_loc = locs.PBmap_mix;
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
		if (locs.Ka >= 0)
			glUniform3fv(locs.Ka, 1, &mtl.Ka.r);
		if (locs.Kd >= 0)
			glUniform3fv(locs.Kd, 1, &mtl.Kd.r);
		if (locs.Ks >= 0)
			glUniform3fv(locs.Ks, 1, &mtl.Ks.r);
		if (locs.Ke >= 0)
			glUniform3fv(locs.Ke, 1, &mtl.Ke.r);
		if (locs.Ni >= 0)
			glUniform1fv(locs.Ni, 1, &mtl.Ni);
		if (locs.Ns >= 0)
			glUniform1fv(locs.Ns, 1, &mtl.Ns);
		if (locs.Tf >= 0)
			glUniform3fv(locs.Tf, 1, &mtl.Tf.r);
		if (locs.Tr >= 0)
			glUniform1fv(locs.Tr, 1, &mtl.Tr);
		if (locs.PBKdm >= 0)
			glUniform1fv(locs.PBKdm, 1, &mtl.PBKdm);
		if (locs.PBKsm >= 0)
			glUniform1fv(locs.PBKsm, 1, &mtl.PBKsm);
		if (locs.PBior >= 0)
			glUniform1fv(locs.PBior, 1, &mtl.PBior);
		if (locs.PBGGXgamma >= 0)
			glUniform1fv(locs.PBGGXgamma, 1, &mtl.PBGGXgamma);
		if (locs.PBm >= 0)
			glUniform1fv(locs.PBm, 1, &mtl.PBm);
		if (locs.PBk >= 0)
			glUniform1fv(locs.PBk, 1, &mtl.PBk);

		// The better way to do this is to first find all the locations for the material, then apply them...
		for (auto uniform : locs.newLocationList) {
			// uniform.first = name
			// uniform.second = details
		}
	}

	void RendererGLES30::disableCurrentTextures() {
		// Turn off textures and reset program_ unit bindings to 0
		for (auto tmapIt = currentTextures.begin(); tmapIt != currentTextures.end(); tmapIt++) {
			SimpleMap* pMap = tmapIt->second;
			if (pMap) {
				FxBindTexture(pMap->cached.unitId, pMap->cached.textureObject.getTarget(), 0);

				glUniform1i(pMap->cached.map_loc, 0);
				glUniform1f(pMap->cached.map_mix_loc, 0.0f);

				if (pMap->cached.unitId != 0) {
					freeTexUnit(pMap->cached.unitId);
					pMap->cached.unitId = 0;
				}
			}
		}
		FxSetActiveTexture(GL_TEXTURE0);

		currentTextures.clear();
	}

	GLuint RendererGLES30::getTexUnit() {
		return textureUnits.Create();
	}

	void RendererGLES30::freeTexUnit(GLuint id) {
		textureUnits.Delete(id);
	}

	void RendererGLES30::initSkyBox() {
		GLfloat size = 50.0f;
		// GLfloat v[] = {
		//     -size, size, -size,
		//     size, size, -size,
		//     size, -size, -size,
		//     -size, -size, -size,
		//     size, size, size,
		//     -size, size, size,
		//     -size, -size, size,
		//     size, -size, size};

		// GLfloat texcoords[] = {
		//     -1.0f, 1.0f, -1.0f,
		//     1.0f, 1.0f, -1.0f,
		//     1.0f, -1.0f, -1.0f,
		//     -1.0f, -1.0f, -1.0f,
		//     1.0f, 1.0f, 1.0f,
		//     -1.0f, 1.0f, 1.0f,
		//     -1.0f, -1.0f, 1.0f,
		//     1.0f, -1.0f, 1.0f};

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

		if (abo == 0) {
			glGenBuffers(1, &abo);
			glGenBuffers(1, &eabo);
			glBindBuffer(GL_ARRAY_BUFFER, abo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		}
	}

	void RendererGLES30::renderSkyBox() {
		if (renderskyboxname.empty()) return;

		Matrix4f skyboxWorldMatrix;

		if (!pSkyboxCube) {
			pSkyboxCube = &pContext->textureCubes[renderskyboxname];
			return;
		}
		else {
			pSkyboxCube->bind(0);
		}

		pProgram->use();
		pProgram->applyUniform(pSkyboxCube->uniformname, 1);
		pProgram->applyUniform("CameraMatrix", pSSG->camera.actualViewMatrix);
		pProgram->applyUniform("ProjectionMatrix", pSSG->camera.projectionMatrix);
		pProgram->applyUniform("WorldMatrix", skyboxWorldMatrix);

		const std::string VERTEX_LOCATION{ "aPosition" };
		const std::string TEXCOORD_LOCATION{ "aTexCoord" };
		int vloc = pProgram->getAttribLocation(VERTEX_LOCATION);
		int tloc = pProgram->getAttribLocation(TEXCOORD_LOCATION);

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

	void RendererGLES30::buildBuffers() {
		if (!validate()) return;
		renderer.reset();
		for (auto it = pSSG->geometry.begin(); it != pSSG->geometry.end(); it++) {
			SimpleGeometryGroup& geo = it->second;
			renderer.SetCurrentObjectId(geo.objectId);
			renderer.SetCurrentMtlLibId(geo.mtllibId);
			renderer.SetCurrentObjectName(geo.objectName);
			renderer.SetCurrentMtlLibName(geo.mtllibName);
			renderer.NewObject();
			// pSSG->geometryObjects[geo.objectId].Render(renderer);
			HFLOGDEBUGFIRSTRUNCOUNTMSG(5, "NEED TO RENDER OBJ_STATIC_MODEL");
		}
		renderer.AssignMaterialIds(pSSG->materials);
		renderer.SetCurrentMtlLibName("");
		renderer.SetCurrentMtlLibId(0);
		initSkyBox();
		areBuffersBuilt = true;
	}
} // namespace Fluxions