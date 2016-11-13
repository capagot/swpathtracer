package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
local image_width = 512
local image_height = image_width

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
    spp = 3200
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

mesh1 = mesh{
    filename = "samples/meshes/monkey.obj",
    color = { 0.75, 0.75, 0.75 },
    emission = { 0, 0, 0 }
}

