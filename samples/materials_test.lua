package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
local image_width = 870
local image_height = 150

Camera = camera{
    type     = "perspective",
    position = { 0, 0.7, 12 },
    up       = { 0, 1, 0 },
    look_at  = { 0, 0, 0 },
    aspect   = image_width / image_height,
    fov      = 12
}

s = sampler{
    type = "jittered",
    spp = 40
}

b = buffer{
    hres = image_width,
    vres = image_height
}

g = globals{
    background_color = { 0, 0, 0 },
    --path_termination = "russian-roulette",
    path_termination = "max-length",
    path_length = 1,
    output_filename = "material_test_2.ppm",
    acceleration_data_structure = "none"
}

-- Diffuse white light
local mat_light = { 
    lambertian{ 
        kd = { 0, 0, 0 },
        surface_sampler = "importance" 
    },
    emission = { 10, 10, 10 }
}

-- Diffuse gray light
local mat_weak_light = { 
    lambertian{ 
        kd = { 0, 0, 0 },
        surface_sampler = "importance" 
    },
    emission = { 0.15, 0.15, 0.15 }
}

-- Rough conductor (copper)
local mat_copper = {
    cook_torrance{ 
        m = 0.25, -- beckmann distribution
        reflectance_at_normal_incidence = { 0.95, 0.64, 0.54 }, -- copper reflectance at normal incidence
        surface_sampler = "importance"
    },
    emission = { 0, 0, 0 }
}

-- Smooth conductor (white)
local mat_smooth_refl = {
    smooth_specular_reflection{
        reflectance_at_normal_incidence = { 0.95, 0.64, 0.54 }, -- -copper reflectance at normal incidence
    },    
    emission = { 0, 0, 0 }
}

-- Smooth dielectric
local mat_smooth_dielectric = {
    smooth_dielectric{ 
        eta = 1.3
    },
    emission = { 0, 0, 0 }
}

-- Diffuse white 
local mat_white = { 
    lambertian{ 
        kd = { 0.75, 0.75, 0.75 },
        surface_sampler = "importance" 
    },
    emission = { 0, 0, 0 }
}

-- Diffuse red
local mat_red = { 
    lambertian{ 
        kd = { 0.75, 0, 0 },
        surface_sampler = "importance" 
    },
    emission = { 0, 0, 0 }
}

-- Red plastic made up of two layers: 
--     1) clear coat (smooth dielectric)
--     2) Diffuse reddish
local plastic = { 
    smooth_dielectric{ 
        eta = 1.5
    },
    lambertian{ 
        kd = { 0.75, 0, 0 },
        surface_sampler = "importance" 
    },
    emission = { 0, 0, 0 }
}

-- Rough conductor covered with clear varnish (2 layers)
--     1) clear coat (smooth dielectric)
--     2) Rough copper (rough condcutor)
local varnish = { 
    smooth_dielectric{ 
        eta = 1.5
    },
    cook_torrance{ 
        m = 0.375, 
        reflectance_at_normal_incidence = { 0.95, 0.64, 0.54 },
        surface_sampler = "importance"
    },
    emission = { 0, 0, 0 }
}

-- Small spherical light source
l1 = sphere{
    center   = { 6, 6, 6 },
    radius   = 3,
    material = mat_light
}

-- Sky light source (large sphere)
l2 = sphere{
    center   = { 0, 0, 0 },
    radius   = 2000,
    material = mat_weak_light
}

-- Floor
local a = 20000

t1 = triangle{ 
    vertices = { 
        {  a, -1, -a },
        { -a, -1, -a },
        {  a, -1,  a } 
    }, 
    material = mat_white
}

t2 = triangle{ 
    vertices = { 
        {  a, -1,  a },
        { -a, -1, -a },
        { -a, -1,  a } 
    }, 
    material = mat_white
}

-- Spheres
s1 = sphere{
    center   = { -5.5, 0, 0 },
    radius   = 1,
    material = mat_red
}

s2 = sphere{
    center   = { -3.3, 0, 0 },
    radius   = 1,
    material = mat_smooth_dielectric
}

s3= sphere{
    center   = { -1.1, 0, 0 },
    radius   = 1,
    material = mat_copper
}

s4 = sphere{
    center   = { 1.1, 0, 0 },
    radius   = 1,
    material = mat_smooth_refl
}

s5 = sphere{
    center   = { 3.3, 0, 0 },
    nter   = { 0, 0, 0 },
    radius   = 1,
    material = plastic
}

s6 = sphere{
    center   = { 5.5, 0, 0 },
    radius   = 1,
    material = varnish
}

