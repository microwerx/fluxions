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

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef FLUXIONS_STDAFX_H
#define FLUXIONS_STDAFX_H

#define NOMINMAX
#define _USE_MATH_DEFINES
#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
#define FLUXIONS_NO_EXTERN_TEMPLATES
#endif

#ifdef WIN32
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <future>
#include <numeric>
#include <functional>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

#ifndef FLUXIONS_NO_SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif
#include <sodium.h>
#include <curl/curl.h>
#include <Python.h>

#include <GL/glew.h>
#ifndef FLUXIONS_NO_FREEGLUT
#include <GL/freeglut.h>
#endif

#ifndef FLUXIONS_NO_OPENEXR
#ifdef WIN32
#define OPENEXR_DLL
#endif
#include <OpenEXR/ImfRgbaFile.h>
#endif

// TODO: reference additional headers your program requires here
#include <ArHosekSkyModel.h>
#include <fluxions.hpp>
#include <fluxions_astronomy.hpp>
#include <fluxions_bbox.hpp>
#include <fluxions_config.hpp>
#include <fluxions_corona.hpp>
#include <fluxions_corona_job.hpp>
#include <fluxions_corona_scene_file.hpp>
#include <fluxions_es2_renderer.hpp>
#include <fluxions_es2_state_info.hpp>
#include <fluxions_fileio.hpp>
#include <fluxions_gte.hpp>
#include <fluxions_gte_bounding_box.hpp>
#include <fluxions_gte_color3.hpp>
#include <fluxions_gte_color4.hpp>
#include <fluxions_gte_geometric_algebra.hpp>
#include <fluxions_gte_image.hpp>
#include <fluxions_gte_iostream.hpp>
#include <fluxions_gte_math.hpp>
#include <fluxions_gte_matrix2.hpp>
#include <fluxions_gte_matrix3.hpp>
#include <fluxions_gte_matrix4.hpp>
#include <fluxions_gte_matrix_extra.hpp>
#include <fluxions_gte_matrix_stack.hpp>
#include <fluxions_gte_plane.hpp>
#include <fluxions_gte_quaternion.hpp>
#include <fluxions_gte_ray.hpp>
#include <fluxions_gte_spherical_harmonic.hpp>
#include <fluxions_gte_vector2.hpp>
#include <fluxions_gte_vector3.hpp>
#include <fluxions_gte_vector4.hpp>
#include <fluxions_gte_image.hpp>
#include <fluxions_multiple_render_target.hpp>
#include <fluxions_opengl.hpp>
#include <fluxions_pbsky.hpp>
#include <fluxions_precompiled_models.hpp>
#include <fluxions_renderer.hpp>
#include <fluxions_resource_manager.hpp>
#include <fluxions_simple_gles30_renderer.hpp>
#include <fluxions_simple_materials.hpp>
#include <fluxions_simple_property.hpp>
#include <fluxions_simple_renderer.hpp>
#include <fluxions_simple_scene_graph.hpp>
#include <fluxions_simple_texture.hpp>
#include <fluxions_ssphh.hpp>
#include <fluxions_stdcxx.hpp>
#include <fluxions_types.hpp>
#include <fluxions_uniform_library.hpp>
#include <fluxions_utilities.hpp>
#include <fluxions_xml.hpp>
#include <glut_extensions.hpp>
#include <hatchetfish.hpp>
#include <kasl.hpp>
#include <kasl_evaluator.hpp>
#include <kasl_lexer.hpp>
#include <kasl_python.hpp>
#include <kasl_syntax_semantic_analyzer.hpp>
#include <unicornfish.hpp>
#include <unicornfish_broker.hpp>
#include <unicornfish_client.hpp>
#include <unicornfish_curl.hpp>
#include <unicornfish_frame.hpp>
#include <unicornfish_majordomo.hpp>
#include <unicornfish_message.hpp>
#include <unicornfish_socket.hpp>
#include <unicornfish_worker.hpp>
#include <viperfish.hpp>
#include <viperfish_gamepad.hpp>
#include <viperfish_keyboard.hpp>
#include <viperfish_mouse.hpp>
#include <viperfish_stopwatch.hpp>
#include <viperfish_utilities.hpp>
#include <viperfish_widget.hpp>
#include <viperfish_xinput.hpp>

#endif
