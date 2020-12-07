cmake_minimum_required(VERSION 3.13)
project(fluxions)

set(CMAKE_C_STANDARD            99)
set(CMAKE_C_STANDARD_REQUIRED   ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
    include_directories(/usr/local/include)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} STATIC
	src/fluxions_sphl.cpp
	src/fluxions_sphl_image_texture.cpp
	src/fluxions_sphl_sampler.cpp
	src/fluxions_ssg_ssphh.cpp
	src/fluxions_ssg_ssphh_light.cpp
	src/fluxions_ssg_ssphh_renderer_plugin.cpp
	src/fluxions_ssphh_utilities.cpp
    )

if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    message("Fluxions using precompiled headers")
    # target_precompile_headers(${PROJECT_NAME} PRIVATE src/fluxions_ssg_pch.cpp)
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

