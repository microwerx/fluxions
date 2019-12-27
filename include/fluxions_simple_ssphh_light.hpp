#ifndef FLUXIONS_SIMPLE_SSPHH_LIGHT_HPP
#define FLUXIONS_SIMPLE_SSPHH_LIGHT_HPP

#include <vector>
#include <fluxions_gte.hpp>
#include <fluxions_gte_spherical_harmonic.hpp>
#include <fluxions_gte_image.hpp>
#include <fluxions_simple_scene_graph.hpp>
#include <fluxions_renderer_texture_cube.hpp>

namespace Fluxions
{
	class SimpleSSPHHLight : public SimpleSceneGraphNode
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

		// This holds a cubemap representation of the path traced (ptrc) 3D light probe
		Image4f ptrcLightProbeImage;
		RendererTextureCube ptrcLightProbeTexture;
		float ptrcLightProbePercent = 0.0f;

		// This holds a cube map representation of the msph for this SPHL
		Image4f msphLightProbeImage;
		RendererTextureCube msphLightProbeTexture;

		// This holds the finalized SELF + NEIGHBOR light probe
		//SSPHHLightHierarchy hierSph;
		Image4f hierLightProbeImage;
		RendererTextureCube hierLightProbeTexture;

		RendererTextureCube depthSphlMap;
		RendererTextureCube colorSphlMap;
		RendererTextureCube environmentLightProbeMap;

		SimpleSSPHHLight();
		~SimpleSSPHHLight();

		// Reads from a Path Traced Light Probe (a cube map stored images from left to right in a single image).
		bool readPtrcLightProbe(const std::string& path);
		// Saves to a Path Traced Light Probe (a cube map with cube faces stored from left to right in a single image).
		bool savePtrcLightProbe(const std::string& path);

		// Saves a JSON form of the multispectral (RGBL) of this SPH. L represents a monochromatic version of the RGB components. { maxDegree: (1-10), coefs : [] }
		bool saveJsonSph(const std::string& path);
		// Reads a JSON format of a multispectral (RGBL) of this SPH. L represents a monochromatic version of the RGB components. { maxDegree: (1-10), coefs : [] }
		bool readJsonSph(const std::string& path);

		// Writes name.obj and name.mtl to the path
		bool saveOBJ(const std::string& path, const std::string& name);

		bool lightProbeToSph(const Image4f& lightProbe, MultispectralSph4f& sph);
		bool sphToLightProbe(const MultispectralSph4f& sph, Image4f& lightProbe);
		bool sphToLightProbe(const MultispectralSph4f& sph, Image4f& lightProbe, int maxDegree);
		bool uploadLightProbe(Image4f& lightProbe, RendererTextureCube& texture);
		bool uploadLightProbe(Image4f& lightProbe, GLuint& texture);

		Color3f getCoefficientColor(int l, int m) const;
		float getCoefficient(int j, int l, int m) const;
		float getCoefficient(int j, int lm) const;
		void zeroCoefficients();
		void isolateCoefficient(int j, int l, int m);
		void setCoefficient(int j, int l, int m, float value);
		void randomizeCoefficient(int j, int l, int m, float minValue, float maxValue);
		inline void standardize() { MakeIntensityChannel4f(msph); }
		void randomizeCoefficients(float size = 1.0f);

		void changeDegrees(int degrees);
		inline void incDegrees() { changeDegrees(maxDegree + 1); }
		inline void decDegrees() { changeDegrees(maxDegree - 1); }

		void resizeHierarchies(int size);

		void makeDirty();

		inline bool isDirty() { return dirty; }
		inline int getMaxCoefficients() const { return maxDegree * (maxDegree + 1) + maxDegree + 1; }
		inline int getMaxCoefficients(int degree) { return degree * (degree + 1) + degree + 1; }
		inline void makeLuminanceChannel() { MakeLuminanceChannel4f(msph); }
		inline void makeIntensityChannel() { MakeIntensityChannel4f(msph); }

		// Hierarchies
		void sortHierarchiesByPercent();
		void sortHierarchiesByIndex();
		void copySphlToHierarchies();

		void setHierarchyDescriptionToIndex();
		void setHierarchyDescriptionToPercent();
		void setHierarchyDescription();
	};
}

#endif