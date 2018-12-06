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
#include "stdafx.h"
#include <fluxions_sphl.hpp>


namespace Fluxions
{


	void FxModel::load(const char *path)
	{
		ifstream fin(path);
		if (fin)
		{
			fin >> vertexCount;
			vertices.resize(vertexCount);
			for (int i = 0; fin && i < vertexCount; i++)
			{
				fin >> vertices[i].x >> vertices[i].y >> vertices[i].z;
				//vertices[i] *= 5.0;
			}

			fin >> triangleCount;
			triangles.resize(triangleCount);
			for (int i = 0; fin && i < triangleCount; i++)
			{
				fin >> triangles[i].x >> triangles[i].y >> triangles[i].z;
			}
			fin.close();
		}
	}


	void FxModel::makeModel(SimpleGeometryMesh &model)
	{
	}


	Sphl::Sphl()
	{
		sph_model.SetAttribName(0, "aPosition");
		sph_model.SetAttribName(1, "aNormal");
		sph_model.SetAttribName(2, "aColor");
		sph_model.SetAttribName(3, "aTexCoord");

		lightProbeModel.SetAttribName(0, "aPosition");
		lightProbeModel.SetAttribName(1, "aNormal");
		lightProbeModel.SetAttribName(2, "aColor");
		lightProbeModel.SetAttribName(3, "aTexCoord");
	}


	void Sphl::randomize()
	{
		for (int i = 0; i < MaxCoefficients; i++)
		{
			v_coefs[0][i] = coefs[i].r = randomSampler(0.18f, 2.2f);
			v_coefs[1][i] = coefs[i].g = randomSampler(0.18f, 2.2f);
			v_coefs[2][i] = coefs[i].b = randomSampler(0.18f, 2.2f);
			v_coefs[3][i] = coefs[i].a = coefs[i].xyz().dot(Vector3f(0.299f, 0.587f, 0.114f));
			v_coefs[4][i] = randomSampler(0.18f, 0.78f);
			v_coefs[5][i] = randomSampler(0.18f, 0.78f);
		}
	}


	void Sphl::copyCoefficients(const SimpleSSPHHLight & sphl, int maxDegrees, bool simpleSPHLs)
	{
		if (maxDegrees < 0)
		{
			maxDegree = sphl.maxDegree;
		}
		else
		{
			maxDegree = min(sphl.maxDegree, maxDegrees);
		}

		makeSimpleSPHLs = simpleSPHLs;

		// clear out the entire coefficient matrix
		for (int j = 0; j <= MaxComponents; j++)
		{
			for (int i = 0; i < MaxCoefficients; i++)
			{
				v_coefs[j][i] = 0.0f;
			}
		}

		for (int j = 0; j < 4; j++)
		{
			int lmmax = GetMaxSphCoefficients(maxDegree);
			for (int lm = 0; lm <= lmmax; lm++)
			{
				v_coefs[j][lm] = sphl.E0 * sphl.msph[j][lm];
			}
		}

		int j = HierarchySelfComponent;
		for (int lm = 0; lm < sphl.self[3].getMaxCoefficients(); lm++)
		{
			v_coefs[j][lm] = sphl.self[3].getCoefficient(lm);
		}

		j = HierarchyNeighborComponent;
		for (int lm = 0; lm < sphl.neighbor[3].getMaxCoefficients(); lm++)
		{
			v_coefs[j][lm] = sphl.neighbor[3].getCoefficient(lm);
		}
	}


	void Sphl::createMesh(FxModel &model)
	{
		sph_model.Reset();
		sph_model.Attrib3f(1, Vector3f(0.0f, 0.0f, 0.0f)); // Normal is 0, we will add neighboring face normals in a bit

		float theta = 0.0f;
		float dtheta = (float)FX_DEGREES_TO_RADIANS;
		float dxs = 0.1f;
		float dx = dxs * sin(theta += dtheta);
		const float scale = 0.25f;
		// const int MaxComponents = 6;
		// Red, Green, Blue, Monochromatic, Hierarchy Self, Hierarchy Neighbors

		Vector3f Colors[MaxComponents] = {
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.5f },
			{ 0.0f, 1.0f, 0.5f }
		};

		Vector3f InvColors[MaxComponents] = {
			{ 0.3f, 0.0f, 0.0f },
			{ 0.0f, 0.3f, 0.0f },
			{ 0.0f, 0.0f, 0.3f },
			{ 0.3f, 0.3f, 0.3f },
			{ 0.3f, 0.0f, 0.15f },
			{ 0.0f, 0.15f, 0.3f }
		};

		int firstIndex = 0;
		int lastIndex = MaxComponents - 1;
		if (makeSimpleSPHLs)
		{
			firstIndex = lastIndex = 3;
		}

		const float dist = scale * 4.0f;
		Vector3f offsets[MaxComponents] = {
			{ dist * cosf(90.0f * FX_F32_DEGREES_TO_RADIANS), dist * sinf(90.0f * FX_F32_DEGREES_TO_RADIANS), 0.0f },
			{ dist * cosf(210.0f * FX_F32_DEGREES_TO_RADIANS), dist * sinf(210.0f * FX_F32_DEGREES_TO_RADIANS), 0.0f },
			{ dist * cosf(330.0f * FX_F32_DEGREES_TO_RADIANS), dist * sinf(330.0f * FX_F32_DEGREES_TO_RADIANS), 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ dist, 0.0f, 0.0f },
			{ -dist, 0.0f, 0.0f }
		};

		float scales[MaxComponents] = {
			scale * 0.5f,
			scale * 0.5f,
			scale * 0.5f,
			scale,
			scale,
			scale
		};

		for (int j = firstIndex; j <= lastIndex; j++)
		{
			for (int i = 0; i < model.vertexCount; i++)
			{
				float sph = calc_spherical_harmonic<float>(maxDegree, v_coefs[j], model.vertices[i].theta(), model.vertices[i].phi());
				if (sph > 0.0)
				{
					//sph = 1.0f + sph;
					sph_model.Attrib3f(2, Colors[j]); // Red SPH Color
				}
				else
				{
					sph = -sph;// sph = 1.0f - sph;
					sph_model.Attrib3f(2, InvColors[j]); // (Opposite) Red SPH Color
				}
				sph_model.Attrib3f(0, offsets[j] + scales[j] * sph * model.vertices[i]); // Position
			}
		}

		int k = 0;
		for (int j = firstIndex; j <= lastIndex; j++)
		{
			sph_model.BeginSurface(SimpleGeometryMesh::SurfaceType::Triangles);
			for (int i = 0; i < model.triangleCount; i++)
			{
				int v0 = k * model.vertexCount + model.triangles[i].v[0];
				int v1 = k * model.vertexCount + model.triangles[i].v[1];
				int v2 = k * model.vertexCount + model.triangles[i].v[2];
				Vector3f dp1 = sph_model.GetVertex(v1).attribs[0].xyz() - sph_model.GetVertex(v0).attribs[0].xyz();
				Vector3f dp2 = sph_model.GetVertex(v2).attribs[0].xyz() - sph_model.GetVertex(v0).attribs[0].xyz();
				Vector3f N = dp1.cross(dp2).norm();
				sph_model.GetVertex(v0).attribs[1] += N;
				sph_model.GetVertex(v1).attribs[1] += N;
				sph_model.GetVertex(v2).attribs[1] += N;

				sph_model.AddIndex(k * model.vertexCount + model.triangles[i].v[0]);
				sph_model.AddIndex(k * model.vertexCount + model.triangles[i].v[1]);
				sph_model.AddIndex(k * model.vertexCount + model.triangles[i].v[2]);

			}
			k++;
		}

		for (int j = firstIndex; j <= lastIndex; j++)
		{
			for (int i = 0; i < model.triangleCount; i++)
			{
				int v0 = j * model.vertexCount + model.triangles[i].v[0];
				int v1 = j * model.vertexCount + model.triangles[i].v[1];
				int v2 = j * model.vertexCount + model.triangles[i].v[2];
				sph_model.GetVertex(v0).attribs[1].normalize();
				sph_model.GetVertex(v1).attribs[1].normalize();
				sph_model.GetVertex(v2).attribs[1].normalize();
			}
		}

		lightProbeModel.Attrib4f(2, Vector4f(-1.0f, -1.0f, -1.0f, 1.0f), false);
	}


	void Sphl::createLightProbe()
	{
		// create light probe
		lightProbe.resize(32, 32, 6);
		for (int face = 0; face < lightProbe.depth(); face++)
		{
			for (int s = 0; s < lightProbe.width(); s++)
			{
				for (int t = 0; t < lightProbe.height(); t++)
				{
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
		if (lightProbeTexIds[2] != 0)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, lightProbeTexIds[2]);
			for (int face = 0; face < lightProbe.depth(); face++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA, lightProbe.width(), lightProbe.height(), 0, GL_RGBA, GL_FLOAT, lightProbe.getImageData(face));
			}
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	}

} // namespace Fluxions
