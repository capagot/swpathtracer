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

-- Since metals present a strong specular component, 
-- and we are not using importance and direct light sampling yet, 
-- the spp must be large 
s = sampler{
    type = "jittered",
    spp = 3000
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

-- light source
l1 = sphere{
    center   = { 5, 5, 5 },
    radius   = 1,
    material = {
                 brdf     = lambertian{ kd = { 0, 0, 0 } },
                 emission = { 40, 40, 40 }
               }
}

-- Sphere
s1 = sphere{
    center   = { 0, 0, 0 },
    radius   = 1,
    material = {
                 brdf    = cook_torrance{ 
                                          material_type = "metal", -- "dieletric" // the fresnel computation will be different in each case
                                          m = 0.2, -- beckman                                        
                                          ks = { 0.95, 0.64, 0.54 },
                                        },
                 emission = { 0, 0, 0 }
               }
}

-- Back wall
local a = 20

t1 = triangle{ 
    vertices = { 
        { -a, -a, -3 },
        {  a, -a, -3 },
        {  a,  a, -3 } 
    }, 
    material = {
                 brdf     = lambertian{ kd = { 0.75, 0.75, 0.75 } },
                 emission = { 0, 0, 0 }
               }
}

t2 = triangle{ 
    vertices = { 
        {  a,  a, -3 },
        { -a,  a, -3 },
        { -a, -a, -3 } 
    }, 
    material = {
                 brdf     = lambertian{ kd = { 0.75, 0.75, 0.75 } },
                 emission = { 0, 0, 0 }
               }
}

-- Floor
t3 = triangle{ 
    vertices = { 
        {  a, -1, -a },
        { -a, -1, -a },
        {  a, -1,  a } 
    }, 
    material = {
                 brdf     = lambertian{ kd = { 0.75, 0.75, 0.75 } },
                 emission = { 0, 0, 0 }
               }
}

t4 = triangle{ 
    vertices = { 
        { -a, -1, -a },
        {  a, -1,  a },
        { -a, -1,  a } 
    }, 
    material = {
                 brdf     = lambertian{ kd = { 0.75, 0.75, 0.75 } },
                 emission = { 0, 0, 0 }
               }
}

-- Left wall
triangle7 = triangle{ 
    vertices = { 
        { -3,  a, -a },
        { -3,  a,  a },
        { -3, -a, -a } 
    }, 
    material = {
                 brdf     = lambertian{ kd = { 0.75, 0.25, 0.25 } },
                 emission = { 0, 0, 0 }
               }
}

triangle8 = triangle{ 
    vertices = { 
        { -3, -a, -a },
        { -3,  a,  a },
        { -3, -a,  a } 
    }, 
    material = {
                 brdf     = lambertian{ kd = { 0.75, 0.25, 0.25 } },
                 emission = { 0, 0, 0 }
               }
}

