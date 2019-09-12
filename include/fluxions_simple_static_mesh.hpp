#ifndef FLUXIONS_SIMPLE_STATIC_MESH_HPP
#define FLUXIONS_SIMPLE_STATIC_MESH_HPP

#include <string>
#include <vector>
#include <fluxions_gte.hpp>

namespace Fluxions
{
	class OBJStaticModel
	{
	public:
		struct Vertex
		{
			Vector3f position;
			Vector3f normal;
			Vector2f texcoord;
			Vector3f tangent;
			Vector3f binormal;
		};

		struct Surface
		{
			GLenum mode = GL_TRIANGLES;
			GLint first = 0;
			GLsizei count = 0;
			std::string materialName;
			std::string surfaceName;
		};

	public:
		OBJStaticModel();
		~OBJStaticModel();

		bool LoadOBJ(const std::string& filename);
		bool SaveOBJ(const std::string& filename);
		void SavePrecompiled(const std::string& filename, const std::string objname);
		void LoadPrecompiled(const int numIndices, const int* indices, const int numVertices, const float* vertices, const int numSurfaces, const OBJSurface* surfaces);
		void ComputeTangentVectors();
		void Clear();
		void Resize(int vertexCount, int indexCount, int surfaceCount = 1);
		void CreateSimpleModel(int vertexCount, int indexCount, int surfaceCount = 1);
		void Transform(const Matrix4d& mat);

		void Render(SimpleRenderer_GLuint& renderer);
		void RenderGL11();

		std::vector<Vertex> Vertices;
		std::vector<unsigned> Indices;
		std::vector<Surface> Surfaces;
		BoundingBoxf BoundingBox;
	};
}

#endif