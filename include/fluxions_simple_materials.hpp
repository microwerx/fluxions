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

#include <fluxions_stdcxx.hpp>
#include <fluxions_fileio.hpp>
#include <fluxions_simple_texture.hpp>
#include <fluxions_resource_manager.hpp>
#include <fluxions_simple_property.hpp>

namespace Fluxions
{
using namespace std;


struct SimpleMaterial
{
Vector3f Kd;
Vector3f Ks;
Vector3f Ke;
Vector3f Ka;
Vector3f Tf; // Translucency;	// Tf
float Tr = 0.0f; // TranslucencyLevel -- same as (1 - d)
Vector3f Refract;
Vector3f Opacity;
float Ns = 0.0f; // Specular exponent [0 <= Ns <= 1000]
float Ni = 0.0f;
float d = 1.0f; // dissolved -- same as (1 - Tr)

float PBm = 0.0f; // for roughness
float PBk = 0.0f; // for metals
float PBKdm = 0.0f; // for diffuse roughness (Oren-Nayer)
float PBKsm = 0.0f; // for specular roughness (Cook-Torrance)
float PBior = 0.0f; // for Fresnel reflection (Cook-Torrance)
float PBGGXgamma = 0.11f; // for GGX BRDF (0.0 - 1.0 maps to 1.0 to 10.0 in the shader)
string PBmap;

float EmissionGlossiness;
float ReflectFresnel;
float RefractGlossiness;
float ReflectGlossiness;
float AnisotropyAmount;
float AnisotropyRotation;
Vector3f AttenuationColor;
float AttenuationDistance;
Vector3f ScatteringAlbedo;
float MeanCosine;
vector<string> RefractMode;
bool HasRoundedCorners = false;
float RoundedCorners;
bool IsPortal = false;
bool HasIesProfile = false;
string IesProfileFilename;
Matrix4f IesProfileMatrix;
bool IesProfileKeepSharp;
vector<string> Invisible;
string map_Ka;
string map_Kd;
string map_Ks;
string map_Ke;
string map_d;
string map_opacity;
string map_Ns;
string map_Ni;
string map_Tr;// translucency;
string map_Tf;//Tf translucencyLevel;
string map_refract;
string map_anisotropy;
string map_anisorotation;
string map_scatteringAlbedo;
string map_attenuation;
string map_normal;
string map_bump;

SimpleAssociativePropertyList Properties;
};


struct SimpleMap
{
string mapName;
GLuint mapId;
string pathname;
string shader;
GLuint textureId;
GLuint samplerId;
GLint unitId; // we're not using GLenum here because unitId can be < 0 and no overflow is expected.
GLint map_loc;
GLint map_mix_loc;
SimpleTexture textureObject;
};


ostream & WriteMaterial(ostream &ostr, const SimpleMaterial &mtl);
istream & ReadMaterial(istream &istr, SimpleMaterial &mtl);
void SetMaterialDefaults(SimpleMaterial &mtl);


struct SimpleMaterialLibrary
{
TResourceManager<SimpleMap> maps;
TResourceManager<SimpleMaterial> mtls;
FilePathInfo fpi;
string name;

SimpleMaterialLibrary() { }
~SimpleMaterialLibrary() { }
};


class SimpleMaterialSystem
{
private:
string defaultName = "<INVALIDMATERIAL>";
string currentMtlLibName;
GLuint currentMtlLibId = 0;
string currentMtlName;
GLuint currentMtlId = 0;

SimpleMaterialLibrary *currentMtlLibPtr = nullptr;
SimpleMaterial *currentMtlPtr = nullptr;

TResourceManager<SimpleMaterialLibrary> mtllibs;
TResourceManager<string> shaderMaps;

map<string, string> maps_paths;
string defaultMapPath = "<UNKNOWN>";

void SynchronizeIds();
public:
SimpleMaterialSystem();
~SimpleMaterialSystem();

void Clear();

auto begin() -> decltype(mtllibs.begin()) { return mtllibs.begin(); }
auto end() -> decltype(mtllibs.end()) { return mtllibs.end(); }
auto size() -> decltype(mtllibs.size()) { return mtllibs.size(); }

bool Save(const string &path);
bool Load(const string &mtllibName, const string &filename);
bool Save(const string &mtllibName, const string &filename);

SimpleMaterialLibrary *CreateLibrary(const string &name);
SimpleMaterial *CreateMaterial(const string &name);

void DeleteLibrary(const string &name);
void DeleteMaterial(const string &name);
void DeleteLibraryMaterial(const string &mtllibName, const string &mtlName);

SimpleMaterialLibrary *SetLibrary(const string &name);
SimpleMaterial *SetMaterial(const string &name);
SimpleMaterial *SetLibraryMaterial(const string &mtllibName, const string &mtlName);
const SimpleMaterial *GetLibraryMaterial(const string &mtllibName, const string &mtlName) const;
SimpleMaterial *GetCurrentMaterial() { return currentMtlPtr; }

const string & GetLibraryName();
const string & GetMaterialName();
const string & GetMaterialName(GLuint id);

GLuint GetLibraryId();
GLuint GetMaterialId();
GLuint GetLibraryId(const string &name);
GLuint GetMaterialId(const string &name);
GLuint GetLibraryMaterialId(const string &mtllibName, const string &mtlName);

bool AddMap(const string &path, const string &filename);
bool AddMapShader(const string &name, const string &shader);
void LoadMaps();
SimpleMap *GetTextureMap(const string &name);
const SimpleMap *GetTextureMap(const string &name) const;

const string & GetMapPath(const string & mapName) const;
};
}

#endif
