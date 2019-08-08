## A CPU-based experimental Path Tracer in its early stages of development.

# Introduction

Physically based rendering is a subject under active research and for which new methods and theories are being developed continuously. In order to be able to experiment and learn the most as possible about it, I've decided to write a new (still unnamed and now, hopefully, more easily expandable), experimental ray tracer in C++ from the scratch. All the code from this project is freely available under the MIT license.

As a way to record and share information about the evolution of the project, I've also created a blog at https://pbrexperiments.blogspot.com.br.

# Current Features

- Perspective (pinhole) and orthographic cameras.
- Input and configuration through Lua scripts.
- Sphere, triangle and smooth triangle primitives.
- Mesh loading through Assimp.
- SAH-based BVH rendering acceleration.
- (NEW and Experimental!) SAH-based SBVH for rendering acceleration.
- Perfectly diffuse (Lambertian) surfaces.
- Rough conductors described by a Cook-Torrance-based microfacet model.
- Smooth conductors.
- Smooth dielectrics.
- Clear coat implemented through a 2-layer BSDF (takes n internal bounces into account).
- Diffuse light sources.
- Importance sampled BSDFs.
- Radiances and reflectances stored as RGB tuples.
- Naïve (brute force) path tracing.
- (NEW) Three ray casters for normal, depth and hit count rendering.
- Path termination through fixed maximum depth or russian roulette.
- Regular, uniform and jittered box-filtered pixel samplers.
- Output in PPM (Portable Pixel Map) file format.
- Vector and matrix computations with glm.
- Single (float) precision computations.
- CPU-based.
- Parallel processing with OpenMP.
- Random number generation with the Marsenne Twister PRNG (C++11).

# Dependencies

The path tracer depends on the following third party software to be compiled:

- Lua version 5.3 or later (https://www.lua.org).
- Assimp version 4.1.0 or later (http://www.assimp.org).
- glm version 0.9.9.3 or later (http://glm.g-truc.net).

The path tracer, in its current state, can only be compiled by OpenMP-capable compilers and executed by OpenMP-capable processors (i.e. at least any current AMD or Intel microprocessor).

# Compiling and Running

I've compiled and executed this software only in Linux (i.e. Ubuntu 18.04).
These are the steps needed in order to get the program up and running:

1. Install the dependencies (Assimp, glm and Lua).
2. Download the renderer source code.
3. cd into the project directory.
4. Create the '_build' directory.
5. cd into '_build'
6. Issue the following CMake command:

    $ cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release DGLM_INCLUDE_DIRS=<glm dir>

    CMake will try to find Lua and Assimp automatically and, if it succeedes in doing that, everything will be fine and you may jump straight to step 7. However, if it cannot find them, you will have invoke CMake with some additional variables that you will have to set manually. Below you will find the instructions on setting those variables.

    CMake did not find Assimp:
    --------------------------
    Issue the cmake command defining the following two additonal variables:

    $ cmake ... -DASSIMP_INCLUDE_DIRS=<path to assimp include dir> -DASSIMP_LIBRARIES=<fully qualified assimp library name>

    CMake did not find Lua:
    -----------------------
    Issue the cmake command defining the following two additonal variables:

    $ cmake ... -DLUA_INCLUDE_DIR=<path to Lua includes> -DLUA_LIBRARIES=<fully qualified Lua library name>

    CMake did not find OpenMP:
    --------------------------
    OpenMP is not simply a library or an include, and depends on the direct support of the compiler. If your compiler supports OpenMP and, even in this case, CMake cannot properly set its variables, one alternative may be to issue the cmake command defining the following additonal variable:

    $ cmake ... -DOpenMP_CXX_FLAGS=-fopenmp

7. make
8. Done!

To render one of the accompanying sample files you may just issue the following command directly from within the _build directory:

    $ ./swpathtracer -i ../samples/<sample Lua script>

# TODOs

The implementation of this ray tracer is on going project in its early steps, and there are lots of things still to be improved and added to the renderer (this could be easily considered a lifetime software project). Among the planned features I would mention: multiple importance sampling (MIS), higher order pixel samplers, direct light sampling, bidirectioal path tracing, metropolis light transport, instancing, thin lens camera, subsurface scattering, photon mapping, light probes, spectral rendering, support for OpenEXR file format, the inclusion of higher order primitives (e.g. implicits), ... and much more.
