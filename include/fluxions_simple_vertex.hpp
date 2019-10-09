#ifndef FLUXIONS_SIMPLE_VERTEX_HPP
#define FLUXIONS_SIMPLE_VERTEX_HPP

#include <fluxions_gte_color_math.hpp>
#include <fluxions_opengl.hpp>

namespace Fluxions
{
	enum class VertexType
	{
		UNDECIDED,
		FAST_VERTEX,
		SLOW_VERTEX
	};

	// 12 bytes/vertex
	struct SimpleZVertex
	{
		GLfloat position[3];

		SimpleZVertex() {
			position[0] = 0.0f;
			position[1] = 0.0f;
			position[2] = 0.0f;
		}

		SimpleZVertex(GLfloat x, GLfloat y, GLfloat z) {
			position[0] = x;
			position[1] = y;
			position[2] = z;
		}
	};

	// 32 bytes/vertex
	struct SimpleFastVertex
	{
		GLshort position[4] = { 0, 0, 0, 0 };
		GLshort normal[4] = { 0, 0, 0, 0 };
		GLshort texCoord[2] = { 0, 0 };
		GLubyte color[4] = { 0, 0, 0, 0 };
		GLshort attrib[4] = { 0, 0, 0, 0 };
	};

	// new type 48 bytes/vertex
	struct SimpleVertex
	{
		Vector3f position;
		Vector3f normal;
		Vector2f texcoord;
		Color4f color;
	};

	// 128 bytes/vertex
	struct SimpleSlowVertex
	{
		GLfloat attrib[8][4] = { 0.0f };
	};
} // namespace Fluxions

#endif