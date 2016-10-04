package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
Camera = camera{
    type     = "perspective",
    position = { 50, 52, 295.6 },
    up       = { 0, 1, 0 }, 
    look_at  = { 50, -0.042612 + 52, -1 + 295.6 },
    aspect   = 1024 / 768, 
    fov      = 30
}

s = sampler{
    type = "jittered",
    spp = 400
}

b = buffer{
    hres = 1024,
    vres = 768
}

g = globals{
    background_color = { 0, 0, 0 },
    max_path_depth = 5,
    output_filename = "smallpt_diffuse.ppm"
}

local a = 2

-- top
s1 = sphere{
    center = { 50, -1e5 + 81.6, 81.6 },
    radius = 1e5,
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

-- bottom
s2 = sphere{
    center = { 50, 1e5, 81.6 },
    radius = 1e5,
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

-- back
s3 = sphere{
    center = { 50, 40.8, 1e5 },
    radius = 1e5,
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

-- left 
s4 = sphere{
    center = { 1e5 + 1, 40.8, 81.6 },
    radius = 1e5,
    color = { 0.75, 0.25, 0.25 },
    emission = { 0, 0, 0 }
}

-- right
s5 = sphere{
    center = { -1e5 + 99, 40.8, 81.6 },
    radius = 1e5,
    color = { 0.25, 0.25, 0.75 },
    emission = { 0, 0, 0 }
}

-- light
s6 = sphere{
    center = { 50, 681.6 - 0.27, 81.6 },
    radius = 600,
    color = { 0, 0, 0 },
    emission = { 12, 12, 12 }
}

-- two spheres
sphere1 = sphere{
    center = { 27, 16.5, 47 },
    radius = 16.5,
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

sphere2 = sphere{
    center = { 73, 16.5, 78 },
    radius = 16.5,
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

