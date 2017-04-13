package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
local image_width = 320
local image_height = 320

Camera = camera{
    type     = "orthographic",
    position = { 0, 0, 6 },
    up       = { 0, 1, 0 },
    look_at  = { 0, 0, 0 },
    aspect   = image_width / image_height,
    minx      = -1.25,
    maxx      = 1.25,
    miny      = -1.25,
    maxy      = 1.25
}

s = sampler{
    type = "jittered",
    spp = 200
}

b = buffer{
    hres = image_width,
    vres = image_height
}

g = globals{
    background_color = { 0, 0, 0 },
    max_path_depth = 5,
    output_filename = "furnace_test.ppm",
    acceleration_data_structure = "none"
}

-- light source
light_source = sphere{
    center = { 0, 0, 0 },
    radius = 10,
    material = {
                 brdf = lambertian{ kd = { 0, 0, 0 },
                                    surface_sampler = "importance" 
                 },
                 emission = { 0.5, 0.5, 0.5 }
               }
}

-- sphere
s1 = sphere{
    center = { 0, 0, 0 },
    radius = 1,
    material = {
                 --[[
                 -- perfect specular reflection           
                 brdf = smooth_specular_reflection{
                                           fresnel_type = "schlick-normal-reflectance", 
                                           reflectance_at_normal_incidence = { 1, 1, 1 }
                       },    
                 -- --]]


                 -- cook-torrance                 
                 brdf    = cook_torrance{ 
                                           m = 0.2, -- beckmann
                                           fresnel_type = "schlick-normal-reflectance", 
                                           reflectance_at_normal_incidence = { 1, 1, 1 },
                                           surface_sampler = "importance"
                                         },
                 -- --]]
               
                 --[[ 
                 -- diffuse
                 brdf = lambertian{ kd = { 1, 1, 1 },
                                    surface_sampler = "uniform" 
                 },
                 -- --]]

                 emission = { 0, 0, 0 }
               }
}

