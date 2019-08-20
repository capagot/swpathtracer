package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
local image_width = 800
local image_height = 512

Camera{
    type     = "pinhole",
    position = { 0, 0.75, 8},
    look_at  = { 0, 0, 0},
    up       = { 0, 1, 0},
    fov      = 40,
}

PixelSampler{
    type = "jittered",
    spp = 16
}

ImageBuffer{
    width = image_width,
    height = image_height,
    file_name = "procedural.ppm",
    viewport_top = 0,
    viewport_left = 0,
    viewport_width = image_width,
    viewport_height = image_height
}

GlobalSettings{
    background_color = {0, 0, 0},
    acceleration_structure = "bvh-sah"
}

Integrator{
    type = "path-tracer",
    path_termination = "max-length",
    path_length = 5,
}

local sampler_type = "importance"

local light = Material{
    bsdf = LayeredBSDF{
        Lambertian{
            kd = {0, 0, 0},
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{2, 2, 2}
}

Triangle{
    v1 = { 2, 2, -2 },
    v2 = {-2, 2, -2 },
    v3 = { 2, 2,  2 },
    material = light
}

Triangle{
    v1 = {-2, 2, -2 },
    v2 = { 2, 2,  2 },
    v3 = {-2, 2,  2 },
    material = light
}

for c1 = 1, 10 do
    for c2 = 1, 10 do
        Sphere{
            center = { c1 - 5.5, -1.25, c2 - 5.5 },
            radius = 0.5,
            material = Material{
                bsdf = LayeredBSDF{
                    Lambertian{
                        kd = {math.random(), math.random(), math.random()},
                        bsdf_sampler = sampler_type
                    }
                },
                emission = Emission{0, 0, 0}
            }
        }
    end
end
