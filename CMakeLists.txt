cmake_minimum_required(VERSION 3.13)
project(fluxions)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# This is to avoid the OpenEXR compile errors for dynamic exception specifications
# set(CMAKE_CXX_EXTENSIONS OFF)

if (APPLE)
    include_directories(/usr/local/include)
    set(STD_CXX_FS c++fs)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../imgui/)

set(IMGUIDIR ${PROJECT_SOURCE_DIR}/../imgui/)

add_library(fluxions STATIC
    src/ArHosekSkyModel.cpp
    src/damselfish.cpp
    src/damselfish_json.cpp
    src/damselfish_lexer.cpp
    src/damselfish_python.cpp
    src/damselfish_variable_list.cpp
    src/fluxions_astronomy.cpp
    src/fluxions_base.cpp
    src/fluxions_bbox.cpp
    src/fluxions_es2_renderer.cpp
    src/fluxions_file_path_info.cpp
    src/fluxions_fileio_iostream.cpp
    src/fluxions_gl1gl2_tools.cpp
    src/fluxions_gte.cpp
    src/fluxions_gte_bounding_box.cpp
    src/fluxions_gte_color3.cpp
    src/fluxions_gte_color4.cpp
    src/fluxions_gte_color_tests.cpp
    src/fluxions_gte_geometric_algebra.cpp
    src/fluxions_gte_image.cpp
    src/fluxions_gte_image_operations.cpp
    src/fluxions_gte_iostream.cpp
    src/fluxions_gte_math.cpp
    src/fluxions_gte_matrix2.cpp
    src/fluxions_gte_matrix3.cpp
    src/fluxions_gte_matrix4.cpp
    src/fluxions_gte_matrix_extra.cpp
    src/fluxions_gte_matrix_stack.cpp
    src/fluxions_gte_quaternion.cpp
    src/fluxions_gte_ray.cpp
    src/fluxions_gte_rect.cpp
    src/fluxions_gte_scalar.cpp
    src/fluxions_gte_spherical_harmonic.cpp
    src/fluxions_gte_superquadric.cpp
    src/fluxions_gte_transformation.cpp
    src/fluxions_gte_vector2.cpp
    src/fluxions_gte_vector3.cpp
    src/fluxions_gte_vector4.cpp
    src/fluxions_image_loader.cpp
    src/fluxions_opengl.cpp
    src/fluxions_pbsky.cpp
    src/fluxions_renderer_config.cpp
    src/fluxions_renderer_context.cpp
    src/fluxions_renderer_framebuffer.cpp
    src/fluxions_renderer_gles30.cpp
    src/fluxions_renderer_gles30_snapshot.cpp
    src/fluxions_renderer_gpu_texture.cpp
    src/fluxions_renderer_object.cpp
    src/fluxions_renderer_program.cpp
    src/fluxions_renderer_sampler.cpp
    src/fluxions_renderer_shader.cpp
    src/fluxions_renderer_texture_2d.cpp
    src/fluxions_renderer_texture_cube.cpp
    src/fluxions_renderer_texture_object.cpp
    src/fluxions_renderer_uniform_blocks.cpp
    src/fluxions_renderer_utilities.cpp
    src/fluxions_simple_geometry_mesh.cpp
    src/fluxions_shader_program_locations.cpp
    src/fluxions_simple_renderer.cpp
    src/fluxions_sphl.cpp
    src/fluxions_sphl_image_texture.cpp
    src/fluxions_sphl_sampler.cpp
    src/fluxions_ssg_aniso_light.cpp
    src/fluxions_ssg_camera.cpp
    src/fluxions_ssg_dirto_light.cpp
    src/fluxions_ssg_environment.cpp
    src/fluxions_ssg_material_library.cpp
    src/fluxions_ssg_materials.cpp
    src/fluxions_ssg_node.cpp
    src/fluxions_ssg_path_animation.cpp
    src/fluxions_ssg_point_light.cpp
    src/fluxions_ssg_property.cpp
    src/fluxions_ssg_reader.cpp
    src/fluxions_ssg_renderer_plugin.cpp
    src/fluxions_ssg_scene_graph.cpp
    src/fluxions_ssg_sphere.cpp
    src/fluxions_ssg_ssphh.cpp
    src/fluxions_ssg_ssphh_light.cpp
    src/fluxions_ssg_ssphh_renderer_plugin.cpp
    src/fluxions_ssg_writer.cpp
    src/fluxions_ssphh_utilities.cpp
    src/fluxions_xml.cpp
    src/hatchetfish_log.cpp
    src/hatchetfish_stopwatch.cpp
    src/unicornfish.cpp
    src/unicornfish_broker.cpp
    src/unicornfish_client.cpp
    src/unicornfish_corona_database.cpp
    src/unicornfish_corona_job.cpp
    src/unicornfish_corona_scene_file.cpp
    src/unicornfish_curl.cpp
    src/unicornfish_frame.cpp
    src/unicornfish_majordomo.cpp
    src/unicornfish_message.cpp
    src/unicornfish_socket.cpp
    src/unicornfish_worker.cpp
    src/viperfish_analog_bitset.cpp
    src/viperfish_animation_window.cpp
    src/viperfish_animpath_window.cpp
    src/viperfish_dear_imgui.cpp
    src/viperfish_gamepad.cpp
    src/viperfish_keyboard.cpp
    src/viperfish_loading_window.cpp
    src/viperfish_mouse.cpp
    src/viperfish_root_window.cpp
    src/viperfish_stats_window.cpp
    src/viperfish_widget.cpp
    src/viperfish_window.cpp
    src/viperfish_xinput.cpp
    ${IMGUIDIR}/imgui.cpp
    ${IMGUIDIR}/imgui_draw.cpp
    ${IMGUIDIR}/imgui_widgets.cpp
    )

if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    message("Using precompiled headers")
    target_precompile_headers(fluxions PRIVATE src/viperfish_pch.cpp)
    target_precompile_headers(fluxions PRIVATE src/damselfish_pch.cpp)
    target_precompile_headers(fluxions PRIVATE src/fluxions_gte_pch.cpp)
    target_precompile_headers(fluxions PRIVATE src/fluxions_pch.cpp)
    target_precompile_headers(fluxions PRIVATE src/fluxions_renderer_pch.cpp)
    target_precompile_headers(fluxions PRIVATE src/fluxions_ssg_pch.cpp)
    target_precompile_headers(fluxions PRIVATE src/unicornfish_pch.cpp)
endif()

install(TARGETS fluxions DESTINATION lib)
#[[install(TARGETS
    ./gamelib.hpp
    ./gamelib_base.hpp
    ./gamelib_object.hpp
    ./gamelib_actor.hpp
    ./gamelib_world.hpp
    DESTINATION include)
]]
