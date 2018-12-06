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
#ifndef FLUXIONS_PRECOMPILED_MODELS_HPP
#define FLUXIONS_PRECOMPILED_MODELS_HPP

#include "fluxions_opengl.hpp"

namespace Fluxions
{
	struct OBJSurface
	{
		GLenum mode;
		GLint first;
		GLsizei count;
		const char *materialName;
		const char *surfaceName;
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
}

#endif
