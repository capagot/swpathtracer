package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
Camera = camera{
    type     = "orthographic",
    position = { 0, 0, 6 },
    up       = { 0, 1, 0 },
    look_at  = { 0, 0, 0 },
    aspect   = 1,
    minx      = -1,
    maxx      = 1,
    miny      = -1,
    maxy      = 1
}

s = sampler{
    type = "jittered",
    spp = 200
}

b = buffer{
    hres = 320,
    vres = 320
}

g = globals{
    background_color = { 0, 0, 0 },
    max_path_depth = 5,
    output_filename = "furnace_test.ppm",
    acceleration_data_structure = "none"
}

-- light source
s0 = sphere{
    center = { 0, 0, 0 },
    radius = 10,
    color = { 0, 0, 0 },
    emission = { .18, .18, .18 }
}

-- sphere
s1 = sphere{
    center = { 0, 0, 0 },
    radius = 1,
    color = { 1, 1, 1 },
    emission = { 0, 0, 0 }
}

