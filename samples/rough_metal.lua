package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
local image_width = 256
local image_height = 256

Camera = camera{
    type     = "perspective",
    position = { 0, 0, 5 },
    up       = { 0, 1, 0 },
    look_at  = { 0, 0, 0 },

    aspect   = image_width / image_height,
    fov      = 45
}

s = sampler{
    type = "jittered",
    spp = 1000
}

b = buffer{
    hres = image_width,
    vres = image_height
}

g = globals{
    background_color = { 0, 0, 0 },
    max_path_depth = 5,
    output_filename = "rough_metal.ppm",
    acceleration_data_structure = "none"
}

local mat_light = { 
                    brdf =  lambertian{ 
                                        kd = { 0, 0, 0 },
                                        surface_sampler = "importance" 
                                      },
                    emission = { 40, 40, 40 }
                  }

local mat_copper = {
                    brdf = cook_torrance{ 
                                           m = 0.2, -- beckmann
                                           fresnel_type = "schlick-normal-reflectance", 
                                           reflectance_at_normal_incidence = { 0.95, 0.64, 0.54 },
                                           surface_sampler = "importance"
                                         },
                    emission = { 0, 0, 0 }
                  }

local mat_smooth_refl = {
                    brdf = smooth_specular_reflection{
                                           fresnel_type = "schlick-normal-reflectance", 
                                           reflectance_at_normal_incidence = { 1, 1, 1 }
                                                     },    
                    emission = { 0, 0, 0 }
}

local mat_white = { 
                    brdf =  lambertian{ 
                                        kd = { 0.75, 0.75, 0.75 },
                                        surface_sampler = "importance" 
                                      },
                    emission = { 0, 0, 0 }
                  }

local mat_red = { 
                    brdf =  lambertian{ 
                                        kd = { 0.75, 0.25, 0.25 },
                                        surface_sampler = "importance" 
                                      },
                    emission = { 0, 0, 0 }
                  }

-- light source
l1 = sphere{
    center   = { 5, 5, 5 },
    radius   = 1,
    material = mat_light
}

-- Sphere
s1 = sphere{
    center   = { 0, 0, 0 },
    radius   = 1,
    material = mat_copper
    --material = mat_smooth_refl
}

-- Back wall
local a = 20

t1 = triangle{ 
    vertices = { 
        { -a, -a, -3 },
        {  a, -a, -3 },
        {  a,  a, -3 } 
    }, 
    material = mat_white
}

t2 = triangle{ 
    vertices = { 
        {  a,  a, -3 },
        { -a,  a, -3 },
        { -a, -a, -3 } 
    }, 
    material = mat_white
}

-- Floor
t3 = triangle{ 
    vertices = { 
        {  a, -1, -a },
        { -a, -1, -a },
        {  a, -1,  a } 
    }, 
    material = mat_white
}

t4 = triangle{ 
    vertices = { 
        {  a, -1,  a },
        { -a, -1, -a },
        { -a, -1,  a } 
    }, 
    material = mat_white
}

-- Left wall
triangle7 = triangle{ 
    vertices = { 
        { -3,  a, -a },
        { -3,  a,  a },
        { -3, -a, -a } 
    }, 
    material = mat_red
}

triangle8 = triangle{ 
    vertices = { 
        { -3, -a, -a },
        { -3,  a,  a },
        { -3, -a,  a } 
    }, 
    material = mat_red
}

-- --]]

