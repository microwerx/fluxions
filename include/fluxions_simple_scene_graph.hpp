// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
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
#ifndef FLUXIONS_SIMPLE_SCENE_GRAPH_HPP
#define FLUXIONS_SIMPLE_SCENE_GRAPH_HPP

#include <fluxions_corona.hpp>
#include <fluxions_fileio.hpp>
#include <fluxions_gte.hpp>
#include <fluxions_multiple_render_target.hpp>
#include <fluxions_opengl.hpp>
#include <fluxions_pbsky.hpp>
#include <fluxions_resource_manager.hpp>
#include <fluxions_simple_materials.hpp>
#include <fluxions_stdcxx.hpp>

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

	struct SimpleGeometryGroup
	{
		Matrix4f transform;
		Matrix4f addlTransform;
		BoundingBoxf bbox;

		FilePathInfo fpi;

		std::string objectName;
		std::string mtllibName;

		GLuint objectId = 0;
		GLuint mtllibId = 0;
	};

	struct SimpleCamera
	{
		bool isOrtho;
		bool isPerspective;
		Matrix4f projectionMatrix;
		Matrix4f viewMatrix;
		Matrix4f actualViewMatrix;
		float fov = 45.0;
		float width = 2.0;
		float fstop = 16.0;
		float filmWidth = 35.0;
		float focalDist = 100.0;
		float bokehPolygonalBlades = 3.0;
		float bokehPolygonalRotation = 0.0;
		float regionStartX = 0.0;
		float regionStartY = 0.0;
		float regionEndX = 1.0;
		float regionEndY = 1.0;
		std::string bokehImg;
		float imageAspect = 1.0;
		float imageWidth = 1024.0;
		float imageHeight = 1024.0;
		float imageX = 0.0;
		float imageY = 0.0;
		float imageNearZ = 0.1f;
		float imageFarZ = 1000.0f;

		void SetImageParameters(float screenWidth, float screenHeight, float znear, float zfar)
		{
			imageWidth = screenWidth;
			imageHeight = screenHeight;
			imageAspect = screenWidth / screenHeight;
			imageNearZ = znear;
			imageFarZ = zfar;
		}
	};

	struct SimpleEnvironment
	{
		bool hasColor = false;
		bool hasTexmap = false;
		bool hasSun = false;
		Color3f color;
		std::string texmap;
		Vector3f sunDirTo;
		Vector3f curSunDirTo;
		Color4f curSunDiskRadiance;
		Color4f curGroundRadiance;
		float toneMapScale = -6.0f;
		float toneMapExposure = -6.0f;
		float toneMapGamma = 1.0f;
		// GLuint curSkyTextureId = 0;
		// GLuint curSkySamplerId = 0;

		Color3f sunColor;
		float sunSize = 1.0;
		GLint isHemisphere = 0;
		float sunShadowMapDistance = 20.0f;
		float sunShadowMapZoom = 1.0f;
		Vector2f sunShadowMapOffset = Vector2f(0.0f, 0.0f);
		Vector3f sunShadowMapTarget;
		Vector3f sunShadowMapOrigin;
		Vector3f sunShadowMapUp;
		float sunShadowMapNearZ = 1.0f;
		float sunShadowMapFarZ = 100.0f;
		float sunShadowMapTime = 0.0;

		PhysicallyBasedSky pbsky;
		Astronomy::PA::CivilDateTime pbsky_dtg;
		bool pbskyGenCubeMap = true;
		bool pbskyGenCylMap = false;

		Matrix4f sunShadowBiasMatrix;
		Matrix4f sunShadowProjectionMatrix;
		Matrix4f sunShadowViewMatrix;
		Matrix4f sunShadowInverseViewMatrix;

		GLuint sunColorMapUnit = 0;
		GLuint sunColorMapId = 0;
		GLuint sunColorMapSamplerId = 0;
		GLuint sunDepthMapUnit = 0;
		GLuint sunDepthMapId = 0;
		GLuint sunDepthMapSamplerId = 0;
		GLuint enviroColorMapUnit = 0;
		GLuint enviroColorMapId = 0;
		GLuint enviroColorMapSamplerId = 0;
		GLuint enviroDepthMapUnit = 0;
		GLuint enviroDepthMapId = 0;
		GLuint enviroDepthMapSamplerId = 0;
		GLuint pbskyColorMapUnit = 0;
		GLuint pbskyColorMapId = 0;
		GLuint pbskyColorMapSamplerId = 0;

		void Update(const BoundingBoxf &bbox);
		void ComputePBSky();
		bool IsSkyComputed() const { return isSkyComputed; }
		double LastSkyGenTime() const { return lastSkyGenTime; }

	private:
		bool isSkyComputing = false;
		bool isSkyComputed = false;
		double lastSkyGenTime = 0.0;
	};

	struct SimpleSphere
	{
		Matrix4f transform;
		std::string mtllibName;
		std::string mtlName;
		GLuint mtllibId = 0;
		GLuint mtlId = 0;
		GLuint objectId = 0;
	};

	struct SimpleGpuTexture
	{
		//SimpleGpuTexture(SimpleGpuTexture & copy)
		//{
		//	target_ = copy.target_;
		//	texture_ = copy.texture_;
		//	lastUnitBound_ = copy.lastUnitBound_;
		//	copy.texture_ = 0;
		//}
		//SimpleGpuTexture & operator = (SimpleGpuTexture &other)
		//{
		//	target_ = other.target_;
		//	texture_ = other.texture_;
		//	lastUnitBound_ = other.lastUnitBound_;
		//	other.texture_ = 0;
		//	return *this;
		//}
		//SimpleGpuTexture(SimpleGpuTexture &&) = default;
		//SimpleGpuTexture & operator = (SimpleGpuTexture &&) = default;

		SimpleGpuTexture(GLenum target)
		{
			if (target != GL_TEXTURE_CUBE_MAP && target != GL_TEXTURE_2D)
				throw "Unsupported Texture Target";

			target_ = target;
			try
			{
				texture_ = std::make_shared<GLuint>(0);
				GLuint texture;
				glGenTextures(1, &texture);
				*texture_ = texture;
				FxDebugBindTexture(target_, *texture_);
				FxDebugBindTexture(target_, 0);
				// Hf::Log.info("%s(): Created texture %d", __FUNCTION__, *texture_);
			}
			catch (...)
			{
				// Hf::Log.error("%s(): glGenTextures() failed", __FUNCTION__);
			}
		}

		~SimpleGpuTexture()
		{
			if (texture_.use_count() == 1)
			{
				GLuint texture = *texture_;
				glDeleteTextures(1, &texture);
				// Hf::Log.info("%s(): Deleted texture %d", __FUNCTION__, *texture_);
			}
		}

		void Bind(int unit)
		{
			if (!texture_)
				return;
			Unbind();
			lastUnitBound_ = unit;
			glActiveTexture(GL_TEXTURE0 + unit);
			FxDebugBindTexture(target_, *texture_);
		}

		void Unbind()
		{
			if (!texture_ || lastUnitBound_ < 0)
				return;

			glActiveTexture(GL_TEXTURE0 + lastUnitBound_);
			glBindTexture(target_, *texture_);
			glActiveTexture(GL_TEXTURE0);
			lastUnitBound_ = -1;
		}

		void CreateStorage(GLenum internalformat, GLint width, GLint height, GLenum format, GLenum type)
		{
			if (!texture_)
				return;
			if (target_ == GL_TEXTURE_2D)
			{
				created_ = true;
				Bind(0);
				glTexImage2D(target_, 0, internalformat, width, height, 0, format, type, nullptr);
				Unbind();
			}
			else if (target_ == GL_TEXTURE_CUBE_MAP)
			{
				created_ = true;
				Bind(0);
				for (int i = 0; i < 6; i++)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalformat, width, height, 0, format, type, nullptr);
				}
				Unbind();
			}
		}

		void CreateTexture2D(GLsizei width = 64, GLsizei height = 64)
		{
			CreateStorage(GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE);
			SetDefaultParameters(GL_LINEAR, GL_LINEAR, GL_REPEAT);
		}

		void CreateTextureShadow2D(GLsizei width = 64, GLsizei height = 64)
		{
			CreateStorage(GL_DEPTH_COMPONENT, width, height, GL_DEPTH_COMPONENT, GL_FLOAT);
			SetDefaultParameters(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
		}

		void CreateTextureCube(GLsizei size = 64)
		{
			CreateStorage(GL_RGB, size, size, GL_RGB, GL_UNSIGNED_BYTE);
			SetDefaultParameters(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
		}

		void CreateTextureShadowCube(GLsizei size = 64)
		{
			CreateStorage(GL_DEPTH_COMPONENT, size, size, GL_DEPTH_COMPONENT, GL_FLOAT);
			SetDefaultParameters(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
		}

		void SetDefaultParameters(GLenum minFilter = GL_NEAREST, GLenum magFilter = GL_NEAREST, GLenum wrapMode = GL_CLAMP_TO_EDGE)
		{
			if (!texture_)
				return;
			Bind(0);
			glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexParameteri(target_, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(target_, GL_TEXTURE_WRAP_T, wrapMode);
			glTexParameteri(target_, GL_TEXTURE_WRAP_R, wrapMode);
			Unbind();
		}

		void GenerateMipmap()
		{
			if (!texture_)
				return;
			Bind(0);
			glGenerateMipmap(target_);
			Unbind();
		}

		inline GLuint GetTexture() const
		{
			if (!texture_)
				return 0;
			return *texture_;
		}

	private:
		GLenum target_ = GL_TEXTURE_CUBE_MAP;
		int lastUnitBound_ = -1;
		//GLuint texture_ = 0;
		bool created_ = false;
		std::shared_ptr<GLuint> texture_;
	};

	static const int MaxSphlLights = 16;
	static const int MaxSphlDegree = 9;
	static const int DefaultSphlDegree = 2;
	static const int SphlSunIndex = MaxSphlLights;

	struct SphlImageTexture
	{
		Image4f lightProbe;
		SimpleGpuTexture texture = SimpleGpuTexture(GL_TEXTURE_CUBE_MAP);

		bool LoadLightProbe(const std::string &path);
		bool SphToLightProbe(const MultispectralSph4f &sph);
		bool UploadLightProbe();
	};

	//struct SSPHHLightHierarchy
	//{
	//	int index = -1;
	//	float percentVisible = 0.0;
	//	SphericalHarmonicf sph;
	//	MultispectralSph4f msph;
	//	std::string description;

	//	SphlImageTexture debugLightProbe;
	//	SphlImageTexture debugSphLightProbe;

	//	// SSPHHLightHierarchy() { }

	//	inline void reset()
	//	{
	//		sph.reset();
	//		for (int k = 0; k < 4; k++)
	//			msph[k].reset();
	//	}

	//	inline void resize(int maxDegree)
	//	{
	//		sph.resize(maxDegree);
	//		for (int k = 0; k < 4; k++)
	//			msph[k].resize(maxDegree);
	//	}

	//	inline void resize(int maxDegree, float value)
	//	{
	//		sph.resize(maxDegree, value);
	//		for (int k = 0; k < 4; k++)
	//			msph[k].resize(maxDegree, value);
	//	}

	//	inline void Accumulate(const SSPHHLightHierarchy & hier)
	//	{
	//		sph.Accumulate(hier.sph, hier.percentVisible);
	//		for (int k = 0; k < 4; k++)
	//			msph[k].Accumulate(hier.msph[k], hier.percentVisible);
	//	}

	//	inline void CopySph(const MultispectralSph4f & _msph, float _percentVisible)
	//	{
	//		for (int k = 0; k < 3; k++)
	//		{
	//			msph[k] = _msph[k];
	//		}
	//		MakeStandardizedSph(sph, msph);
	//		percentVisible = _percentVisible;
	//	}
	//};

	struct SimpleCubeTexture
	{
		GLint unit = 0;
		GLint samplerId = 0;
		double buildTime = 0.0;
		float zfar = 0.0f;
		SimpleGpuTexture texture = SimpleGpuTexture(GL_TEXTURE_CUBE_MAP);
		bool dirty = true;
	};

	class SimpleSSPHHLight
	{
	public:
		bool enabled = true;
		float E0 = 1.0f;
		float falloffRadius = 100.0f;
		Vector4f position = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
		Quaternionf orientation;
		MultispectralSph4f msph;

		//vector<SSPHHLightHierarchy> hierarchies;

		std::vector<Image4f> vizgenLightProbes;
		Sph4f self;
		Sph4f neighbor;

		std::string hier_description;

		bool dirty = true;
		bool randomize = false;
		bool randomizePosition = false;
		std::string name;
		int index = -1;
		int maxDegree = DefaultSphlDegree;

		// This holds a cubemap representation of a loaded Corona 3D light probe
		Image4f lightProbe_corona;
		SimpleGpuTexture coronaLightProbeTexture = SimpleGpuTexture(GL_TEXTURE_CUBE_MAP);
		//GLuint coroneLightProbeTexture = 0;
		float coronaLightProbePercent = 0.0f;

		// This holds a cube map representation of the msph for this SPHL
		Image4f lightProbe_sph;
		SimpleGpuTexture sphLightProbeTexture = SimpleGpuTexture(GL_TEXTURE_CUBE_MAP);
		//GLuint sphLightProbeTexture = 0;

		// This holds the finalized SELF + NEIGHBOR light probe
		//SSPHHLightHierarchy hierSph;
		Image4f lightProbe_hier;
		SimpleGpuTexture hierLightProbeTexture = SimpleGpuTexture(GL_TEXTURE_CUBE_MAP);
		//GLuint hierLightProbeTexture = 0;

		SimpleCubeTexture depthSphlMap;
		SimpleCubeTexture colorSphlMap;
		SimpleCubeTexture environmentLightProbeMap;

		SimpleSSPHHLight();
		~SimpleSSPHHLight();

		// Reads from a Corona Light Probe (a cube map stored images from left to right in a single image).
		bool ReadCoronaLightProbe(const std::string &path);
		// Saves to a Corona Light Probe (a cube map with cube faces stored from left to right in a single image).
		bool SaveCoronaLightProbe(const std::string &path);

		// Saves a JSON form of the multispectral (RGBL) of this SPH. L represents a monochromatic version of the RGB components. { maxDegree: (1-10), coefs : [] }
		bool SaveJsonSph(const std::string &path);
		// Reads a JSON format of a multispectral (RGBL) of this SPH. L represents a monochromatic version of the RGB components. { maxDegree: (1-10), coefs : [] }
		bool ReadJsonSph(const std::string &path);

		// Writes name.obj and name.mtl to the path
		bool SaveOBJ(const std::string &path, const std::string &name);

		bool LightProbeToSph(const Image4f &lightProbe, MultispectralSph4f &sph);
		bool SphToLightProbe(const MultispectralSph4f &sph, Image4f &lightProbe);
		bool SphToLightProbe(const MultispectralSph4f &sph, Image4f &lightProbe, int maxDegree);
		bool UploadLightProbe(Image4f &lightProbe, SimpleGpuTexture &texture);
		bool UploadLightProbe(Image4f &lightProbe, GLuint &texture);

		Color3f GetCoefficientColor(int l, int m) const;
		float GetCoefficient(int j, int l, int m) const;
		float GetCoefficient(int j, int lm) const;
		void ZeroCoefficients();
		void IsolateCoefficient(int j, int l, int m);
		void SetCoefficient(int j, int l, int m, float value);
		void RandomizeCoefficient(int j, int l, int m, float minValue, float maxValue);
		inline void Standardize() { MakeIntensityChannel4f(msph); }
		void Randomize(float size = 1.0f);

		void ChangeDegrees(int degrees);
		inline void IncDegrees() { ChangeDegrees(maxDegree + 1); }
		inline void DecDegrees() { ChangeDegrees(maxDegree - 1); }

		void ResizeHierarchies(int size);

		void MakeDirty();

		inline bool IsDirty() { return dirty; }
		inline int GetMaxCoefficients() const { return maxDegree * (maxDegree + 1) + maxDegree + 1; }
		inline int GetMaxCoefficients(int degree) { return degree * (degree + 1) + degree + 1; }
		inline void MakeLuminanceChannel() { MakeLuminanceChannel4f(msph); }
		inline void MakeIntensityChannel() { MakeIntensityChannel4f(msph); }

		// Hierarchies
		void SortHierarchiesByPercent();
		void SortHierarchiesByIndex();
		void CopySphlToHierarchies();

		void SetHierarchyDescriptionToIndex();
		void SetHierarchyDescriptionToPercent();
		void SetHierarchyDescription();
	};

	class SimpleSceneGraph;
	class SimpleSSPHH
	{
	public:
		SimpleSSPHH();
		~SimpleSSPHH();

		void INIT(SimpleSceneGraph &ssg);
		void GEN();
		void VIZ();
		void HIER(bool includeSelf = true, bool includeNeighbor = true, int maxDegrees = -1);

		bool saveJSONs = false;
		bool savePPMs = false;
		bool convToPNG = false;

		float VIZmix = 1.0;

	private:
		/// A pair (int index, float p)
		struct Qpair
		{
			int index = -1;
			float p = 0.0f;
		};

		std::string sceneName;
		std::vector<SimpleSSPHHLight> *sphls_ = nullptr;
		size_t size_ = 0; // The number of SPHLs

		// GEN creates this light probe
		std::vector<Sph4f> S;
		// VIZ generates these visibility spherical harmonics. Index i is S
		std::vector<std::vector<Sph4f>> H;
		// VIZ generates visibility probability for each H.
		std::vector<std::vector<float>> P;
		// HIER creates Q from index and P;
		std::vector<Qpair> Q;
		// HIER sorts Q
		std::vector<Qpair> Qsorted;
		// HIER generates this finalized light probe
		std::vector<Sph4f> Sprime;

		std::vector<Sph4f> self;
		std::vector<Sph4f> neighbor;
	};

	struct SimplePointLight
	{
		std::string name;
		size_t index;
		float E0;
		float falloffRadius;
		Vector3f position;

		SimpleCubeTexture scs;
	};

	struct __ShaderProgramLocations
	{
		GLint Ka = -1;
		GLint Kd = -1;
		GLint Ks = -1;
		GLint Ke = -1;
		GLint Tr = -1;
		GLint Tf = -1;
		GLint Ns = -1;
		GLint Ni = -1;

		GLint PBKdm = -1;
		GLint PBKsm = -1;
		GLint PBior = -1;
		GLint PBGGXgamma = -1;
		GLint PBm = -1;
		GLint PBk = -1;
		GLint PBmap = -1;
		GLint PBmap_mix = -1;

		GLint map_Ka = -1;
		GLint map_Kd = -1;
		GLint map_Ks = -1;
		GLint map_Ke = -1;
		GLint map_bump = -1;
		GLint map_normal = -1;
		GLint map_Tr = -1;
		GLint map_Tf = -1;
		GLint map_Ns = -1;
		GLint map_Ni = -1;

		GLint map_Ka_mix = -1;
		GLint map_Kd_mix = -1;
		GLint map_Ks_mix = -1;
		GLint map_Ke_mix = -1;
		GLint map_bump_mix = -1;
		GLint map_normal_mix = -1;
		GLint map_Tr_mix = -1;
		GLint map_Tf_mix = -1;
		GLint map_Ns_mix = -1;
		GLint map_Ni_mix = -1;

		GLint sphere_array = -1;
		GLint sphere_count = -1;
		GLint sphere_Ke = -1;

		GLint sphl_light_count = -1;
		GLint sphl_lights_enabled[16] = { -1 };
		GLint sphl_lights_position[16] = { -1 };
		GLint sphl_lights_E0[16] = { -1 };
		GLint sphl_lights_lightProbeCubeMap[16] = { -1 };
		GLint sphl_lights_environmentCubeMap[16] = { -1 };
		GLint sphl_lights_depthShadowCubeMap[16] = { -1 };
		GLint sphl_lights_depthShadowZFar[16] = { -1 };
		GLint sphl_lights_sph[16][9] = { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1} };

		GLint toneMapScale = -1;
		GLint toneMapExposure = -1;
		GLint toneMapGamma = -1;
		GLint sunShadowMap = -1;
		GLint sunColorMap = -1;
		GLint sunDirTo = -1;
		GLint sunColor = -1;
		GLint sunSize = -1;
		GLint sunE0 = -1;
		GLint groundE0 = -1;
		GLint sunShadowMatrix = -1;
		GLint enviroCubeMapAmount = -1;
		GLint enviroCubeMap = -1;
		GLint pbskyCubeMap = -1;
		GLint shaderDebugChoice = -1;
		GLint shaderDebugLight = -1;
		GLint shaderDebugSphl = -1;

		std::map<std::string, GLint> locations;

		SimpleAssociativePropertyList newLocationList;

		void GetMaterialProgramLocations(SimpleProgram &program);
	};

	class SimpleSceneGraph
	{
	public:
		std::string name;
		std::vector<std::string> sceneFileLines;
		std::vector<std::string> pathsToTry;
		Matrix4f currentTransform;

		std::vector<std::string> confFiles;
		SimpleCamera camera;
		SimpleEnvironment environment;

		TResourceManager<std::string> shaderMaps;
		TResourceManager<SimpleSphere> spheres;
		TResourceManager<SimpleGeometryGroup> geometry;
		TResourceManager<OBJStaticModel> geometryObjects;
		std::vector<SimpleSSPHHLight> ssphhLights;
		std::vector<SimplePointLight> pointLights;

		SimpleMaterialSystem materials;
		mutable SimpleRenderer_GLuint renderer;
		__ShaderProgramLocations locs;
		std::map<std::string, SimpleMap *> currentTextures;
		SimpleSSPHH ssphh;

		bool ReadMtlLibFile(const std::string &filename);
		bool ReadConfFile(const std::string &filename);
		bool ReadObjFile(const std::string &filename, const std::string &name);
		bool ReadTexmap(const std::string &name, const std::string &texmap);
		bool ReadCamera(std::istream &istr);

		// Rendering tools
		void ApplySpheresToCurrentProgram();
		void ApplyGlobalSettingsToCurrentProgram();
		void ApplyMaterialToCurrentProgram(SimpleMaterial &mtl, bool useMaps);
		void DisableCurrentTextures();

	private:
		BoundingBoxf boundingBox;

		TSimpleResourceManager<GLuint> textureUnits;
		void InitTexUnits();
		void KillTexUnits();

	public:
		GLuint GetTexUnit() { return textureUnits.Create(); }
		void FreeTexUnit(GLuint &id)
		{
			textureUnits.Delete(id);
			id = 0;
		}
		void FreeTexUnit(GLint &id)
		{
			if (id > 0)
			{
				GLuint tid = static_cast<GLuint>(id);
				textureUnits.Delete(tid);
				id = 0;
			}
		}

	public:
		SimpleSceneGraph();
		~SimpleSceneGraph();

		/// <summary>Reset() is a method of SimpleSceneGraph.
		/// <para>Resets scene graph to initial conditions. It's completely empty with default values.</para></summary>
		void Reset();

		bool Load(const std::string &filename);
		bool Save(const std::string &filename);

		const BoundingBoxf &GetBoundingBox();

		/// Note this is where we went wrong, by including rendering information in the scene graph...

		void BuildBuffers();
		void Render(SimpleProgram &program);
		void RenderZOnly(SimpleProgram &program);

		void Render(SimpleProgram &program, bool useMaterials, bool useMaps, bool useZOnly, Matrix4f &projectionMatrix, Matrix4f &cameraMatrix);

		void AdvancedRender(SimpleRenderConfiguration &rc);
		void AdvancedRenderZOnly(const SimpleRenderConfiguration &rc) const;
		void RenderZOnly(SimpleProgramPtr &program) const;

		void SetUniforms(SimpleProgramPtr &shader);

		void InitSphls();
		void MakeSphlsUnclean();
	};

	void RenderCubeShadowMap(const SimpleSceneGraph &ssg, SimpleCubeTexture &scs, const SimpleRenderConfiguration &rc);
} // namespace Fluxions

#endif
