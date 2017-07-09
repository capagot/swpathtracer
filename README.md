## A CPU-based experimental Path Tracer in its early stages of development.

# Introduction

Physically based rendering is a quite new subject (assuming that I was born before the advent of rendering equation) that is under active research and for which new methods and theories are being developed continuously. In order to be able to experiment and learn the most as possible about it, I've decided to write a new (still unnamed and now, hopefully, more easily expandable), experimental ray tracer in C++11 from the scratch. All the code from this project is freely available for download in this repository under the MIT license.

As a way to record and share information about the evolution of the project, I've also created a blog at https://pbrexperiments.blogspot.com.br.

# Current Features

- Perspective (pinhole) and orthographic cameras.
- Input and configuration through Lua scripts.
- Sphere and triangle primitives.
- Mesh loading through Assimp.
- SAH-based BVH for rendering acceleration.
- Perfectly diffuse (Lambertian) surfaces.
- Rough conductors described by a Cook-Torrance-based microfacet model.
- Smooth conductors.
- Smooth dielectrics.
- Clear coat implemented through a 2-layer BSDF (takes TIR into account).
- Diffuse light sources.
- Importance sampled BSDFs.
- RGB spectrum.
- Naïve (brute force) path tracing, with a maximum prescribed path depth.
- Regular, uniform and jittered box-filtered pixel samplers.
- Output in PPM (Portable Pixel Map) file format.
- Vector and matrix computations with glm.
- Double precision computations.
- CPU-based.
- Parallel processing with OpenMP.
- Random number generation with the Marsenne Twister PRNG (C++11).

# Dependencies

The path tracer depends on the following third party software to be compiled:

- Lua version 5.2 or later (https://www.lua.org).
- Assimp version 3.3.1 or later (http://www.assimp.org).
- glm version 0.9.7.0 or later (http://glm.g-truc.net).

The path tracer, in its current state, can only be compiled by OpenMP-capable compilers and executed by OpenMP-capable processors (i.e. at least any current AMD or Intel microprocessor).

# Compiling and Running

I've compiled and executed this software only in Linux (Ubuntu 14.04).
These are the steps needed in order to compile the program:

1. Install the dependencies (Assimp, glm and Lua).
2. Download the source code together with the Makefile.
3. Open the Makefile with a text editor (e.g. vim, gedit, etc.).
4. Configure the variables CPPFLAGS and LDFLAGS in order to reflect your Assimp/glm/Lua installations.
5. Save the Makefile.
6. Call 'make DEBUG=0' at the project root directory.
7. Done

To render one of the sample files, just issue the following command from the project root directory: 

    $ ./swpathtracer -i samples/'sample Lua file'

# TODOs

The development of this ray tracer is just in the beggining, and there are lots and lots of things to be improved and added to the renderer (this coule bd easily considered a lifetime software project... :). Among the planned features, I would mention: multiple importance sampling, higher order pixel samplers, direct light sampling, bidirectioal path tracing, metropolis light transport, instancing, thin lens camera, russian roulette for ray path termination, subsurface scattering, spectral rendering, support for OpenEXR file format... and much more.

