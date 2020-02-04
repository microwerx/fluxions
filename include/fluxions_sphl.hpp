#ifndef FLUXIONS_SPHL_HPP
#define FLUXIONS_SPHL_HPP

#include <fluxions_ssphh.hpp>
#include <fluxions_ssg_ssphh_light.hpp>

namespace Fluxions {
	struct FxModel {
		std::vector<Vector3f> vertices;
		std::vector<Vector3i> triangles;
		unsigned vertexCount = 0;
		unsigned triangleCount = 0;

		void load(const char* path);
		void makeModel(SimpleGeometryMesh& model);
	};

	struct Sphl {
		// constants
		const static int MaxCoefficients = 121;
		const static int MaxComponents = 6;
		const static int RedComponent = 0;
		const static int GreenComponent = 1;
		const static int BlueComponent = 2;
		const static int LuminanceComponent = 3;
		const static int HierarchySelfComponent = 4;
		const static int HierarchyNeighborComponent = 5;

		// shader parameters
		bool enabled = false;
		// if w = 1, then light source is treated as a local light (V = Vertex Position - Light Position)
		// if w = 0, then light source is treated as a global light (V = Vertex Normal)
		Vector4f position;
		Color4f E0;
		// light probe texture 0 = Unsampled raw light probe
		// light probe texture 1 = SPH precomputed light probe
		// light probe texture 2 = ??? SPH precomputed Y values (degrees 0 and 1)
		GLuint lightProbeTexIds[3] = { 0, 0, 0 };
		// SPHL coefficients
		int maxDegree = 0;
		bool makeSimpleSPHLs = false;
		Vector4f coefs[MaxCoefficients];

		// other parameters
		float v_coefs[MaxComponents][MaxCoefficients];
		SimpleGeometryMesh sph_model;
		SimpleGeometryMesh lightProbeModel;
		Image4f lightProbe;

		Sphl();

		void randomize();
		void copyCoefficients(const SimpleSSPHHLight& sphl, int maxDegrees = -1, bool simpleSPHLs = false);
		void createMesh(FxModel& model);
		void createLightProbe();
	};
} // namespace Fluxions

#endif
