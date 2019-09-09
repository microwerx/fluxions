// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
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
#include "pch.h"
#include <fluxions_simple_gles30_renderer.hpp>

namespace Fluxions
{

SimpleGLES30Renderer::SimpleGLES30Renderer()
{
}

SimpleGLES30Renderer::SimpleGLES30Renderer(const SimpleRenderConfiguration &rc)
{
    SetRenderConfig(rc);
}

SimpleGLES30Renderer::~SimpleGLES30Renderer()
{
    if (abo)
        glDeleteBuffers(1, &abo);
    if (eabo)
        glDeleteBuffers(1, &eabo);
}

void SimpleGLES30Renderer::SetSceneGraph(SimpleSceneGraph &ssg_)
{
    ssg = ssg_;
    areBuffersBuilt = false;
}

bool SimpleGLES30Renderer::ApplyRenderConfig()
{
    if (renderConfig.shaderProgram == nullptr)
        return false;
    if (renderConfig.useZOnly && renderConfig.zShaderProgram == nullptr)
        return false;

    program = renderConfig.shaderProgram;

    if (renderConfig.useZOnly)
    {
        program = renderConfig.zShaderProgram;
    }

    if (program->IsLinked() == false)
        return false;

    if (renderConfig.renderToFBO)
        renderConfig.fbo.Use();

    program->Use();
    locs.GetMaterialProgramLocations(*program);
    ApplyGlobalSettingsToCurrentProgram();
    ApplySpheresToCurrentProgram();

    if (renderConfig.enableDepthTest)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(renderConfig.depthComparisonTest);
    }

    if (renderConfig.clearColorBuffer)
    {
        glClearColor(renderConfig.clearColor.r, renderConfig.clearColor.g, renderConfig.clearColor.b, renderConfig.clearColor.a);
    }

    if (renderConfig.GetClearBits())
        glClear(renderConfig.GetClearBits());

    Matrix4f projectionMatrix_;
    Matrix4f cameraMatrix_;

    if (renderConfig.useSceneCamera)
    {
        projectionMatrix_.LoadIdentity();
        projectionMatrix_.PerspectiveY((float)ssg.camera.fov, renderConfig.viewportRect.aspectRatiof(), (float)ssg.camera.imageNearZ, (float)ssg.camera.imageFarZ);
        cameraMatrix_ = renderConfig.preCameraMatrix * ssg.camera.viewMatrix * renderConfig.postCameraMatrix;
    }
    else
    {
        projectionMatrix_.LoadIdentity();
        projectionMatrix_.PerspectiveY((float)renderConfig.fov, renderConfig.viewportRect.aspectRatiof(), (float)renderConfig.znear, (float)renderConfig.zfar);
        cameraMatrix_ = renderConfig.preCameraMatrix * renderConfig.postCameraMatrix;
    }
    renderConfig.cameraMatrix = cameraMatrix_;

    glViewport(renderConfig.viewportRect.x, renderConfig.viewportRect.y, renderConfig.viewportRect.w, renderConfig.viewportRect.h);

    if (renderConfig.recomputeProjectionMatrix)
    {
        renderConfig.projectionMatrix.LoadIdentity();
        renderConfig.projectionMatrix.PerspectiveY(renderConfig.fov, (float)renderConfig.viewportRect.aspectRatio(), renderConfig.znear, renderConfig.zfar);
    }
    projectionMatrix_ = renderConfig.projectionMatrix;

    program->ApplyUniform("CameraPosition", (SimpleUniform)(cameraMatrix_.AsInverse()).col4());
    program->ApplyUniform("CameraMatrix", (SimpleUniform)cameraMatrix_);
    program->ApplyUniform("ProjectionMatrix", (SimpleUniform)projectionMatrix_);

    return true;
}

bool SimpleGLES30Renderer::SaveGLState()
{
    gles30StateSnapshot.Save();
    return true;
}

bool SimpleGLES30Renderer::RestoreGLState()
{
    program = nullptr;

    if (renderConfig.renderToFBO)
        renderConfig.fbo.RestoreGLState();

    gles30StateSnapshot.Restore();

    //glUseProgram(0);

    //if (renderConfig.clearColorBuffer)
    //{
    //	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //}

    //if (renderConfig.enableDepthTest)
    //{
    //	glDisable(GL_DEPTH_TEST);
    //	glDepthFunc(GL_LESS);
    //}

    //if (renderConfig.enableBlend)
    //{
    //	glDisable(GL_BLEND);
    //	glBlendFunc(GL_ONE, GL_ZERO);
    //}

    return true;
}

void SimpleGLES30Renderer::SetRenderConfig(const SimpleRenderConfiguration &rc)
{
    renderConfig = rc;
}

void SimpleGLES30Renderer::Render()
{
    if (renderConfig.isCubeMap)
        RenderCubeImages();
    else
        RenderSingleImage();
}

class BufferObject
{
  public:
    GLuint buffer = 0;
    GLenum target = 0;

  public:
    BufferObject(GLenum target, GLsizei size, const void *data, GLenum usage)
    {
        glGenBuffers(1, &buffer);
        if (buffer > 0)
        {
            this->target = target;
            glBindBuffer(target, buffer);
            glBufferData(target, size, data, usage);
            glBindBuffer(target, buffer);
        }
    }

    ~BufferObject()
    {
        if (buffer > 0)
            glDeleteBuffers(1, &buffer);
    }

    void Bind()
    {
        glBindBuffer(target, buffer);
    }

    void Delete()
    {
        if (buffer > 0)
            glDeleteBuffers(1, &buffer);
    }
};

class VertexArrayObject
{
  private:
    GLuint vao = 0;
    GLuint abo = 0;
    GLuint eabo = 0;

    struct Surface
    {
        GLenum mode;
        GLsizei count;
        void *offset;
        GLenum type;
    };

    std::vector<Surface> surfaces;

  public:
    VertexArrayObject(const SimpleGeometryMesh &mesh, GLuint program, GLuint abo, GLuint eabo)
    {
        this->abo = abo;
        this->eabo = eabo;

        glGenVertexArrays(1, &vao);
        if (vao > 0)
        {
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, abo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
            int count = 0;
            for (int i = 0; i < 8; i++)
            {
                if (mesh.IsAttribEnabled(i))
                {
                    const char *name = mesh.GetAttribName(i);
                    if (name == nullptr)
                        continue;
                    GLint loc = glGetAttribLocation(program, name);
                    if (loc < 0)
                    {
                        //hflog.warning("%s(): Program %i does not have attrib %s", __FUNCTION__, program_, name);
                        continue;
                    }
                    glVertexAttribPointer(loc, 4, GL_FLOAT, mesh.IsAttribNormalized(i), sizeof(SimpleGeometryMesh::Vertex), cast_to_pointer(mesh.GetVertexOffset(i)));
                    glEnableVertexAttribArray(loc);
                    count++;
                }
            }

            if (count == 0)
            {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
                glDeleteVertexArrays(1, &vao);
                vao = 0;
                return;
            }

            GLenum type = 0;
            switch (sizeof(SimpleGeometryMesh::Index))
            {
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
            for (int i = 0, count = (int)mesh.GetSurfaces().size(); i < count; i++)
            {
                auto meshSurface = mesh.GetSurface(i);
                surfaces[i].mode = meshSurface.type;
                surfaces[i].count = meshSurface.count;
                surfaces[i].offset = cast_to_pointer(meshSurface.first * mesh.GetIndexSize());

                surfaces[i].type = type;
            }

            //glBindBuffer(GL_ARRAY_BUFFER, 0);
            //glVertexArrayElementBuffer(vao, eabo_);
            glBindVertexArray(0);
        }
    }

    ~VertexArrayObject()
    {
        if (vao > 0)
        {
            glDeleteVertexArrays(1, &vao);
        }
    }

    void Draw()
    {
        if (vao == 0)
            return;

        glBindVertexArray(vao);
        for (auto &surface : surfaces)
        {
            glDrawElements(surface.mode, surface.count, surface.type, surface.offset);
        }
        glBindVertexArray(0);
    }

    inline void *cast_to_pointer(size_t i) const noexcept
    {
        return (void *)i;
    }
};

void SimpleGLES30Renderer::RenderMesh(SimpleGeometryMesh &mesh, const Matrix4f &modelViewMatrix)
{
    if (renderConfig.shaderProgram == nullptr)
        return;

    renderConfig.shaderProgram->Use();
    renderConfig.shaderProgram->ApplyUniform("ModelViewMatrix", (SimpleUniform)modelViewMatrix);
    renderConfig.shaderProgram->ApplyUniform("WorldMatrix", (SimpleUniform)modelViewMatrix);

    // create a vbo
    BufferObject abo_(GL_ARRAY_BUFFER, (GLsizei)mesh.GetVertexDataSize(), mesh.GetVertexData(), GL_STATIC_DRAW);
    BufferObject eabo_(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)mesh.GetIndexDataSize(), mesh.GetIndexData(), GL_STATIC_DRAW);

    VertexArrayObject vao(mesh, renderConfig.shaderProgram->GetProgram(), abo_.buffer, eabo_.buffer);
    vao.Draw();
}

void SimpleGLES30Renderer::RenderCubeImages()
{
    if (!areBuffersBuilt)
        BuildBuffers();

    GLsizei s = renderConfig.viewportRect.w;

    renderConfig.viewportRect.x = 0;
    renderConfig.viewportRect.y = s;
    renderConfig.defaultCubeFace = 1;
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderSingleImage();

    renderConfig.viewportRect.x = 2 * s;
    renderConfig.viewportRect.y = s;
    renderConfig.defaultCubeFace = 0;
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderSingleImage();

    renderConfig.viewportRect.x = s;
    renderConfig.viewportRect.y = 2 * s;
    renderConfig.defaultCubeFace = 2;
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderSingleImage();

    renderConfig.viewportRect.x = s;
    renderConfig.viewportRect.y = 0;
    renderConfig.defaultCubeFace = 3;
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderSingleImage();

    renderConfig.viewportRect.x = s;
    renderConfig.viewportRect.y = s;
    renderConfig.defaultCubeFace = 4;
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderSingleImage();

    renderConfig.viewportRect.x = 3 * s;
    renderConfig.viewportRect.y = s;
    renderConfig.defaultCubeFace = 5;
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderSingleImage();
}

void SimpleGLES30Renderer::RenderSingleImage()
{
    if (!areBuffersBuilt)
        BuildBuffers();

    if (!ApplyRenderConfig())
        return;

    if (renderConfig.isCubeMap)
    {
        projectionMatrix.LoadIdentity();
        projectionMatrix.Perspective(90.0f, 1.0f, renderConfig.znear, renderConfig.zfar);
        Vector4f cameraPosition(0, 0, 0, 1);
        cameraPosition = cameraMatrix * cameraPosition;
        if (renderConfig.defaultCubeFace >= 0 && renderConfig.defaultCubeFace < 6)
        {
            cameraMatrix.LoadIdentity();
            cameraMatrix.CubeMatrix(GL_TEXTURE_CUBE_MAP_POSITIVE_X + renderConfig.defaultCubeFace);
            cameraMatrix.Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z);
            Render(*program, renderConfig.useMaterials, renderConfig.useMaps, renderConfig.useZOnly, projectionMatrix, cameraMatrix);
        }
        else
        {
            for (int i = 0; i < 6; i++)
            {
                cameraMatrix.LoadIdentity();
                cameraMatrix.Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z);
                cameraMatrix.CubeMatrix(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
                Render(*program, renderConfig.useMaterials, renderConfig.useMaps, renderConfig.useZOnly, projectionMatrix, cameraMatrix);
            }
        }
    }
    else
    {
        Render(*program, renderConfig.useMaterials, renderConfig.useMaps, renderConfig.useZOnly, projectionMatrix, cameraMatrix);
    }

    glUseProgram(0);

    if (renderConfig.renderSkyBox)
    {
        glEnable(GL_DEPTH_TEST);
        RenderSkyBox();
        glDisable(GL_DEPTH_TEST);
    }

    RestoreGLState();
}

void SimpleGLES30Renderer::ApplyGlobalSettingsToCurrentProgram()
{
    if (locs.enviroCubeMap >= 0)
        glUniform1i(locs.enviroCubeMap, ssg.environment.enviroColorMapUnit);
    if (locs.enviroCubeMapAmount >= 0)
        glUniform1f(locs.enviroCubeMapAmount, 0.0);
    if (locs.pbskyCubeMap >= 0)
        glUniform1i(locs.pbskyCubeMap, ssg.environment.pbskyColorMapUnit);
    if (locs.sunDirTo >= 0)
        glUniform3fv(locs.sunDirTo, 1, ssg.environment.curSunDirTo.const_ptr());
    if (locs.sunColor >= 0)
        glUniform3fv(locs.sunColor, 1, ssg.environment.sunColor.const_ptr());
    if (locs.sunSize >= 0)
        glUniform1f(locs.sunSize, ssg.environment.sunSize);
    if (locs.sunShadowMap >= 0)
        glUniform1i(locs.sunShadowMap, ssg.environment.sunDepthMapUnit);
    if (locs.sunColorMap >= 0)
        glUniform1i(locs.sunColorMap, ssg.environment.sunColorMapUnit);
}

void SimpleGLES30Renderer::ApplySpheresToCurrentProgram()
{
    std::vector<float> spherePositions;
    std::vector<float> sphereKe;
    int numSpheres = 0;
    for (auto sphIt = ssg.spheres.begin(); sphIt != ssg.spheres.end(); sphIt++)
    {
        if (spherePositions.size() > 8)
            break;
        Vector4f pos(0, 0, 0, 1);
        Vector4f radius(1, 0, 0, 1);
        pos = sphIt->second.transform * pos;
        radius = sphIt->second.transform * radius;
        radius = radius - pos;
        float length = radius.length();
        pos.w = length;
        SimpleMaterial *mtl = ssg.materials.SetLibraryMaterial(sphIt->second.mtllibName, sphIt->second.mtlName);
        // only push spheres that are emissive...
        if (mtl)
        {
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
    if (numSpheres > 0)
    {
        if (locs.sphere_array >= 0)
            glUniform4fv(locs.sphere_array, numSpheres, &spherePositions[0]);
        if (locs.sphere_Ke >= 0)
            glUniform4fv(locs.sphere_Ke, numSpheres, &sphereKe[0]);
    }
    if (locs.sphere_count >= 0)
        glUniform1i(locs.sphere_count, numSpheres);
}

void SimpleGLES30Renderer::Render(SimpleProgram &program_, bool useMaterials, bool useMaps, bool useZOnly, Matrix4f &projectionMatrix_, Matrix4f &cameraMatrix_)
{
    Matrix4f inverseCameraMatrix = cameraMatrix_.AsInverse();
    Vector4f cameraPosition(0, 0, 0, 1);
    cameraPosition = inverseCameraMatrix * cameraPosition;

    SimpleUniform InverseCameraMatrix = cameraMatrix_.AsInverse();
    SimpleUniform CameraMatrix = cameraMatrix_;
    SimpleUniform ProjectionMatrix = projectionMatrix_;
    SimpleUniform CameraPosition = cameraPosition;

    program_.ApplyUniform("ProjectionMatrix", ProjectionMatrix);
    program_.ApplyUniform("CameraMatrix", CameraMatrix);
    program_.ApplyUniform("InverseCameraMatrix", (SimpleUniform)cameraMatrix_.AsInverse());
    program_.ApplyUniform("CameraPosition", CameraPosition);

    // apply each material separately (use the idea that material state changes are worse than geometry ones
    for (auto libIt = ssg.materials.begin(); libIt != ssg.materials.end(); libIt++)
    {
        SimpleMaterialLibrary &mtllib = libIt->second;
        std::string mtllibName = mtllib.name;
        GLuint mtllibId = ssg.materials.GetLibraryId(mtllib.name);
        ssg.materials.SetLibrary(mtllib.name);

        // loop through each material
        for (auto mtlIt = mtllib.mtls.begin(); mtlIt != mtllib.mtls.end(); mtlIt++)
        {
            GLuint mtlId = mtlIt->first;
            std::string mtlName = ssg.materials.GetMaterialName(mtlId);
            SimpleMaterial &mtl = mtlIt->second;
            ssg.materials.SetMaterial(mtlName);

            if (useMaterials)
                ApplyMaterialToCurrentProgram(mtl, useMaps);

            // loop through each geometry object
            for (auto geoIt = ssg.geometry.begin(); geoIt != ssg.geometry.end(); geoIt++)
            {
                SimpleGeometryGroup &geo = geoIt->second;
                GLuint objectId = geo.objectId;
                GLuint groupId = 0;
                renderer.ApplyIdToMtlNames(mtlName, mtlIt->first);

                // Apply object specific uniforms like transformation matrices
                SimpleUniform ModelViewMatrix;
                SimpleUniform SunShadowBiasMatrix;
                SimpleUniform SunShadowProjectionMatrix;
                SimpleUniform SunShadowViewMatrix;
                SimpleUniform SunShadowInverseViewMatrix;
                Matrix4f totalTransform = geo.transform * geo.addlTransform;
                ModelViewMatrix = totalTransform;

                SunShadowBiasMatrix = ssg.environment.sunShadowBiasMatrix;
                SunShadowProjectionMatrix = ssg.environment.sunShadowProjectionMatrix;
                SunShadowViewMatrix = ssg.environment.sunShadowViewMatrix;
                SunShadowInverseViewMatrix = ssg.environment.sunShadowInverseViewMatrix;

                program_.ApplyUniform("ModelViewMatrix", ModelViewMatrix);
                program_.ApplyUniform("SunShadowBiasMatrix", SunShadowBiasMatrix);
                program_.ApplyUniform("SunShadowProjectionMatrix", SunShadowProjectionMatrix);
                program_.ApplyUniform("SunShadowViewMatrix", SunShadowViewMatrix);
                program_.ApplyUniform("SunShadowInverseViewMatrix", SunShadowInverseViewMatrix);

                // Now iterate through each object and render it with this material
                if (objectId && groupId && mtllibId && mtlId)
                    renderer.RenderIf(objectId, groupId, mtllibId, mtlId, useZOnly);
                else
                    renderer.RenderIf(geo.objectName, "", geo.mtllibName, mtlName, useZOnly);
            }

            DisableCurrentTextures();
        }
    }
}

void SimpleGLES30Renderer::ApplyMaterialToCurrentProgram(SimpleMaterial &mtl, bool useMaps)
{
    GLuint unit = 1;

    if (useMaps)
    {
        if (!mtl.map_Ka.empty())
            currentTextures["map_Ka"] = ssg.materials.GetTextureMap(mtl.map_Ka);
        if (!mtl.map_Kd.empty())
            currentTextures["map_Kd"] = ssg.materials.GetTextureMap(mtl.map_Kd);
        if (!mtl.map_Ks.empty())
            currentTextures["map_Ks"] = ssg.materials.GetTextureMap(mtl.map_Ks);
        if (!mtl.map_Ke.empty())
            currentTextures["map_Ke"] = ssg.materials.GetTextureMap(mtl.map_Ke);
        if (!mtl.map_Ns.empty())
            currentTextures["map_Ns"] = ssg.materials.GetTextureMap(mtl.map_Ns);
        if (!mtl.map_Ns.empty())
            currentTextures["map_Ni"] = ssg.materials.GetTextureMap(mtl.map_Ni);
        if (!mtl.map_Tf.empty())
            currentTextures["map_Tf"] = ssg.materials.GetTextureMap(mtl.map_Tf);
        if (!mtl.map_Tr.empty())
            currentTextures["map_Tr"] = ssg.materials.GetTextureMap(mtl.map_Tr);
        if (!mtl.map_bump.empty())
            currentTextures["map_bump"] = ssg.materials.GetTextureMap(mtl.map_bump);
        if (!mtl.map_normal.empty())
            currentTextures["map_normal"] = ssg.materials.GetTextureMap(mtl.map_normal);
        if (!mtl.PBmap.empty())
            currentTextures["PBmap"] = ssg.materials.GetTextureMap(mtl.PBmap);

        for (auto tmapIt = currentTextures.begin(); tmapIt != currentTextures.end(); tmapIt++)
        {
            SimpleMap *pMap = tmapIt->second;
            if (pMap)
            {
                if (pMap->unitId <= 0)
                    pMap->unitId = GetTexUnit();
                pMap->samplerId = pMap->textureObject.samplerObject.GetId();
                pMap->textureId = pMap->textureObject.GetTextureId();
                pMap->textureObject.Bind(pMap->unitId, false);
                glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);

                if (pMap->samplerId == 0)
                {
                    glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_WRAP_R, GL_REPEAT);
                    glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                }

                pMap->map_loc = -1;
                pMap->map_mix_loc = -1;
                if (tmapIt->first == "map_Ka")
                {
                    pMap->map_loc = locs.map_Ka;
                    pMap->map_mix_loc = locs.map_Ka_mix;
                }
                else if (tmapIt->first == "map_Kd")
                {
                    pMap->map_loc = locs.map_Kd;
                    pMap->map_mix_loc = locs.map_Kd_mix;
                }
                else if (tmapIt->first == "map_Ks")
                {
                    pMap->map_loc = locs.map_Ks;
                    pMap->map_mix_loc = locs.map_Ks_mix;
                }
                else if (tmapIt->first == "map_Ke")
                {
                    pMap->map_loc = locs.map_Ke;
                    pMap->map_mix_loc = locs.map_Ke_mix;
                }
                else if (tmapIt->first == "map_Ni")
                {
                    pMap->map_loc = locs.map_Ni;
                    pMap->map_mix_loc = locs.map_Ni_mix;
                }
                else if (tmapIt->first == "map_Ns")
                {
                    pMap->map_loc = locs.map_Ns;
                    pMap->map_mix_loc = locs.map_Ns_mix;
                }
                else if (tmapIt->first == "map_Tf")
                {
                    pMap->map_loc = locs.map_Tf;
                    pMap->map_mix_loc = locs.map_Tf_mix;
                }
                else if (tmapIt->first == "map_Tr")
                {
                    pMap->map_loc = locs.map_Tr;
                    pMap->map_mix_loc = locs.map_Tr_mix;
                }
                else if (tmapIt->first == "map_bump")
                {
                    pMap->map_loc = locs.map_bump;
                    pMap->map_mix_loc = locs.map_bump_mix;
                }
                else if (tmapIt->first == "map_normal")
                {
                    pMap->map_loc = locs.map_normal;
                    pMap->map_mix_loc = locs.map_normal_mix;
                }
                else if (tmapIt->first == "PBmap")
                {
                    pMap->map_loc = locs.PBmap;
                    pMap->map_mix_loc = locs.PBmap_mix;
                }

                if (pMap->unitId < 0 || pMap->unitId >= g_MaxCombinedTextureUnits)
                {
                    hflog.error("%s(): pMap->unitId (%d) is out of range.", __FUNCTION__, pMap->unitId);
                }
                else
                {
                    if (pMap->map_loc >= 0)
                        glUniform1i(pMap->map_loc, pMap->unitId);
                    if (pMap->map_mix_loc >= 0)
                        glUniform1f(pMap->map_mix_loc, 1.0f);
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
    for (auto uniform : locs.newLocationList)
    {
        // uniform.first = name
        // uniform.second = details
    }
}

void SimpleGLES30Renderer::DisableCurrentTextures()
{
    // Turn off textures and reset program_ unit bindings to 0
    for (auto tmapIt = currentTextures.begin(); tmapIt != currentTextures.end(); tmapIt++)
    {
        SimpleMap *pMap = tmapIt->second;
        if (pMap)
        {
            glutBindTexture(pMap->unitId, pMap->textureObject.GetTarget(), 0);

            glUniform1i(pMap->map_loc, 0);
            glUniform1f(pMap->map_mix_loc, 0.0f);

            if (pMap->unitId != 0)
            {
                FreeTexUnit(pMap->unitId);
                pMap->unitId = 0;
            }
        }
    }
    glutSetActiveTexture(GL_TEXTURE0);

    currentTextures.clear();
}

void SimpleGLES30Renderer::InitSkyBox()
{
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
        size, -size, size, 1.0f, -1.0f, 1.0f};

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
        0, 3, 2};

    if (abo == 0)
    {
        glGenBuffers(1, &abo);
        glGenBuffers(1, &eabo);
        glBindBuffer(GL_ARRAY_BUFFER, abo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }
}

void SimpleGLES30Renderer::RenderSkyBox()
{
    // GLuint vbo = 0;
    GLint vloc = -1;
    GLint tloc = -1;
    GLuint programId = 0;
    GLint uCubeTexture = -1;
    GLint uWorldMatrix = -1;
    GLint uCameraMatrix = -1;
    GLint uProjectionMatrix = -1;
    GLint uSunE0 = -1;
    GLint uSunDirTo = -1;
    GLint uGroundE0 = -1;

    auto it = renderer.programs.find("skybox");
    if (it != renderer.programs.end())
    {
        programId = it->second->GetProgram();
        uCubeTexture = it->second->GetUniformLocation("uCubeTexture");
        uWorldMatrix = it->second->GetUniformLocation("WorldMatrix");
        uCameraMatrix = it->second->GetUniformLocation("CameraMatrix");
        uProjectionMatrix = it->second->GetUniformLocation("ProjectionMatrix");
        uSunE0 = it->second->GetUniformLocation("SunE0");
        uGroundE0 = it->second->GetUniformLocation("GroundE0");
        uSunDirTo = it->second->GetUniformLocation("SunDirTo");
    }

    if (programId == 0)
        return;

    vloc = glGetAttribLocation(programId, "aPosition");
    tloc = glGetAttribLocation(programId, "aTexCoord");

    glUseProgram(programId);
    if (uCubeTexture >= 0)
    {
        glutBindTextureAndSampler(ssg.environment.pbskyColorMapUnit, GL_TEXTURE_CUBE_MAP, ssg.environment.pbskyColorMapId, ssg.environment.pbskyColorMapSamplerId);
        glUniform1i(uCubeTexture, ssg.environment.pbskyColorMapUnit);
    }
    if (uProjectionMatrix >= 0)
    {
        Matrix4f projectionMatrix_ = ssg.camera.projectionMatrix;
        //projectionMatrix_.MakePerspective(ssg.camera.fov, aspectRatio, ssg.camera.imageNearZ, ssg.camera.imageFarZ);
        glUniformMatrix4fv(uProjectionMatrix, 1, GL_FALSE, projectionMatrix_.ptr());
    }
    if (uCameraMatrix >= 0)
    {
        Matrix4f viewMatrix = renderConfig.preCameraMatrix * ssg.camera.viewMatrix;
        viewMatrix.m14 = viewMatrix.m24 = viewMatrix.m34 = viewMatrix.m41 = viewMatrix.m42 = viewMatrix.m43 = 0.0f;
        glUniformMatrix4fv(uCameraMatrix, 1, GL_FALSE, viewMatrix.const_ptr());
    }
    if (uWorldMatrix >= 0)
    {
        Matrix4f worldMatrix;
        glUniformMatrix4fv(uWorldMatrix, 1, GL_FALSE, worldMatrix.const_ptr());
    }
    if (uSunDirTo >= 0 && uSunE0 >= 0 && uGroundE0 >= 0)
    {
        glUniform3fv(uSunDirTo, 1, ssg.environment.curSunDirTo.const_ptr());
        glUniform4fv(uSunE0, 1, ssg.environment.curSunDiskRadiance.const_ptr());
        glUniform4fv(uGroundE0, 1, ssg.environment.curGroundRadiance.const_ptr());
    }

    glBindBuffer(GL_ARRAY_BUFFER, abo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
    glVertexAttribPointer(vloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void *)0);
    glVertexAttribPointer(tloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void *)12);
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

void SimpleGLES30Renderer::BuildBuffers()
{
    renderer.Reset();
    for (auto it = ssg.geometry.begin(); it != ssg.geometry.end(); it++)
    {
        SimpleGeometryGroup &geo = it->second;
        renderer.SetCurrentObjectId(geo.objectId);
        renderer.SetCurrentMtlLibId(geo.mtllibId);
        renderer.SetCurrentObjectName(geo.objectName);
        renderer.SetCurrentMtlLibName(geo.mtllibName);
        renderer.NewObject();
        ssg.geometryObjects[geo.objectId].Render(renderer);
    }
    renderer.AssignMaterialIds(ssg.materials);
    renderer.SetCurrentMtlLibName("");
    renderer.SetCurrentMtlLibId(0);
    InitSkyBox();
    areBuffersBuilt = true;
}
} // namespace Fluxions