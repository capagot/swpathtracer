package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
local image_width = 800
local image_height = 512

Camera = camera{
    type     = "perspective",
    position = { 0, 0.75, 8 },
    up       = { 0, 1, 0 }, 
    look_at  = { 0, 0, 0 },
    aspect   = image_width / image_height, 
    fov      = 40
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
    output_filename = "procedural.ppm",
    acceleration_data_structure = "bvh-sah"
}

local mat_light = {
              brdf = lambertian{ kd = { 0, 0, 0 },
                                 surface_sampler = "importance" 
                               },
                 emission = { 2, 2, 2 }
}

-- light source
t1 = triangle{ 
    vertices = { 
        {  2, 2, -2 },
        { -2, 2, -2 },
        {  2, 2,  2 } 
    },
    material = mat_light
}

t2 = triangle{ 
    vertices = { 
        { -2, 2, -2 },
        {  2, 2,  2 },
        { -2, 2,  2 } 
    }, 
    material = mat_light
}

-- 100 randomly colored spheres
spheres = union{}

i = 1

for c1 = 1, 10 do
    for c2 = 1, 10 do
        spheres[i] = sphere{
            center = { c1 - 5.5, -1.25, c2 - 5.5 },
            radius = 0.5,
            material = {
                        brdf     = lambertian{ kd = { math.random(), math.random(), math.random() }, surface_sampler = "importance" },
                        emission = { 0, 0, 0 }
                       }
        }

        i = i + 1
    end
end

