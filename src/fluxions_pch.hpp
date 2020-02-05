#ifndef FLUXIONS_PCH_HPP
#define FLUXIONS_PCH_HPP

#define _USE_MATH_DEFINES

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
#define FLUXIONS_NO_EXTERN_TEMPLATES
#endif

//////////////////////////////////////////////////////////////////////
// C + +   S t a n d a r d   T e m p l a t e   L i b r a r y /////////
//////////////////////////////////////////////////////////////////////

#include <fluxions_base.hpp>

/*
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
*/

//////////////////////////////////////////////////////////////////////
// C   S t a n d a r d   L i b r a r y ///////////////////////////////
//////////////////////////////////////////////////////////////////////

/*
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cinttypes>
*/

//////////////////////////////////////////////////////////////////////
// E X T E R N A L   D E P E N D E N C I E S /////////////////////////
//////////////////////////////////////////////////////////////////////

//#include <fluxions_stdcxx.hpp>
//#include <fluxions_gte.hpp>
//#include <fluxions_file_path_info.hpp>

#ifndef FLUXIONS_NO_SDL
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
#endif

//#include <sodium.h>
//#include <curl/curl.h>
//#include <Python.h>
//#include <GL/glew.h>

#ifdef FLUXIONS_USE_FREEGLUT
//#include <GL/freeglut.h>
#endif

#ifndef FLUXIONS_NO_OPENEXR
#ifdef _WIN32
#define OPENEXR_DLL
#endif
//#include <OpenEXR/ImfRgbaFile.h>
#endif

//////////////////////////////////////////////////////////////////////
// INTERNAL HEADERS //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*
#include <fluxions.hpp>
#include <fluxions_astronomy.hpp>
#include <fluxions_bbox.hpp>
#include <fluxions_config.hpp>
#include <fluxions_es2_renderer.hpp>
#include <fluxions_es2_state_info.hpp>
#include <fluxions_multiple_render_target.hpp>
#include <fluxions_opengl.hpp>
#include <fluxions_pbsky.hpp>
#include <fluxions_precompiled_models.hpp>
#include <fluxions_renderer.hpp>
#include <fluxions_resource_manager.hpp>
#include <fluxions_ssg_gles30_renderer.hpp>
#include <fluxions_ssg_materials.hpp>
#include <fluxions_ssg_property.hpp>
#include <fluxions_ssg_renderer.hpp>
#include <fluxions_ssg_scene_graph.hpp>
#include <fluxions_ssg_texture.hpp>
#include <fluxions_ssphh.hpp>
#include <fluxions_stdcxx.hpp>
#include <fluxions_types.hpp>
#include <fluxions_uniform_library.hpp>
#include <fluxions_utilities.hpp>
#include <fluxions_xml.hpp>
#include <fluxions_draw_gl1gl2.hpp>
*/

//#include <fluxions_resource_manager.hpp>

/*
#include <ArHosekSkyModel.h>

#include <hatchetfish.hpp>

#include <damselfish.hpp>
#include <damselfish_evaluator.hpp>
#include <damselfish_lexer.hpp>
#include <damselfish_python.hpp>
#include <damselfish_syntax_semantic_analyzer.hpp>

#include <unicornfish_base.hpp>
#include <unicornfish_broker.hpp>
#include <unicornfish_client.hpp>
#include <unicornfish_curl.hpp>
#include <unicornfish_frame.hpp>
#include <unicornfish_majordomo.hpp>
#include <unicornfish_message.hpp>
#include <unicornfish_socket.hpp>
#include <unicornfish_worker.hpp>
#include <unicornfish_corona.hpp>
#include <unicornfish_corona_job.hpp>
#include <unicornfish_corona_scene_file.hpp>

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

#include <viperfish.hpp>
#include <viperfish_gamepad.hpp>
#include <viperfish_keyboard.hpp>
#include <viperfish_mouse.hpp>
#include <viperfish_stopwatch.hpp>
#include <viperfish_utilities.hpp>
#include <viperfish_widget.hpp>
#include <viperfish_xinput.hpp>
*/

constexpr int MAX_RUN_MESSAGES = 0;

#endif