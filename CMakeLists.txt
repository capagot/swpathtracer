cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(swpathtracer LANGUAGES CXX)

###############################################################################
# Select between Release and Debug
###############################################################################

if (CMAKE_BUILD_TYPE MATCHES Debug)
  add_definitions(-DDEBUG)
  message("Building DEBUG version")
else(CMAKE_BUILD_TYPE MATCHES Release)
  add_definitions(-DNDEBUG)
  message("Building RELEASE version")
endif()

if (DEFINE_ENABLE_FEEXCEPTION)
  add_definitions(-DENABLE_FEEXCEPTION)
  message("Catch SIGFPE enabled")
else()
  message("Catch SIGFPE disabled")
endif()

###############################################################################
# Find packages: OpenMP, Lua 5.3, Assimp
###############################################################################

find_package(OpenMP REQUIRED)
find_package(Lua 5.3 REQUIRED)
find_package(assimp REQUIRED)

###############################################################################
# Enforce c++14 standard
###############################################################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic ${OpenMP_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 14)          # enforces c++14
set(CMAKE_CXX_STANDARD_REQUIRED ON) # does not fallback if the compiler does not cupport c++14
set(CMAKE_CXX_EXTENSIONS OFF)       # sets -std=c++14 instead of -std=g++14

###############################################################################
# Prepares the application
###############################################################################

add_executable(swpathtracer
               ${PROJECT_SOURCE_DIR}/acceleration/aabb.cpp
               ${PROJECT_SOURCE_DIR}/acceleration/no_accel_structure.cpp
               ${PROJECT_SOURCE_DIR}/acceleration/sbvh.cpp
               ${PROJECT_SOURCE_DIR}/bsdf_samplers/bsdf_sampler.cpp
               ${PROJECT_SOURCE_DIR}/bsdf_samplers/cook_torrance_bsdf_sampler.cpp
               ${PROJECT_SOURCE_DIR}/bsdf_samplers/cosine_bsdf_sampler.cpp
               ${PROJECT_SOURCE_DIR}/bsdf_samplers/refraction_bsdf_sampler.cpp
               ${PROJECT_SOURCE_DIR}/bsdf_samplers/smooth_specular_bsdf_sampler.cpp
               ${PROJECT_SOURCE_DIR}/bsdf_samplers/uniform_bsdf_sampler.cpp
               ${PROJECT_SOURCE_DIR}/cameras/camera.cpp
               ${PROJECT_SOURCE_DIR}/cameras/orthographic_camera.cpp
               ${PROJECT_SOURCE_DIR}/cameras/pinhole_camera.cpp
               ${PROJECT_SOURCE_DIR}/core/image_buffer.cpp
               ${PROJECT_SOURCE_DIR}/core/main.cpp
               ${PROJECT_SOURCE_DIR}/core/onb.cpp
               ${PROJECT_SOURCE_DIR}/core/ray.cpp
               ${PROJECT_SOURCE_DIR}/core/scene.cpp
               ${PROJECT_SOURCE_DIR}/core/swpathtracer_app.cpp
               ${PROJECT_SOURCE_DIR}/integrators/depth_raycaster.cpp
               ${PROJECT_SOURCE_DIR}/integrators/integrator.cpp
               ${PROJECT_SOURCE_DIR}/integrators/int_test_count_raycaster.cpp
               ${PROJECT_SOURCE_DIR}/integrators/normal_raycaster.cpp
               ${PROJECT_SOURCE_DIR}/integrators/pathtracer.cpp
               ${PROJECT_SOURCE_DIR}/lua_bind/lua_bind.cpp
               ${PROJECT_SOURCE_DIR}/materials/bsdf.cpp
               ${PROJECT_SOURCE_DIR}/materials/cook_torrance.cpp
               ${PROJECT_SOURCE_DIR}/materials/fresnel.cpp
               ${PROJECT_SOURCE_DIR}/materials/fresnel_schlick.cpp
               ${PROJECT_SOURCE_DIR}/materials/lambertian.cpp
               ${PROJECT_SOURCE_DIR}/materials/layered_bsdf.cpp
               ${PROJECT_SOURCE_DIR}/materials/material.cpp
               ${PROJECT_SOURCE_DIR}/materials/smooth_conductor.cpp
               ${PROJECT_SOURCE_DIR}/materials/smooth_dielectric.cpp
               ${PROJECT_SOURCE_DIR}/pixel_samplers/jittered_pixel_sampler.cpp
               ${PROJECT_SOURCE_DIR}/pixel_samplers/pixel_sampler.cpp
               ${PROJECT_SOURCE_DIR}/pixel_samplers/regular_pixel_sampler.cpp
               ${PROJECT_SOURCE_DIR}/pixel_samplers/uniform_pixel_sampler.cpp
               ${PROJECT_SOURCE_DIR}/primitives/primitive.cpp
               ${PROJECT_SOURCE_DIR}/primitives/smooth_triangle.cpp
               ${PROJECT_SOURCE_DIR}/primitives/sphere.cpp
               ${PROJECT_SOURCE_DIR}/primitives/triangle.cpp)

target_include_directories(swpathtracer
                           PUBLIC
                           ${PROJECT_SOURCE_DIR}/acceleration
                           ${PROJECT_SOURCE_DIR}/bsdf_samplers
                           ${PROJECT_SOURCE_DIR}/cameras
                           ${PROJECT_SOURCE_DIR}/core
                           ${PROJECT_SOURCE_DIR}/integrators
                           ${PROJECT_SOURCE_DIR}/lua_bind
                           ${PROJECT_SOURCE_DIR}/materials
                           ${PROJECT_SOURCE_DIR}/pixel_samplers
                           ${PROJECT_SOURCE_DIR}/primitives
                           ${LUA_INCLUDE_DIR}
                           ${ASSIMP_INCLUDE_DIRS})

target_include_directories(swpathtracer
                           SYSTEM PUBLIC
                           ${GLM_INCLUDE_DIRS})

target_link_libraries(swpathtracer
                      ${ASSIMP_LIBRARIES}
                      ${LUA_LIBRARIES})
