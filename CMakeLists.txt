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
# include_directories(${PROJECT_SOURCE_DIR}/../imgui/)

# set(IMGUIDIR ${PROJECT_SOURCE_DIR}/../imgui/)

add_library(${PROJECT_NAME} STATIC
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
	src/fluxions_shader_program_locations.cpp
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
#     ${IMGUIDIR}/imgui.cpp
#     ${IMGUIDIR}/imgui_draw.cpp
#     ${IMGUIDIR}/imgui_widgets.cpp
    )

if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    message("Fluxions using precompiled headers")
    target_precompile_headers(${PROJECT_NAME} PRIVATE src/fluxions_renderer_pch.cpp)
    target_precompile_headers(${PROJECT_NAME} PRIVATE src/fluxions_ssg_pch.cpp)
endif()

find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)

# install(TARGETS fluxions DESTINATION lib)
# #[[install(TARGETS
#     ./gamelib.hpp
#     ./gamelib_base.hpp
#     ./gamelib_object.hpp
#     ./gamelib_actor.hpp
#     ./gamelib_world.hpp
#     DESTINATION include)
# ]]

