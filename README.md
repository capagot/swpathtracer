# ACME Renderer

An experimental ray tracer in its early stages of development.

## Introduction

Physically based rendering is a subject under active research and for which new techniques are being developed continuously. In order to be able to experiment and learn the most as possible about it, I've decided to write an experimental ray tracer in C++ from the scratch. Although some acceleration techniques have already been (and may be in the future) included into the project, the main goal is not performance, but the construction of a full featured, CPU-based ray tracer through readable, modular, extensible and easy to understand code.
I am also trying to, as time permits, record its development at https://pbrexperiments.blogspot.com.br. 

## Current Features

- Perspective (pinhole) and orthographic cameras.
- Input and configuration through Lua scripts.
- Sphere, triangle and smooth triangle primitives.
- Mesh loading through Assimp.
- SAH-based BVH rendering acceleration.
- Perfectly diffuse (Lambertian) surfaces.
- Rough conductors described by a Cook-Torrance-based microfacet model.
- Smooth conductors.
- Smooth dielectrics.
- Multi-layer BSDF support (extremmly slow because it fully simulates internal bounces).
- Diffuse light sources.
- Importance sampled BSDFs.
- Radiances and reflectances stored as RGB tuples.
- Naïve (brute force) path tracing.
- *NEW:* Three ray casters for normal, depth and hit test/count rendering.
- Path termination through fixed maximum depth or russian roulette (no splitting).
- Regular, uniform and jittered box-filtered pixel samplers.
- Output in PPM (Portable Pixel Map) file format.
- Vector and matrix computations with glm.
- Single (float) precision computations.
- Multithreaded through OpenMP.
- Random number generation with the Marsenne Twister (native C++ PRNG).

## Dependencies

The ray tracer currently depends on the following third party software:

- Lua version 5.3 or later (https://www.lua.org).
- Assimp version 4.1.0 or later (http://www.assimp.org).
- glm version 0.9.9.3 or later (http://glm.g-truc.net).

Additionally, in its current state, the ray tracer needs OpenMP support. 

## Compiling and Running

I've compiled and executed this software only in Linux (i.e. Ubuntu 18.04). These are the steps needed in order to get the program up and running:

1. Install the dependencies (Assimp, glm and Lua).
2. Download the renderer source code.
3. cd into the project directory.
4. Create the '_build' directory.
5. cd into '_build'
6. Issue the following CMake command:

    '$ cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release DGLM_INCLUDE_DIRS=<glm dir>'

    CMake will try to find Lua and Assimp automatically and, if it succeedes in doing that, everything will be fine and you may jump straight to step 7. However, if it cannot find them, you will have invoke CMake with some additional variables that you will have to set manually. Below you will find the instructions on setting those variables.

### CMake did not find Assimp:

    Issue the cmake command defining the following two additonal variables:

    '$ cmake ... -DASSIMP_INCLUDE_DIRS=<path to assimp include dir> -DASSIMP_LIBRARIES=<fully qualified assimp library name>'

### CMake did not find Lua:

    Issue the cmake command defining the following two additonal variables:

    '$ cmake ... -DLUA_INCLUDE_DIR=<path to Lua includes> -DLUA_LIBRARIES=<fully qualified Lua library name>'

### CMake did not find OpenMP:

    OpenMP is not simply a library or an include, it depends on the direct support of the compiler. If your compiler supports OpenMP and, even in this case, CMake cannot properly set its variables, one alternative may be to issue the cmake command defining the following additonal variable:

    '$ cmake ... -DOpenMP_CXX_FLAGS=-fopenmp'

7. make
8. Done!

To render one of the accompanying sample files you may just issue the following command directly from within the '_build' directory:

    '$ ./swpathtracer -i ../samples/<sample Lua script>'

## TODOs

The implementation of this ray tracer is on going project in its early steps, and there are lots of things still to be improved and added to the renderer (this could be easily considered a lifetime software project). Among the planned features I would mention: multiple importance sampling (MIS), higher order pixel samplers, direct light sampling, bidirectioal path tracing, metropolis light transport, instancing, thin lens camera, subsurface scattering, photon mapping, light probes, spectral rendering, support for OpenEXR file format, the inclusion of higher order primitives (e.g. implicits), ... and much more.
