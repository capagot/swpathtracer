package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
Camera = camera{
    type     = "perspective",
    position = { 0, 0.75, 8 },
    up       = { 0, 1, 0 }, 
    look_at  = { 0, 0, 0 },
    aspect   = 800 / 512, 
    fov      = 40
}

s = sampler{
    type = "jittered",
    spp = 10000
}

b = buffer{
    hres = 800,
    vres = 512
}

g = globals{
    background_color = { 0, 0, 0 },
    max_path_depth = 5,
    output_filename = "procedural.ppm"
}

-- light source
triangle1 = triangle{ 
    vertices = { 
        {  2, 2, -2 },
        { -2, 2, -2 },
        {  2, 2,  2 } 
    }, 
    color = { 0, 0, 0 },
    emission = { 2, 2, 2 }
}

triangle2 = triangle{ 
    vertices = { 
        { -2, 2, -2 },
        {  2, 2,  2 },
        { -2, 2,  2 } 
    }, 
    color = { 0, 0, 0 },
    emission = { 2, 2, 2 }
}

-- 100 randomly colored spheres
spheres = union{}

i = 1

for c1 = 1, 10 do
    for c2 = 1, 10 do
        spheres[i] = sphere{
            center = { c1 - 5.5, -1.25, c2 - 5.5 },
            radius = 0.5,
            color = { math.random(), math.random(), math.random() },
            emission = { 0, 0, 0 }
        }

        i = i + 1
    end
end

