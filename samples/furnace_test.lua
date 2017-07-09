package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
local image_width = 512
local image_height = 512

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
    spp = 512
}

b = buffer{
    hres = image_width,
    vres = image_height
}

g = globals{
    background_color = { 0, 0, 0 },
    max_path_depth = 10,
    output_filename = "furnace_test.ppm",
    acceleration_data_structure = "none"
}

local mat_light = {
    lambertian{ 
        kd = { 0, 0, 0 },
        surface_sampler = "importance" 
    },
    emission = { 0.5, 0.5, 0.5 }
}

local mat_lambertian = {
    lambertian{ 
        kd = { 1, 1, 1 },
        surface_sampler = "importance" 
    },
     emission = { 0, 0, 0 }
}

local mat_smooth_reflection = {
    smooth_specular_reflection{
        fresnel_type = "schlick-normal-reflectance", 
        reflectance_at_normal_incidence = { 1, 1, 1 }
    },    
    emission = { 0, 0, 0 }
}

local mat_smooth_dielectric = {
    smooth_dielectric{ 
        eta = 1.3
    },
    emission = { 0, 0, 0 }
}

local mat_cook_torrance = {
    cook_torrance{ 
        m = 0.2, -- beckmann distribution
        reflectance_at_normal_incidence = { 1, 1, 1 },
        surface_sampler = "importance"
    },
    emission = { 0, 0, 0 }
}

local plastic = { 
    smooth_dielectric{ 
        eta = 1.5
    },
    lambertian{ 
        kd = { 1, 1, 1 },
        surface_sampler = "importance" 
    },
    emission = { 0, 0, 0 }
}

local varnish = { 
    smooth_dielectric{ 
        eta = 1.5
    },
    cook_torrance{ 
        m = 0.375, 
        reflectance_at_normal_incidence = { 1, 1, 1 },
        surface_sampler = "importance"
    },
    emission = { 0, 0, 0 }
}

-- light source
light_source = sphere{
    center = { 0, 0, 0 },
    radius = 10,
    material = mat_light
}

-- sphere
s1 = sphere{
    center = { 0, 0, 0 },
    radius = 1,
    --material = mat_lambertian
    --material = mat_smooth_reflection
    --material = mat_cook_torrance
    --material = mat_smooth_dielectric
    material = plastic
    --material = varnish
}

