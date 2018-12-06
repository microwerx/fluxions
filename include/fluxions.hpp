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
#ifndef FLUXIONS_HPP
#define FLUXIONS_HPP

#ifdef WIN32
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
//#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "freeglut.lib")
#endif

#include <fluxions_stdcxx.hpp>

// TODO:
// * Should be composed of the main modules
//   - fluxions_utilities.hpp --- C++ Utilities / Non-math / Non-graphics
//   - fluxions_opengl.hpp --- OpenGL related libraries (OpenGL, GLU, GLEW, FreeGLUT, Vulkan)
//   - fluxions_gte.hpp --- Geometry Transformation Engine (Matrices, Vectors, Quaternions, Spherical Harmonics, etc.)
//   - fluxions_sdl.hpp --- Link to Simple DirectMedia Layer--especially for image loading
//   - fluxions_scenegraph.hpp --- Scenegraph module (Renderer agnostic scene graph)
//   - fluxions_renderer.hpp --- Renderer module (interface to actual rendering system)
//   - fluxions_renderer_gles10 --- Renderer targeting fixed function style OpenGL (for rendering old style graphics)
//   - fluxions_renderer_gles20 -- Renderer targeting simple shader based graphics (for cross platform WebGL/GLES 2.0/Desktop systems)
//                              -- GL_ARB_ES2_compatibility
//   - fluxions_renderer_gles30 -- Renderer targeting more advanced shader based graphics (for cross platform GLES 3.0/GL 4.3 Desktop systems)
//                              -- GL_ARB_ES3_compatibility
//   - fluxions_renderer_gles32 -- Renderer targeting more advanced shader based graphics and compute (for cross platform GLES 3.2/GL 4.5 Desktop systems)
//                              -- GL_ARB_ES3_2_compatibility 
//   - fluxions_renderer_vulkan -- Renderer targeting advanced graphics and compute...
// * Future modules:
//   - XOR 2D/3D graphics -- Depends on GLES 2.0 based renderer
//                        -- OpenGL 1.1 like Begin/End paradigm
//                        -- OpenGL 1.1 like materials
//                        -- fluxions_xor.hpp

#include <hatchetfish.hpp>
#include <fluxions_utilities.hpp>
#include <fluxions_opengl.hpp>
#include <fluxions_gte.hpp>
#include <fluxions_uniform_library.hpp>
#include <fluxions_simple_renderer.hpp>
#include <fluxions_simple_scene_graph.hpp>
#include <fluxions_simple_geometry_mesh.hpp>
#include <fluxions_simple_gles30_renderer.hpp>
#include <fluxions_multiple_render_target.hpp>
#include <fluxions_corona.hpp>


namespace Fluxions
{
	extern bool debugging;
	extern const string BlankString;


	string GetPathTracerName(const string & sceneName, bool ks, int mrd, int pl);
	string GetSphlRenderName(const string & sceneName, int md);
	string GetPathTracerSphlRenderName(const string & sceneName, bool ks, int mrd, int pl, int md);
	string GetStatsName(const string & sceneName, bool ks, int mrd, int pl, int md);

}

#endif
