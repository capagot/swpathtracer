package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
Camera1 = camera{
    type     = "perspective",
    position = { 0, 0, 8 },
    up       = { 0, 1, 0 }, 
    look_at  = { 0, 0, 0 },
    aspect   = 1, 
    fov      = 40
}

s1 = sampler{
    type = "jittered",
    spp = 1600
}

b = buffer{
    hres = 512,
    vres = 512
}

g = globals{
    background_color = { 0, 0, 0 },
    max_path_depth = 5,
    output_filename = "sphereinabox.ppm"
}

local s = 2

-- back wall
t1 = triangle{ 
    vertices = { 
        { -s, -s, -s },
        {  s, -s, -s },
        {  s,  s, -s } 
    }, 
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

t2 = triangle{ 
    vertices = { 
        {  s,  s, -s },
        { -s,  s, -s },
        { -s, -s, -s } 
    }, 
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}
--]]


-- top
t3 = triangle{ 
    vertices = { 
        {  s, s, -s },
        { -s, s, -s },
        {  s, s,  s } 
    }, 
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

t4 = triangle{ 
    vertices = { 
        { -s, s, -s },
        {  s, s,  s },
        { -s, s,  s } 
    }, 
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

-- bottom
t5 = triangle{ 
    vertices = { 
        {  s, -s, -s },
        { -s, -s, -s },
        {  s, -s,  s } 
    }, 
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

t6 = triangle{ 
    vertices = { 
        { -s, -s, -s },
        {  s, -s,  s },
        { -s, -s,  s } 
    }, 
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

-- left wall
t7 = triangle{ 
    vertices = { 
        { -s,  s, -s },
        { -s,  s,  s },
        { -s, -s, -s } 
    }, 
    color = { 0.75, 0.25, 0.25 },
    emission = { 0, 0, 0 }
}

t8 = triangle{ 
    vertices = { 
        { -s, -s, -s },
        { -s,  s,  s },
        { -s, -s,  s } 
    }, 
    color = { 0.75, 0.25, 0.25 },
    emission = { 0, 0, 0 }
}

-- right wall
t9 = triangle{ 
    vertices = { 
        { s,  s, -s },
        { s,  s,  s },
        { s, -s, -s } 
    }, 
    color = { 0.25, 0.25, 0.75 },
    emission = { 0, 0, 0 }
}

t10 = triangle{ 
    vertices = { 
        { s, -s, -s },
        { s,  s,  s },
        { s, -s,  s } 
    }, 
    color = { 0.25, 0.25, 0.75 },
    emission = { 0, 0, 0 }
}

sph1 = sphere{
    center = { -1, -1.25, -1 },
    radius = 0.75,
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

sph2 = sphere{
    center = { 1, -1.25, 0.8 },
    radius = 0.75,
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

-- light source

local offset = 0.001
local ls = 0.5
local e = { 17, 17, 17 }

t11 = triangle{ 
    vertices = { 
        {  ls, s - offset, -ls },
        { -ls, s - offset, -ls },
        {  ls, s - offset,  ls } 
    }, 
    color = { 0, 0, 0 },
    emission = e
}

t12 = triangle{ 
    vertices = { 
        { -ls, s - offset, -ls },
        {  ls, s - offset,  ls },
        { -ls, s - offset,  ls } 
    }, 
    color = { 0, 0, 0 },
    emission = e
}
--]]

-- mesh file
--[[
m1 = mesh{
    filename = "samples/meshes/tallbox.obj",
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}


m2 = mesh{
    filename = "samples/meshes/monkey.obj",
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}
--]]


