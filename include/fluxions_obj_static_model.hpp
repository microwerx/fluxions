#ifndef FLUXIONS_OBJ_STATIC_MODEL_HPP
#define FLUXIONS_OBJ_STATIC_MODEL_HPP

#include <string>
#include <vector>
#include <fluxions_gte.hpp>
#include <fluxions_simple_renderer.hpp>

namespace Fluxions
{
	class OBJStaticModel
	{
	public:
		enum SurfaceType
		{
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
			unsigned mode = SurfaceType::Triangles;
			unsigned first = 0;
			unsigned count = 0;
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
		void RenderGL1();

		std::vector<Vertex> Vertices;
		std::vector<unsigned> Indices;
		std::vector<Surface> Surfaces;
		BoundingBoxf BoundingBox;
	};
}

#endif