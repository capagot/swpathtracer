# SWPathtracer Renderer

An experimental ray tracer in its early stages of development.

## Introduction

SWPathtracer is a physically based renderer, based on the Path Tracing algorithm, that has been written from scratch in C++ for educational purposes. Although some acceleration techniques have already been (and may be in the future) included into the project, the main goal is not performance, but the construction of a high quality, full featured, CPU-based ray tracer through readable, modular, extensible and easy to understand code.

![Happy Buddha](https://github.com/capagot/swpathtracer/blob/master/samples/happy_buddha.png)

Happy Buddha rendering specifications:
* 800x600 pixels.
* Forward Path Tracing.
* 4096 samples per pixel with box-filtering.
* BSDF importance sampling.
* ~1M triangles.
* SAH-bases BVH (~787K nodes).
* Buddha material: 2 layer BSDF (dielectric + rough conductor).
* Rendering time: ~1h24min on a Intel i5 running Ubuntu 18.04.

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
- Clear coat (slow because it fully simulates the ligh behavior between layers).
- Diffuse light sources.
- Importance sampled BSDFs.
- Radiances and reflectances stored as RGB tuples.
- Naïve (brute force) path tracing.
- **NEW:** Three ray casters for normal, depth and hit test/count rendering.
- Path termination through fixed maximum depth or russian roulette (currently it does not support splitting).
- Regular, uniform and jittered box-filtered pixel samplers.
- Output in PPM (Portable Pixel Map) file format.
- Vector and matrix computations with glm.
- Single (float) precision computations.
- Multithreading through OpenMP.
- Random number generation with the Marsenne Twister (native C++ PRNG).

## Dependencies

SWPathtracer depends on the following third party software:

- Lua version 5.3 or later (https://www.lua.org).
- Assimp version 4.1.0 or later (http://www.assimp.org).
- glm version 0.9.9.3 or later (http://glm.g-truc.net).

Additionally, the ray tracer depends on [OpenMP](https://www.openmp.org) support.

## Compiling and Running

I've compiled and executed this software only in Linux (i.e. Ubuntu 18.04). These are the steps needed in order to get the program up and running:

1. Install the dependencies (Assimp, glm and Lua).
2. Download the renderer source code.
3. ```cd``` into the ```project``` directory.
4. Create the ```_build``` directory.
5. cd into ```_build```
6. Issue the following CMake command:

```$ cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release DGLM_INCLUDE_DIRS=<glm dir>```

7. ```$ make```
8. Done!

To render one of the accompanying sample files you may just issue the following command directly from within the ```_build``` directory:

```$ ./swpathtracer -i ../samples/<sample Lua script>```

## If CMake Fails to find Lua and Assimp

If CMake fails to find Assimp or Lua, you will have to invoke CMake and set variables manually. Below you will find the instructions on setting those variables.

### CMake did not find Assimp:

Issue the cmake command defining the following two additonal variables:

```$ cmake ... -DASSIMP_INCLUDE_DIRS=<path to assimp include dir> -DASSIMP_LIBRARIES=<fully qualified assimp library name>```

### CMake did not find Lua:

Issue the cmake command defining the following two additonal variables:

```$ cmake ... -DLUA_INCLUDE_DIR=<path to Lua includes> -DLUA_LIBRARIES=<fully qualified Lua library name>```

### CMake did not find OpenMP:

OpenMP is not simply a library or an include, it depends on the direct support of the compiler. If your compiler supports OpenMP and, even in this case, CMake cannot properly set its variables, one may try to issue the cmake command with the following additonal variable:

```$ cmake ... -DOpenMP_CXX_FLAGS=-fopenmp```

## TODOs

The implementation of this ray tracer is on going project in its early steps, and there are lots of things still to be improved and added to the renderer (this could be easily considered a lifetime software project). Among the planned features I would mention: multiple importance sampling (MIS), higher order pixel samplers, direct light sampling, bidirectioal path tracing, metropolis light transport, VCM, instancing, thin lens camera, CSG, subsurface scattering, photon mapping, light probes, spectral rendering, support for OpenEXR, Disney "principled" BRDFs, higher order primitives (*e.g.* polynomials), and more.
