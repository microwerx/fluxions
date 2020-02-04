#ifndef FLUXIONS_PRECOMPILED_MODELS_HPP
#define FLUXIONS_PRECOMPILED_MODELS_HPP

#include <fluxions_opengl.hpp>

namespace Fluxions {
	struct OBJSurface {
		GLenum mode;
		GLint first;
		GLsizei count;
		const char* materialName;
		const char* surfaceName;
	};

	extern const int OBJ_Sponza_IndicesSize;
	extern const int OBJ_Sponza_VerticesSize;
	extern const int OBJ_Sponza_SurfacesSize;
	extern const int OBJ_Sponza_Indices[];
	extern const float OBJ_Sponza_Vertices[];
	extern const OBJSurface OBJ_Sponza_Surfaces[];

	extern const int OBJ_Teapot_IndicesSize;
	extern const int OBJ_Teapot_VerticesSize;
	extern const int OBJ_Teapot_SurfacesSize;
	extern const int OBJ_Teapot_Indices[];
	extern const float OBJ_Teapot_Vertices[];
	extern const OBJSurface OBJ_Teapot_Surfaces[];

	extern const int OBJ_TorusKnot_IndicesSize;
	extern const int OBJ_TorusKnot_VerticesSize;
	extern const int OBJ_TorusKnot_SurfacesSize;
	extern const int OBJ_TorusKnot_Indices[];
	extern const float OBJ_TorusKnot_Vertices[];
	extern const OBJSurface OBJ_TorusKnot_Surfaces[];

	//void CreatePrecompiledModels();
} // namespace Fluxions

#endif
