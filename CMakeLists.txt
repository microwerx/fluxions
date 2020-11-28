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
	src/fluxions_sphl.cpp
	src/fluxions_sphl_image_texture.cpp
	src/fluxions_sphl_sampler.cpp
	src/fluxions_ssg_ssphh.cpp
	src/fluxions_ssg_ssphh_light.cpp
	src/fluxions_ssg_ssphh_renderer_plugin.cpp
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

