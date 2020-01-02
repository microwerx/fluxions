// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2019 Jonathan Metzgar
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
#ifndef FLUXIONS_SIMPLE_RENDERER_HPP
#define FLUXIONS_SIMPLE_RENDERER_HPP

#include <memory>
#include <fluxions_gte.hpp>
#include <fluxions_opengl.hpp>
#include <fluxions_simple_materials.hpp>
#include <fluxions_simple_vertex.hpp>
#include <fluxions_simple_surface.hpp>
#include <fluxions_simple_geometry_mesh.hpp>
#include <fluxions_renderer_program.hpp>

namespace Fluxions
{
	/// <summary>SimpleRenderer handles the needs of several different rendering approaches</summary>
	/// It is designed to accomodate a variety of different rendering options.
	/// Z Only. This outputs a 3-float position only for 12 bytes/vertex.
	/// Slow Vertex. This outputs 8 generic 4 component float vertex attributes for 128 bytes/vertex.
	/// Fast Vertex. This uses mostly integer inputs to reduce size to 32 bytes/vertex.
	/// <code>RenderZOnly()</code> renders all surfaces as Z only.
	/// <code>Render()</code> renders all surfaces as slow/fast vertex according to how data was input.
	template <typename IndexType, GLenum GLIndexType>
	class SimpleRenderer
	{
	private:
		GLuint arrayBuffer = 0;        // memory structure [ZONLY, FAST VERTICES, SLOW VERTICES]
		GLuint elementArrayBuffer = 0; // memory structure [ZONLY, FAST VERTICES, SLOW VERTICES]

		std::vector<GLubyte> vertexMemoryBuffer;
		std::vector<GLubyte> indexMemoryBuffer;

		std::vector<SimpleZVertex> zVertices;
		std::vector<SimpleFastVertex> fastVertices;
		std::vector<SimpleSlowVertex> slowVertices;
		std::vector<IndexType> Indices;
		std::vector<IndexType> zIndices;
		std::vector<SimpleSurface> surfaces;

		struct BUFFERINFO
		{
			GLsizei zVertexOffset = 0;
			GLsizei zVertexSize = 0;
			GLsizei fastVertexOffset = 0;
			GLsizei fastVertexSize = 0;
			GLsizei slowVertexOffset = 0;
			GLsizei slowVertexSize = 0;

			GLsizei zIndexOffset = 0;
			GLsizei zIndexSize = 0;
			GLsizei IndexOffset = 0;
			GLsizei IndexSize = 0;

			GLsizei vertexBufferSizeInBytes = 0;
			GLsizei indexBufferSizeInBytes = 0;
		};

		BUFFERINFO bufferInfo;

		SimpleFastVertex currentFastVertex;
		SimpleSlowVertex currentSlowVertex;
		SimpleBoneVertex currentBoneVertex;

		IndexType baseZIndex = 0;
		IndexType baseFastIndex = 0;
		IndexType baseSlowIndex = 0;

		GLuint currentMtlLibId = 0;
		GLuint currentMtlId = 0;
		GLuint currentGroupId = 0;
		GLuint currentObjectId = 0;
		GLuint currentProgramId = 0;
		std::string currentMtlName;
		std::string currentMtlLibName;
		std::string currentObjectName;
		std::string currentGroupName;

		VertexType lastVertexType = VertexType::UNDECIDED;

		// Vertex Array Objects
		GLuint zVAO = 0;
		GLuint slowVAO = 0;
		GLuint fastVAO = 0;

		bool isMakingSurface = false;
		unsigned currentSurface = 0;

		void BuildMemoryBuffers();
		void HandleVertexTypeChange(VertexType vertexType);
		void EmitVertex();
		void ZVertex(GLfloat x, GLfloat y, GLfloat z);

	public:
		SimpleRenderer();
		~SimpleRenderer();

		std::map<std::string, RendererProgramPtr> programs;

		void SetCurrentMtlLibId(GLuint value) { currentMtlLibId = value; }
		void SetCurrentMtlId(GLuint value) { currentMtlId = value; }
		void SetCurrentObjectId(GLuint value) { currentObjectId = value; }
		void SetCurrentGroupId(GLuint value) { currentGroupId = value; }
		void SetCurrentProgramId(GLuint value) { currentProgramId = value; }

		GLuint GetCurrentMtlLibId() const { return currentMtlLibId; }
		GLuint GetCurrentMtlId() const { return currentMtlId; }
		GLuint GetCurrentObjectId() const { return currentObjectId; }
		GLuint GetCurrentGroupId() const { return currentGroupId; }
		GLuint GetCurrentProgramId() const { return currentProgramId; }

		void SetCurrentMtlName(const std::string& name) { currentMtlName = name; }
		void SetCurrentMtlLibName(const std::string& name) { currentMtlLibName = name; }
		void SetCurrentObjectName(const std::string& name) { currentObjectName = name; }
		void SetCurrentGroupName(const std::string& name) { currentGroupName = name; }

		const std::string& GetCurrentObjectName() const { return currentObjectName; }
		const std::string& GetCurrentMtlLibName() const { return currentMtlLibName; }
		const std::string& GetCurrentMtlName() const { return currentMtlName; }
		const std::string& GetCurrentGroupName() const { return currentGroupName; }

		void ApplyIdToObjectNames(const std::string& objectName, GLuint id);
		void ApplyIdToGroupNames(const std::string& groupName, GLuint id);
		void ApplyIdToMtlLibNames(const std::string& mtllibName, GLuint id);
		void ApplyIdToMtlNames(const std::string& mtlName, GLuint id);
		void AssignUniqueGroupIds();

		void AssignMaterialIds(SimpleMaterialSystem& materials);

		void Begin(GLenum mode, bool isIndexed = false);
		void End();
		void NewObject();
		void DrawOBJ(const SimpleGeometryMesh& obj);

		void Index(IndexType index);
		void Index(std::vector<IndexType> indices);

		static const int POSITION = 0;
		static const int NORMAL = 1;
		static const int TEXCOORD = 2;
		static const int COLOR = 3;
		static const int ATTRIB1 = 4;
		static const int BONEINDEX = 5;
		static const int BONEWEIGHT = 6;
		static const int ATTRIB2 = 7;

		void VertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		void Vertex4s(GLshort x, GLshort y, GLshort z, GLshort w);
		void Normal4s(GLshort x, GLshort y, GLshort z, GLshort w);
		void TexCoord2s(GLshort s, GLshort t);
		void Color4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
		void Attrib4s(GLshort x, GLshort y, GLshort z, GLshort w);

		void Position3f(Vector3f position) { VertexAttrib4f(POSITION, position.x, position.y, position.z, 1.0); }
		void Normal3f(Vector3f normal) { VertexAttrib4f(NORMAL, normal.x, normal.y, normal.z, 0.0f); }
		void TexCoord2f(Vector2f st) { VertexAttrib4f(TEXCOORD, st.x, st.y, 0.0f, 0.0f); }
		void Color3f(Color3f color) { VertexAttrib4f(COLOR, color.r, color.g, color.b, 1.0f); }
		void Attrib14f(Vector4f v) { VertexAttrib4f(ATTRIB1, v.x, v.y, v.z, v.w); }
		void BoneIndex4f(Vector4f v) { VertexAttrib4f(BONEINDEX, v.x, v.y, v.z, v.w); }
		void BoneWeight4f(Vector4f v) { VertexAttrib4f(BONEWEIGHT, v.x, v.y, v.z, v.w); }
		void Attrib24f(Vector4f v) { VertexAttrib4f(ATTRIB2, v.x, v.y, v.z, v.w); }

		void BuildBuffers();
		void BindBuffers();
		void reset();
		void Render();
		void RenderIf(const std::string& objectName, const std::string& groupName, const std::string& mtllibName, const std::string& mtlName, bool onlyRenderZ = false);
		void RenderIf(GLuint objectId = 0, GLuint groupId = 0, GLuint mtllibId = 0, GLuint mtlId = 0, bool onlyRenderZ = false);
		void RenderZOnly();

		//void Configure(RendererContext *pRenderer);
		int vertexCount = 0;
		int triangleCount = 0;
	}; // template class SimpleRenderer

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
	extern template class SimpleRenderer<GLbyte, GL_BYTE>;
	extern template class SimpleRenderer<GLubyte, GL_UNSIGNED_BYTE>;
	extern template class SimpleRenderer<GLshort, GL_SHORT>;
	extern template class SimpleRenderer<GLushort, GL_UNSIGNED_SHORT>;
	extern template class SimpleRenderer<GLint, GL_INT>;
	extern template class SimpleRenderer<GLuint, GL_UNSIGNED_INT>;
#endif

	using SimpleRenderer_GLubyte = SimpleRenderer<GLubyte, GL_UNSIGNED_BYTE>;
	using SimpleRenderer_GLushort = SimpleRenderer<GLushort, GL_UNSIGNED_SHORT>;
	using SimpleRenderer_GLuint = SimpleRenderer<GLuint, GL_UNSIGNED_INT>;
} // namespace Fluxions

#endif
