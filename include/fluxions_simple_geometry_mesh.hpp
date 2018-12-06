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
#ifndef FLUXIONS_SIMPLE_GEOMETRY_MESH_HPP
#define FLUXIONS_SIMPLE_GEOMETRY_MESH_HPP


#include <vector>
#include <string>
#include <fluxions_gte_vector4.hpp>


namespace Fluxions
{
	using namespace std;

	class SimpleGeometryMesh
	{
	public:
		//SimpleGeometryMesh() { }
		//virtual ~SimpleGeometryMesh() { }

		class Vertex
		{
		public:
			Vertex(void) {
				for (int i = 0; i < 8; i++)
				{
					attribs[i].reset();
				}
			}
			Vertex(const Vertex &v) {
				for (int i = 0; i < 8; i++)
				{
					attribs[i] = v.attribs[i];

				}
			}

			union {
			//	struct {
			//		tvec4<float> attrib0;
			//		tvec4<float> attrib1;
			//		tvec4<float> attrib2;
			//		tvec4<float> attrib3;
			//		tvec4<float> attrib4;
			//		tvec4<float> attrib5;
			//		tvec4<float> attrib6;
			//		tvec4<float> attrib7;
			//	};
			//	Vector4f attribs[8];
			//};
			Vector4f attribs[8];
		};

		class AttribInfo
		{
		public:
			int location = -1;
			string name = "GenericAttribName";
			bool normalized = false;
			bool enabled = true;
		};

		using Index = unsigned;// unsigned short;

		enum SurfaceType {
			//#define GL_LINES 0x0001
			//#define GL_LINE_LOOP 0x0002
			//#define GL_LINE_STRIP 0x0003
			//#define GL_TRIANGLES 0x0004
			//#define GL_TRIANGLE_STRIP 0x0005
			//#define GL_TRIANGLE_FAN 0x0006
			Points = 0,
			Lines = 1,
			LineLoop = 2,
			LineStrip = 3,
			Triangles = 4,
			TriangleStrip = 5,
			TriangleFan = 6
		};

		struct Surface
		{
			string materialName;
			SurfaceType type = SurfaceType::Triangles;
			int first;
			int count;
		};

		using VertexPtr = Vertex *;
		using IndexPtr = Index *;
		using SurfacePtr = Surface *;

		inline void Attrib4f(int i, float x, float y, float z, float w, bool addIndex = false) {
			if (!within(i, 0, 7)) return;
			curVertexAttrib.attribs[i].x = x;
			curVertexAttrib.attribs[i].y = y;
			curVertexAttrib.attribs[i].z = z;
			curVertexAttrib.attribs[i].w = w;
			if (i == 0) {
				vertices.push_back(curVertexAttrib);
				dirty = true;
				if (addIndex) {
					AddIndex(-1);
				}
			}	
		}

		inline void Attrib3f(int i, float x, float y, float z, bool addIndex = false) { Attrib4f(i, x, y, z, 1.0f, addIndex); }
		inline void Attrib2f(int i, float x, float y, bool addIndex = false) { Attrib4f(i, x, y, 0.0f, 1.0f, addIndex); }
		inline void Attrib1f(int i, float x, bool addIndex = false) { Attrib4f(i, x, 0.0f, 0.0f, 1.0f, addIndex); }

		inline void Attrib4f(int i, const Vector4f &v, bool addIndex = false) { Attrib4f(i, v.x, v.y, v.z, v.w, addIndex); }
		inline void Attrib3f(int i, const Vector3f &v, bool addIndex = false) { Attrib4f(i, v.x, v.y, v.z, 1.0f, addIndex); }
		inline void Attrib2f(int i, const Vector2f &v, bool addIndex = false) { Attrib4f(i, v.x, v.y, 0.0f, 1.0f, addIndex); }

		void Reset();
		void BeginSurface(SurfaceType type);
		inline void AddIndex(int i = -1) {
			if (i < 0)
				indices.push_back(GetIndexCount());
			else if (within(i, 0, GetVertexCount() - 1))
				indices.push_back(i);
			else
				return;
			if (!surfaces.empty())
				surfaces.back().count++;
			dirty = true;
		}
		inline void SetMaterial(const string &materialName) { curMaterialName = materialName; }
		inline Surface & GetSurface(int i) {
			if (within(i, 0, (int)surfaces.size())) {
				return surfaces[i];
			}
			return blahSurface;
		}
		inline const Surface & GetSurface(int i) const {
			if (within(i, 0, (int)surfaces.size())) {
				return surfaces[i];
			}
			return blahSurface;
		}

		inline const vector<Surface> & GetSurfaces() const { return surfaces; }
		inline const vector<Vertex> & GetVertices() const { return vertices; }
		inline const vector<Index> & GetIndices() const { return indices; }

		inline Vertex & GetVertex(int i) { if (within(i, 0, GetVertexCount())) return vertices[i]; return blahVertex; }
		inline int GetVertexCount() const { return (int)vertices.size(); }
		inline int GetVertexSize() const { return (int)sizeof(Vertex); }
		inline int GetVertexOffset(int i) const { if (within(i, 0, 7)) return i * sizeof(Vector4f); return 0; }
		inline const void * GetVertexData() const { return &vertices[0]; }
		inline size_t GetVertexDataSize() const { return (size_t)(sizeof(Vertex) * vertices.size()); }
		inline Vertex * GetVertexData() { return vertices.empty() ? nullptr : &vertices[0]; }

		inline Index & GetIndex(int i) { if (within(i, 0, GetIndexCount())) return indices[i]; return blahIndex; }
		inline Index GetIndexCount() const { return (Index)indices.size(); }
		inline int GetIndexSize() const { return sizeof(Index); }
		inline const void *GetIndexData() const { return &indices[0]; }
		inline size_t GetIndexDataSize() const { return (size_t)(sizeof(Index) * indices.size()); }
		inline Index *GetIndexData() { return indices.empty() ? nullptr : &indices[0]; }

		inline void EnableAttrib(int i) { if (within(i, 0, 7)) attribInfo[i].enabled = true; }
		inline void DisableAttrib(int i) { if (within(i, 0, 7)) attribInfo[i].enabled = false; }
		inline bool IsAttribEnabled(int i) const { if (within(i, 0, 7)) return attribInfo[i].enabled; return false; }
		inline bool IsAttribNormalized(int i) const { if (within(i, 0, 7)) return attribInfo[i].normalized; return false; }
		inline const char *GetAttribName(int i) const { if (within(i, 0, 7)) return attribInfo[i].name.c_str(); return nullptr; }
		inline void SetAttribName(int i, const string &name) { if (within(i, 0, 7)) attribInfo[i].name = name; }
		//inline void SetAttribName(int i, const char *name) { if (within(i, 0, 7)) attribInfo[i].name = name; }
		inline void SetAttribNormalized(int i, bool normalized) { if (within(i, 0, 7)) attribInfo[i].normalized = normalized; }
		inline int GetAttribLocation(const string &name) const { for (int i = 0; i < 8; i++) { if (attribInfo[i].name == name) return i; } return -1; }
		inline bool IsDirty() const { return dirty; }

		bool SaveOBJ(const string & path, bool output_normals = true, bool output_texcoords = true);
	protected:
		string curMaterialName;
		Vertex curVertexAttrib;
		vector<AttribInfo> attribInfo = {
			{ 0, "aPosition", false, true },
			{ 1, "aNormal", false, true },
			{ 2, "aColor", false, true },
			{ 3, "aTexCoord", false, true },
			{ 4, "aGenericAttribute1", false, false },
			{ 5, "aGenericAttribute2", false, false },
			{ 6, "aGenericAttribute3", false, false },
			{ 7, "aGenericAttribute4", false, false }
		};
		bool dirty = false;
		vector<Vertex> vertices;
		vector<Index> indices;
		vector<Surface> surfaces;

		// these are for returning references that are out of range
		Surface blahSurface;
		Vertex blahVertex;
		Index blahIndex;

		inline bool within(int i, int a, int b) const { return i >= a && i <= b; }
	};
}

#endif