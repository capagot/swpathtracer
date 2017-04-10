package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
local image_width = 512
local image_height = 512

Camera = camera{
    type     = "perspective",
    position = { 0, 0, 8 },
    up       = { 0, 1, 0 }, 
    look_at  = { 0, 0, 0 },
    aspect   = image_width / image_height, 
    fov      = 40
}

s = sampler{
    type = "jittered",
    spp = 100
}

b = buffer{
    hres = image_width,
    vres = image_height
}

g = globals{
    background_color = { 0, 0, 0 },
    max_path_depth = 5,    
    output_filename = "monkey_in_a_box.ppm",
    acceleration_data_structure = "bvh-sah"
}

local a = 2

local white = { 0.75, 0.75, 0.75 }
local red = { 0.75, 0.25, 0.25 }
local blue = { 0.25, 0.25, 0.75 }

-- back wall
t1 = triangle{ 
    vertices = { 
        { -a, -a, -a },
        {  a, -a, -a },
        {  a,  a, -a } 
    },
    material = {
                 brdf = lambertian{ kd = { 0.75, 0.75, 0.75 },
                                    surface_sampler = "importance" 
                 },
 
                 --brdf    = cook_torrance{ 
                 --                          m = 0.2, -- beckmann
                 --                          fresnel_type = "schlick-normal-reflectance", 
                 --                          reflectance_at_normal_incidence = { 0.95, 0.64, 0.54 },
                 --                          surface_sampler = "importance"
                 --                        },

                 --brdf     = lambertian{ kd = white },
                 emission = { 0, 0, 0 }
               }
}

t2 = triangle{ 
    vertices = { 
        {  a,  a, -a },
        { -a,  a, -a },
        { -a, -a, -a } 
    }, 
    material = {
                 brdf = lambertian{ kd = { 0.75, 0.75, 0.75 },
                                    surface_sampler = "importance" 
                 },
 
                 --brdf    = cook_torrance{ 
                 --                          m = 0.2, -- beckmann
                 --                          fresnel_type = "schlick-normal-reflectance", 
                 --                          reflectance_at_normal_incidence = { 0.95, 0.64, 0.54 },
                 --                          surface_sampler = "importance"
                 --                       },

                 --brdf     = lambertian{ kd = white },
                 emission = { 0, 0, 0 }
               }
}

-- top
t3 = triangle{ 
    vertices = { 
        { -a, a, -a },
        {  a, a, -a },
        {  a, a,  a } 
    },
    material = {
                 brdf     = lambertian{ kd = white,
                                        surface_sampler = "importance" 
                                      },
                 emission = { 0, 0, 0 }
               }
}

t4 = triangle{ 
    vertices = { 
        { -a, a, -a },
        {  a, a,  a },
        { -a, a,  a } 
    }, 
    material = {
                 brdf     = lambertian{ kd = white,
                                        surface_sampler = "importance" 
                                      },
                 emission = { 0, 0, 0 }
               }
}

-- bottom
t5 = triangle{ 
    vertices = { 
        {  a, -a, -a },
        { -a, -a, -a },
        {  a, -a,  a } 
    },
    material = {
                 brdf     = lambertian{ kd = white,
                                        surface_sampler = "importance" 
                                      },
                 emission = { 0, 0, 0 }
               }
}

t6 = triangle{ 
    vertices = { 
        {  a, -a,  a },
        { -a, -a, -a },
        { -a, -a,  a } 
    }, 
    material = {
                 brdf     = lambertian{ kd = white,
                                        surface_sampler = "importance" 
                                      },
                 emission = { 0, 0, 0 }
               }
}

-- left wall ( red )
t7 = triangle{ 
    vertices = { 
        { -a,  a, -a },
        { -a,  a,  a },
        { -a, -a, -a } 
    },
    material = {
                 brdf     = lambertian{ kd = red,
                                        surface_sampler = "importance" 
                                      },
                 emission = { 0, 0, 0 }
               }
}

t8 = triangle{ 
    vertices = { 
        { -a, -a, -a },
        { -a,  a,  a },
        { -a, -a,  a } 
    },
    material = {
                 brdf     = lambertian{ kd = red,
                                        surface_sampler = "importance" 
                                      },
                 emission = { 0, 0, 0 }
               }
}

-- right wall (blue)
t9 = triangle{ 
    vertices = { 
        { a,  a,  a },
        { a,  a, -a },
        { a, -a, -a } 
    }, 
    material = {
                 brdf     = lambertian{ kd = blue,
                                        surface_sampler = "importance" 
                                      },
                 emission = { 0, 0, 0 }
               }
}

t10 = triangle{ 
    vertices = { 
        { a,  a,  a },
        { a, -a, -a },
        { a, -a,  a } 
    }, 
    material = {
                 brdf     = lambertian{ kd = blue,
                                        surface_sampler = "importance" 
                                      },
                 emission = { 0, 0, 0 }
               }
}

-- light source
local offset = 0.001

t11 = triangle{ 
    vertices = { 
        {  0.5, a - offset, -0.5 },
        { -0.5, a - offset, -0.5 },
        {  0.5, a - offset,  0.5 } 
    },
    material = {
                 brdf     = lambertian{ kd = { 0, 0, 0 },
                                        surface_sampler = "importance" 
                                      },
                 emission = { 17, 17, 17 }
               }
}

t12 = triangle{ 
    vertices = { 
        { -0.5, a - offset, -0.5 },
        {  0.5, a - offset,  0.5 },
        { -0.5, a - offset,  0.5 } 
    }, 
    material = {
                 brdf     = lambertian{ kd = { 0, 0, 0 },
                                        surface_sampler = "importance" 
                                      },
                 emission = { 17, 17, 17 }
               }
}


s1 = sphere{
    center   = { -1, -1.25, -1 },
    radius   = 0.75,
    material = {
                 --brdf = lambertian{ kd = { 0.75, 0.75, 0.75 },
                 --                   surface_sampler = "importance" 
                 --},
                 
                 brdf = smooth_specular_reflection{
                                           fresnel_type = "schlick-normal-reflectance", 
                                           reflectance_at_normal_incidence = { 0.95, 0.64, 0.54 }
                        },
                 emission = { 0, 0, 0 }
               }
}
-- --]]

--[[
mesh1 = mesh{
    filename = "samples/meshes/monkey.obj",
    material = {
                 brdf    = cook_torrance{ 
                                           m = 0.02, -- beckmann
                                           fresnel_type = "schlick-normal-reflectance", 
                                           reflectance_at_normal_incidence = { 0.95, 0.64, 0.54 },
                                           surface_sampler = "importance"
                                        },
                 emission = { 0, 0, 0 }
               }
}
-- --]]

