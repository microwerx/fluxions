#ifndef FLUXIONS_SIMPLE_GEOMETRY_MESH_HPP
#define FLUXIONS_SIMPLE_GEOMETRY_MESH_HPP

#include <fluxions_base.hpp>

namespace Fluxions {
	class SimpleGeometryMesh {
	public:
		// These constants match the GL_POINTS, GL_LINES, ... constants
		enum SurfaceType {
			Points = 0,
			Lines = 1,
			LineLoop = 2,
			LineStrip = 3,
			Triangles = 4,
			TriangleStrip = 5,
			TriangleFan = 6
		};

		struct Vertex {
			Vector3f position;
			Vector3f normal;
			Vector2f texcoord;
			Color4f color;
			Vector3f tangent;
			Vector3f binormal;

			// future, for padding right now
			Vector4ub boneIndex;
			Vector4f boneWeights;
			float sh[9];
		};

		struct Surface {
			unsigned mode = SurfaceType::Triangles;
			unsigned first = 0;
			unsigned count = 0;
			std::string materialLibrary;
			std::string materialName;
			std::string surfaceName;
			int materialId = -1;
		};

		SimpleGeometryMesh();
		~SimpleGeometryMesh();

		bool loadOBJ(const std::string& filename);
		bool saveOBJ(const std::string& filename) const;
		int saveOBJByMaterial(const std::string& filename,
							  const std::string& mtllib,
							  const std::string& materialName) const;
		int saveOBJByMaterial(const std::string& filename,
							  const std::string& materialName,
							  int materialId) const;
		bool saveCache(const std::string& filename) const;
		bool loadCache(const std::string& filename);
		void computeTangentVectors();
		void clear();
		void resize(int vertexCount, int indexCount, int surfaceCount = 1);
		void createSimpleModel(int vertexCount, int indexCount, int surfaceCount = 1);
		void transform(const Matrix4f& mat);

		// Drawing commands
	private:
		Vertex curVertexAttrib_;
		std::string currentMaterial_;
		std::string currentMaterialLibrary_;
		bool dirty{ true };
	public:
		int getIndexCount() const { return (int)Indices.size(); }
		int getVertexCount() const { return (int)Vertices.size(); }

		Vertex& getVertex(int i) { return Vertices[i]; }

		inline void setMaterial(const std::string& mtl) { currentMaterial_ = mtl; }
		inline void setMaterialLibrary(const std::string& mtllib) { currentMaterialLibrary_ = mtllib; }

		inline void beginSurface(SurfaceType mode) {
			Surface newSurface;
			newSurface.mode = mode;
			newSurface.first = getIndexCount();
			newSurface.count = 0;
			newSurface.materialLibrary = currentMaterialLibrary_;
			newSurface.materialName = currentMaterial_;
			Surfaces.push_back(newSurface);
		}

		void position3f(float x, float y, float z, bool addIndex_ = false) {
			curVertexAttrib_.position.reset(x, y, z);
			Vertices.push_back(curVertexAttrib_);
			dirty = true;
			if (addIndex_) {
				addIndex(-1);
			}
		}

		void position3f(const Vector3f v, bool addIndex_ = false) {
			curVertexAttrib_.position = v;
			Vertices.push_back(curVertexAttrib_);
			dirty = true;
			if (addIndex_) {
				addIndex(-1);
			}
		}

		void normal3f(float x, float y, float z) {
			curVertexAttrib_.normal.reset(x, y, z);
		}

		void texcoord2f(float s, float t) {
			curVertexAttrib_.texcoord.reset(s, t);
		}

		void color3f(float r, float g, float b) {
			curVertexAttrib_.color.reset(r, g, b, 1.0f);
		}

		void color3f(const Color3f color) {
			curVertexAttrib_.color = color;
		}

		void color4f(float r, float g, float b, float a) {
			curVertexAttrib_.color.reset(r, g, b, a);
		}

		void tangent3f(float x, float y, float z) {
			curVertexAttrib_.tangent.reset(x, y, z);
		}

		void binormal3f(float x, float y, float z) {
			curVertexAttrib_.binormal.reset(x, y, z);
		}

		inline void attrib4f(int i, float x, float y, float z, float w, bool addIndex_ = false) {
			switch (i) {
			case 0: curVertexAttrib_.position.reset(x, y, z);
				break;
			case 1: curVertexAttrib_.normal.reset(x, y, z);
				break;
			case 2: curVertexAttrib_.texcoord.reset(x, y);
				break;
			case 3: curVertexAttrib_.color.reset(x, y, z, w);
				break;
			case 4: curVertexAttrib_.tangent.reset(x, y, z);
				break;
			case 5: curVertexAttrib_.binormal.reset(x, y, z);
				break;
			default:
				return;
			}
			if (i == 0) {
				Vertices.push_back(curVertexAttrib_);
				dirty = true;
				if (addIndex_) {
					addIndex(-1);
				}
			}
		}

		inline void addIndex(int i = -1) {
			if (i < 0)
				Indices.push_back(getIndexCount());
			else if (within(i, 0, getVertexCount() - 1))
				Indices.push_back(i);
			else
				return;
			if (!Surfaces.empty())
				Surfaces.back().count++;
			dirty = true;
		}

		inline void attrib3f(int i, float x, float y, float z, bool addIndex = false) { attrib4f(i, x, y, z, 1.0f, addIndex); }
		inline void attrib2f(int i, float x, float y, bool addIndex = false) { attrib4f(i, x, y, 0.0f, 1.0f, addIndex); }
		inline void attrib1f(int i, float x, bool addIndex = false) { attrib4f(i, x, 0.0f, 0.0f, 1.0f, addIndex); }

		inline void attrib4f(int i, const Vector4f& v, bool addIndex = false) { attrib4f(i, v.x, v.y, v.z, v.w, addIndex); }
		inline void attrib3f(int i, const Vector3f& v, bool addIndex = false) { attrib4f(i, v.x, v.y, v.z, 1.0f, addIndex); }
		inline void attrib2f(int i, const Vector2f& v, bool addIndex = false) { attrib4f(i, v.x, v.y, 0.0f, 1.0f, addIndex); }
		inline void attrib4f(int i, const Color4f& v, bool addIndex = false) { attrib4f(i, v.r, v.g, v.b, v.a, addIndex); }
		inline void attrib3f(int i, const Color3f& v, bool addIndex = false) { attrib4f(i, v.r, v.g, v.b, 1.0f, addIndex); }

		// Memory
		inline const void* getVertexData() const { return &Vertices[0]; }
		inline size_t getVertexDataSize() const { return (size_t)(sizeof(Vertex) * Vertices.size()); }
		inline const void* getIndexData() const { return &Indices[0]; }
		inline size_t getIndexDataSize() const { return (size_t)(sizeof(unsigned) * Indices.size()); }
		bool isAttribEnabled(int i) const { return (i >= 0 && i <= 3); }
		const char* getAttribName(int i) const {
			switch (i) {
			case 0: return "aPosition";
			case 1: return "aNormal";
			case 2: return "aTexCoord";
			case 3: return "aColor";
			}
			return nullptr;
		}
		bool isAttribNormalized(int i) const { if (i < 0) return true; return false; }
		int getVertexOffset(int i) const {
			switch (i) {
			case 0: return 0;
			case 1: return 3 * sizeof(float);
			case 2: return 6 * sizeof(float);
			case 3: return 8 * sizeof(float);
			}
			return 0;
		}

		// Properties
		std::string name;
		std::string path;
		std::map<std::string, std::string> mtllibs;
		std::map<std::string, std::string> Materials;
		std::vector<Vertex> Vertices;
		std::vector<unsigned> Indices;
		std::vector<Surface> Surfaces;
		BoundingBoxf BoundingBox;
	private:
		// note: modifies mtllibname
		bool add_mtllib(std::istream& istr, std::string& mtllibname, const std::string& basepath);
	};
}

#endif
