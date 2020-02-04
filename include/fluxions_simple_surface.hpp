#ifndef FLUXIONS_SIMPLE_SURFACE_HPP
#define FLUXIONS_SIMPLE_SURFACE_HPP

#include <fluxions_simple_vertex.hpp>

namespace Fluxions {
	/// <summary>SimpleSurface is a class representing a drawable object</summary>
	/// A Simple Surface represents a geometrical object with prescribed properties.
	/// It is consistent in the following respects:
	/// - One type of mode (POINTS, LINES, LINE_LOOP, LINE_STRIP, TRIANGLES, TRIANGLE_STRIP
	/// - Specified by a vertex array or an indexed vertex array
	/// - A surface may be rendered multiple times with different programs/uniforms but not vice versa
	struct SimpleSurface {
		VertexType vertexType = VertexType::UNDECIDED;
		bool isIndexed = false;

		// Valid for both DrawArrays and DrawElements
		GLenum mode = 0;
		GLsizei count = 0;

		// glDrawArrays
		GLint first = 0;

		// Z Only mode
		GLint zFirst = 0;
		GLsizei zCount = 0;

		// glDrawElements
		// baseIndexOffset is the index offset into the indices vector (not respective of slow or fast vertices)
		// offset is the memory buffer offset (computed by BuildBuffers)
		GLenum type = 0;
		GLsizei firstIndex = 0;
		GLsizei firstZIndex = 0;
		mutable GLsizeiptr baseZIndexBufferOffset = 0;
		mutable GLsizeiptr baseIndexBufferOffset = 0;

		// Scene graph information
		GLuint groupId = 0;
		GLuint objectId = 0;
		GLuint programId = 0;
		GLuint mtlId = 0;
		GLuint mtllibId = 0;
		GLint drawMtlId{ -1 };

		std::string mtlName;
		std::string mtllibName;
		std::string objectName;
		std::string groupName;
	};
} // namespace Fluxions

#endif
