#ifndef FLUXIONS_SIMPLE_VERTEX_HPP
#define FLUXIONS_SIMPLE_VERTEX_HPP

#include <fluxions_gte_color_math.hpp>
#include <fluxions_opengl.hpp>

namespace Fluxions {
	enum class VertexType {
		UNDECIDED,
		FAST_VERTEX,
		SLOW_VERTEX
	};

	// 12 bytes/vertex
	struct SimpleZVertex {
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
	struct SimpleMediumVertex {
		Vector3f aPosition;
		Vector3f aNormal;
		Vector2f aTexCoord;
	};

	// 32 bytes/vertex
	struct SimpleFastVertex {
		GLshort position[4] = { 0, 0, 0, 0 };
		GLshort normal[4] = { 0, 0, 0, 0 };
		GLshort texCoord[2] = { 0, 0 };
		GLubyte color[4] = { 0, 0, 0, 0 };
		GLshort attrib[4] = { 0, 0, 0, 0 };
	};

	// 64 bytes/vertex
	struct SimpleVertex {
		Vector3f aPosition;
		Vector3f aNormal;
		Vector2f aTexCoord;
		Color4f aColor;
		Vector4f aAttrib;
	};

	// new type 64 bytes/vertex
	struct SimpleBoneVertex {
		Vector3f aPosition;   // 12 bytes
		Vector3f aNormal;     // 12 bytes
		Vector2f aTexCoord;   //  8 bytes
		Color4ub aColor;      //  4 bytes
		Vector4s aAttrib1;     //  8 bytes
		Vector4ub aBoneIndex; //  4 bytes
		Vector4f aBoneWeight; // 16 bytes
	};

	// 128 bytes/vertex
	struct SimpleSlowVertex {
		//static constexpr int aPosition = 0;
		//static constexpr int aNormal = 1;
		//static constexpr int aTexCoord = 2;
		//static constexpr int aColor = 3;
		//static constexpr int aAttrib1 = 4;
		//static constexpr int aBoneIndex = 5;
		//static constexpr int aBoneWeight = 6;
		//static constexpr int aAttrib2 = 7;

		GLfloat attrib[8][4] = { 0.0f };

		SimpleSlowVertex() {}
		~SimpleSlowVertex() {}
	};
} // namespace Fluxions

#endif
