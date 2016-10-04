package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
Camera = camera{
    type     = "perspective",
    position = { 0, 0, 8 },
    up       = { 0, 1, 0 }, 
    look_at  = { 0, 0, 0 },
    aspect   = 1, 
    fov      = 40
}

--[[
Camera = camera{
    type     = "orthographic",
    position = { 0, 0, 8 },
    up       = { 0, 1, 0 }, 
    look_at  = { 0, 0, 0 },
    minx   = -2, 
    maxx   = 2, 
    miny   = -2, 
    maxy   = 2, 
}
--]]

s = sampler{
    type = "jittered",
    spp = 20000
}

b = buffer{
    hres = 512,
    vres = 512
}

g = globals{
    background_color = { 0, 0, 0 },
    max_path_depth = 5,
    output_filename = "sphere_in_a_box.ppm"
}

local a = 2

-- back wall
triangle1 = triangle{ 
    vertices = { 
        { -a, -a, -a },
        {  a, -a, -a },
        {  a,  a, -a } 
    }, 
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

triangle2 = triangle{ 
    vertices = { 
        {  a,  a, -a },
        { -a,  a, -a },
        { -a, -a, -a } 
    }, 
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

-- top
triangle3 = triangle{ 
    vertices = { 
        {  a, a, -a },
        { -a, a, -a },
        {  a, a,  a } 
    }, 
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

triangle4 = triangle{ 
    vertices = { 
        { -a, a, -a },
        {  a, a,  a },
        { -a, a,  a } 
    }, 
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

-- bottom
triangle5 = triangle{ 
    vertices = { 
        {  a, -a, -a },
        { -a, -a, -a },
        {  a, -a,  a } 
    }, 
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

triangle6 = triangle{ 
    vertices = { 
        { -a, -a, -a },
        {  a, -a,  a },
        { -a, -a,  a } 
    }, 
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

-- left wall ( red )
triangle7 = triangle{ 
    vertices = { 
        { -a,  a, -a },
        { -a,  a,  a },
        { -a, -a, -a } 
    }, 
    color = { 0.75, 0.25, 0.25 },
    emission = { 0, 0, 0 }
}

triangle8 = triangle{ 
    vertices = { 
        { -a, -a, -a },
        { -a,  a,  a },
        { -a, -a,  a } 
    }, 
    color = { 0.75, 0.25, 0.25 },
    emission = { 0, 0, 0 }
}

-- right wall (blue)
triangle9 = triangle{ 
    vertices = { 
        { a,  a, -a },
        { a,  a,  a },
        { a, -a, -a } 
    }, 
    color = { 0.25, 0.25, 0.75 },
    emission = { 0, 0, 0 }
}

triangle10 = triangle{ 
    vertices = { 
        { a, -a, -a },
        { a,  a,  a },
        { a, -a,  a } 
    }, 
    color = { 0.25, 0.25, 0.75 },
    emission = { 0, 0, 0 }
}

-- light source
local offset = 0.001

triangle11 = triangle{ 
    vertices = { 
        {  0.5, a - offset, -0.5 },
        { -0.5, a - offset, -0.5 },
        {  0.5, a - offset,  0.5 } 
    }, 
    color = { 0, 0, 0 },
    emission = { 17, 17, 17 }
}

triangle12 = triangle{ 
    vertices = { 
        { -0.5, a - offset, -0.5 },
        {  0.5, a - offset,  0.5 },
        { -0.5, a - offset,  0.5 } 
    }, 
    color = { 0, 0, 0 },
    emission = { 17, 17, 17 }
}

-- two spheres
sphere1 = sphere{
    center = { -1, -1.25, -1 },
    radius = 0.75,
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

sphere2 = sphere{
    center = { 1, -1.25, 0.8 },
    radius = 0.75,
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

