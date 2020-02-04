#include "fluxions_ssg_pch.hpp"
#include <fstream>
#include <fluxions_gte.hpp>
#include <fluxions_gte_spherical_harmonic.hpp>
#include <fluxions_gte_scalar_math.hpp>
#include <fluxions_sphl.hpp>

template class std::vector<Fluxions::TVector3<int>>;
template class std::vector<Fluxions::SimpleGeometryMesh::Vertex>;
template class std::vector<unsigned int>;
template class std::vector<Fluxions::TVector3<float>>;

namespace Fluxions {
	constexpr int POSITION = 0;
	constexpr int NORMAL = 1;
	constexpr int TEXCOORD = 2;
	constexpr int COLOR = 3;

	void FxModel::load(const char* path) {
		std::ifstream fin(path);
		if (fin) {
			fin >> vertexCount;
			vertices.resize(vertexCount);
			for (unsigned i = 0; fin && i < vertexCount; i++) {
				fin >> vertices[i].x >> vertices[i].y >> vertices[i].z;
				//vertices[i] *= 5.0;
			}

			fin >> triangleCount;
			triangles.resize(triangleCount);
			for (unsigned i = 0; fin && i < triangleCount; i++) {
				fin >> triangles[i].x >> triangles[i].y >> triangles[i].z;
			}
			fin.close();
		}
	}

	void FxModel::makeModel(SimpleGeometryMesh& model) {}

	Sphl::Sphl() {
		//sph_model.SetAttribName(POSITION, "aPosition");
		//sph_model.SetAttribName(NORMAL, "aNormal");
		//sph_model.SetAttribName(COLOR, "aColor");
		//sph_model.SetAttribName(TEXCOORD, "aTexCoord");

		//lightProbeModel.SetAttribName(POSITION, "aPosition");
		//lightProbeModel.SetAttribName(NORMAL, "aNormal");
		//lightProbeModel.SetAttribName(COLOR, "aColor");
		//lightProbeModel.SetAttribName(TEXCOORD, "aTexCoord");
	}

	void Sphl::randomize() {
		for (unsigned i = 0; i < MaxCoefficients; i++) {
			v_coefs[0][i] = coefs[i].x = randomSampler(0.18f, 2.2f);
			v_coefs[1][i] = coefs[i].y = randomSampler(0.18f, 2.2f);
			v_coefs[2][i] = coefs[i].z = randomSampler(0.18f, 2.2f);
			v_coefs[3][i] = coefs[i].w = coefs[i].xyz().dot(Vector3f(0.299f, 0.587f, 0.114f));
			v_coefs[4][i] = randomSampler(0.18f, 0.78f);
			v_coefs[5][i] = randomSampler(0.18f, 0.78f);
		}
	}

	void Sphl::copyCoefficients(const SimpleSSPHHLight& sphl, int maxDegrees, bool simpleSPHLs) {
		if (maxDegrees < 0) {
			maxDegree = sphl.maxDegree;
		}
		else {
			maxDegree = Fluxions::min2(sphl.maxDegree, maxDegrees);
		}

		makeSimpleSPHLs = simpleSPHLs;

		// clear out the entire coefficient matrix
		for (unsigned j = 0; j < MaxComponents; j++) {
			for (unsigned i = 0; i < MaxCoefficients; i++) {
				v_coefs[j][i] = 0.0f;
			}
		}

		for (unsigned j = 0; j < 4; j++) {
			unsigned lmmax = GetMaxSphCoefficients(maxDegree);
			for (unsigned lm = 0; lm < lmmax; lm++) {
				v_coefs[j][lm] = sphl.E0 * sphl.msph[j][lm];
			}
		}

		int j = HierarchySelfComponent;
		for (unsigned lm = 0; lm < sphl.self[3].getMaxCoefficients(); lm++) {
			v_coefs[j][lm] = sphl.self[3].getCoefficient(lm);
		}

		j = HierarchyNeighborComponent;
		for (unsigned lm = 0; lm < sphl.neighbor[3].getMaxCoefficients(); lm++) {
			v_coefs[j][lm] = sphl.neighbor[3].getCoefficient(lm);
		}
	}

	void Sphl::createMesh(FxModel& model) {
		sph_model.clear();
		sph_model.normal3f(0.0f, 0.0f, 0.0f); // Normal is 0, we will add neighboring face normals in a bit

		float theta = 0.0f;
		float dtheta = (float)FX_DEGREES_TO_RADIANS;
		float dxs = 0.1f;
		//float dx = dxs * sin(theta += dtheta);
		const float scale = 0.25f;
		// const int MaxComponents = 6;
		// Red, Green, Blue, Monochromatic, Hierarchy Self, Hierarchy Neighbors

		Color3f Colors[MaxComponents] = {
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 0.5f},
			{0.0f, 1.0f, 0.5f} };

		Color3f InvColors[MaxComponents] = {
			{0.3f, 0.0f, 0.0f},
			{0.0f, 0.3f, 0.0f},
			{0.0f, 0.0f, 0.3f},
			{0.3f, 0.3f, 0.3f},
			{0.3f, 0.0f, 0.15f},
			{0.0f, 0.15f, 0.3f} };

		unsigned firstIndex = 0;
		unsigned lastIndex = MaxComponents - 1;
		if (makeSimpleSPHLs) {
			firstIndex = lastIndex = 3;
		}

		const float dist = scale * 4.0f;
		Vector3f offsets[MaxComponents] = {
			{dist * cosf(90.0f * FX_F32_DEGREES_TO_RADIANS), dist * sinf(90.0f * FX_F32_DEGREES_TO_RADIANS), 0.0f},
			{dist * cosf(210.0f * FX_F32_DEGREES_TO_RADIANS), dist * sinf(210.0f * FX_F32_DEGREES_TO_RADIANS), 0.0f},
			{dist * cosf(330.0f * FX_F32_DEGREES_TO_RADIANS), dist * sinf(330.0f * FX_F32_DEGREES_TO_RADIANS), 0.0f},
			{0.0f, 0.0f, 0.0f},
			{dist, 0.0f, 0.0f},
			{-dist, 0.0f, 0.0f} };

		float scales[MaxComponents] = {
			scale * 0.5f,
			scale * 0.5f,
			scale * 0.5f,
			scale,
			scale,
			scale };

		for (unsigned j = firstIndex; j <= lastIndex; j++) {
			for (unsigned i = 0; i < model.vertexCount; i++) {
				float sph = calc_spherical_harmonic<float>(maxDegree, v_coefs[j], model.vertices[i].theta(), model.vertices[i].phi());
				if (sph > 0.0) {
					//sph = 1.0f + sph;
					sph_model.color3f(Colors[j]); // Red SPH Color
				}
				else {
					sph = -sph;							 // sph = 1.0f - sph;
					sph_model.color3f(InvColors[j]); // (Opposite) Red SPH Color
				}
				// Added this line to move SPH between 0.25 and 0.5
				sph = 0.25f + 0.25f * sph;
				sph_model.position3f(offsets[j] + scales[j] * sph * model.vertices[i]); // Position
			}
		}

		unsigned k = 0;
		for (unsigned j = firstIndex; j <= lastIndex; j++) {
			sph_model.beginSurface(SimpleGeometryMesh::SurfaceType::Triangles);
			for (unsigned i = 0; i < model.triangleCount; i++) {
				unsigned v0 = k * model.vertexCount + model.triangles[i].x;
				unsigned v1 = k * model.vertexCount + model.triangles[i].y;
				unsigned v2 = k * model.vertexCount + model.triangles[i].z;
				Vector3f dp1 = sph_model.getVertex(v1).position - sph_model.getVertex(v0).position;
				Vector3f dp2 = sph_model.getVertex(v2).position - sph_model.getVertex(v0).position;
				Vector3f N = dp1.cross(dp2).unit();
				sph_model.getVertex(v0).normal += N;
				sph_model.getVertex(v1).normal += N;
				sph_model.getVertex(v2).normal += N;

				sph_model.addIndex((int)(k * model.vertexCount + model.triangles[i].x));
				sph_model.addIndex((int)(k * model.vertexCount + model.triangles[i].y));
				sph_model.addIndex((int)(k * model.vertexCount + model.triangles[i].z));
			}
			k++;
		}

		for (unsigned j = firstIndex; j <= lastIndex; j++) {
			for (unsigned i = 0; i < model.triangleCount; i++) {
				unsigned v0 = j * model.vertexCount + model.triangles[i].x;
				unsigned v1 = j * model.vertexCount + model.triangles[i].y;
				unsigned v2 = j * model.vertexCount + model.triangles[i].z;
				sph_model.getVertex(v0).normal.normalize();
				sph_model.getVertex(v1).normal.normalize();
				sph_model.getVertex(v2).normal.normalize();
			}
		}

		lightProbeModel.texcoord2f(0.0f, 0.0f);
	}

	void Sphl::createLightProbe() {
		// create light probe
		lightProbe.resize(32, 32, 6);
		for (unsigned face = 0; face < lightProbe.depth(); face++) {
			for (unsigned s = 0; s < lightProbe.width(); s++) {
				for (unsigned t = 0; t < lightProbe.height(); t++) {
					float _s = (float)s / (float)lightProbe.width();
					float _t = (float)t / (float)lightProbe.height();
					Vector3f v;
					MakeCubeVectorFromFaceST(face, _s, 1.0f - _t, &v.x, &v.y, &v.z);
					v.normalize();
					Color4f color;
					float theta = v.theta();
					float phi = v.phi();

					color.r = calc_spherical_harmonic<float>(maxDegree, v_coefs[0], theta, phi);
					color.g = calc_spherical_harmonic<float>(maxDegree, v_coefs[1], theta, phi);
					color.b = calc_spherical_harmonic<float>(maxDegree, v_coefs[2], theta, phi);
					color.a = calc_spherical_harmonic<float>(maxDegree, v_coefs[3], theta, phi);

					// debug code to determine if vectors align on the cube map
					//color.r = sin(theta) * cos(phi);
					//color.g = cos(theta);
					//color.b = sin(theta) * sin(phi);
					//color.a = 1.0f;

					// debug code to determine if 3D position align on the cube map
					//color.r = 0.5 * color.r + 0.5;
					//color.g = 0.5 * color.g + 0.5;
					//color.b = 0.5 * color.b + 0.5;

					lightProbe.setPixelUnsafe(s, t, face, color);
				}
			}
		}
		if (lightProbeTexIds[2] != 0) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, lightProbeTexIds[2]);
			for (int face = 0; face < (int)lightProbe.depth(); face++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA, (GLsizei)lightProbe.width(), (GLsizei)lightProbe.height(), 0, GL_RGBA, GL_FLOAT, lightProbe.getImageData(face));
			}
			FxGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	}

} // namespace Fluxions
