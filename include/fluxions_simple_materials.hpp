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
#ifndef FLUXIONS_SIMPLE_MATERIALS_HPP
#define FLUXIONS_SIMPLE_MATERIALS_HPP

#include <fluxions_fileio.hpp>
#include <fluxions_resource_manager.hpp>
#include <fluxions_simple_property.hpp>
#include <fluxions_simple_texture.hpp>
#include <fluxions_stdcxx.hpp>

namespace Fluxions
{

struct SimpleMaterial
{
    Color3f Kd;
    Color3f Ks;
    Color3f Ke;
    Color3f Ka;
    Color3f Tf;      // Translucency;	// Tf
    float Tr = 0.0f; // TranslucencyLevel -- same as (1 - d)
    Color3f Refract;
    Color3f Opacity;
    float Ns = 0.0f; // Specular exponent [0 <= Ns <= 1000]
    float Ni = 0.0f;
    float d = 1.0f; // dissolved -- same as (1 - Tr)

    float PBm = 0.0f;         // for roughness
    float PBk = 0.0f;         // for metals
    float PBKdm = 0.0f;       // for diffuse roughness (Oren-Nayer)
    float PBKsm = 0.0f;       // for specular roughness (Cook-Torrance)
    float PBior = 0.0f;       // for Fresnel reflection (Cook-Torrance)
    float PBGGXgamma = 0.11f; // for GGX BRDF (0.0 - 1.0 maps to 1.0 to 10.0 in the shader)
    std::string PBmap;

    float EmissionGlossiness = 0;
    float ReflectFresnel = 0;
    float RefractGlossiness = 0;
    float ReflectGlossiness = 0;
    float AnisotropyAmount = 0;
    float AnisotropyRotation = 0;
    Color3f AttenuationColor;
    float AttenuationDistance = 0;
    Color3f ScatteringAlbedo;
    float MeanCosine = 0;
    std::vector<std::string> RefractMode;
    bool HasRoundedCorners = false;
    float RoundedCorners = 0;
    bool IsPortal = false;
    bool HasIesProfile = false;
    std::string IesProfileFilename;
    Matrix4f IesProfileMatrix;
    bool IesProfileKeepSharp = false;
    std::vector<std::string> Invisible;
    std::string map_Ka;
    std::string map_Kd;
    std::string map_Ks;
    std::string map_Ke;
    std::string map_d;
    std::string map_opacity;
    std::string map_Ns;
    std::string map_Ni;
    std::string map_Tr; // translucency;
    std::string map_Tf; //Tf translucencyLevel;
    std::string map_refract;
    std::string map_anisotropy;
    std::string map_anisorotation;
    std::string map_scatteringAlbedo;
    std::string map_attenuation;
    std::string map_normal;
    std::string map_bump;

    SimpleAssociativePropertyList Properties;
};

struct SimpleMap
{
    std::string mapName;
    GLuint mapId = 0;
    std::string pathname;
    std::string shader;
    GLuint textureId = 0;
    GLuint samplerId = 0;
    GLint unitId = 0; // we're not using GLenum here because unitId can be < 0 and no overflow is expected.
    GLint map_loc = 0;
    GLint map_mix_loc = 0;
    SimpleTexture textureObject;
};

std::ostream &WriteMaterial(std::ostream &ostr, const SimpleMaterial &mtl);
std::istream &ReadMaterial(std::istream &istr, SimpleMaterial &mtl);
void SetMaterialDefaults(SimpleMaterial &mtl);

struct SimpleMaterialLibrary
{
    TResourceManager<SimpleMap> maps;
    TResourceManager<SimpleMaterial> mtls;
    FilePathInfo fpi;
    std::string name;

    SimpleMaterialLibrary() {}
    ~SimpleMaterialLibrary() {}
};

class SimpleMaterialSystem
{
  private:
    std::string defaultName = "<INVALIDMATERIAL>";
    std::string currentMtlLibName;
    GLuint currentMtlLibId = 0;
    std::string currentMtlName;
    GLuint currentMtlId = 0;

    SimpleMaterialLibrary *currentMtlLibPtr = nullptr;
    SimpleMaterial *currentMtlPtr = nullptr;

    TResourceManager<SimpleMaterialLibrary> mtllibs;
    TResourceManager<std::string> shaderMaps;

    std::map<std::string, std::string> maps_paths;
    std::string defaultMapPath = "<UNKNOWN>";

    void SynchronizeIds();

  public:
    SimpleMaterialSystem();
    ~SimpleMaterialSystem();

    void Clear();

    auto begin() -> decltype(mtllibs.begin()) { return mtllibs.begin(); }
    auto end() -> decltype(mtllibs.end()) { return mtllibs.end(); }
    auto size() -> decltype(mtllibs.size()) { return mtllibs.size(); }

    bool Save(const std::string &path);
    bool Load(const std::string &mtllibName, const std::string &filename);
    bool Save(const std::string &mtllibName, const std::string &filename);

    SimpleMaterialLibrary *CreateLibrary(const std::string &name);
    SimpleMaterial *CreateMaterial(const std::string &name);

    void DeleteLibrary(const std::string &name);
    void DeleteMaterial(const std::string &name);
    void DeleteLibraryMaterial(const std::string &mtllibName, const std::string &mtlName);

    SimpleMaterialLibrary *SetLibrary(const std::string &name);
    SimpleMaterial *SetMaterial(const std::string &name);
    SimpleMaterial *SetLibraryMaterial(const std::string &mtllibName, const std::string &mtlName);
    const SimpleMaterial *GetLibraryMaterial(const std::string &mtllibName, const std::string &mtlName) const;
    SimpleMaterial *GetCurrentMaterial() { return currentMtlPtr; }

    const std::string &GetLibraryName();
    const std::string &GetMaterialName();
    const std::string &GetMaterialName(GLuint id);

    GLuint GetLibraryId();
    GLuint GetMaterialId();
    GLuint GetLibraryId(const std::string &name);
    GLuint GetMaterialId(const std::string &name);
    GLuint GetLibraryMaterialId(const std::string &mtllibName, const std::string &mtlName);

    bool AddMap(const std::string &path, const std::string &filename);
    bool AddMapShader(const std::string &name, const std::string &shader);
    void LoadMaps();
    SimpleMap *GetTextureMap(const std::string &name);
    const SimpleMap *GetTextureMap(const std::string &name) const;

    const std::string &GetMapPath(const std::string &mapName) const;
};
} // namespace Fluxions

#endif
