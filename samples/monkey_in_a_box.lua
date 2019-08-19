package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
local image_width = 512
local image_height = 512

Camera{
    type     = "pinhole",
    position = { 0, 0, 8 },
    look_at  = { 0, 0, 0 },
    up       = { 0, 1, 0 }, 
    fov      = 40,
}

PixelSampler{
    type = "jittered",
    spp = 256
}

ImageBuffer{
    width = image_width,
    height = image_height,
    file_name = "monkey_in_a_box.ppm",
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
    path_termination = "russian-roulette",
    path_length = 3
 }
 
local sampler_type = "importance"
local no_emission = Emission{0, 0, 0}

mat_white = Material{
    bsdf = LayeredBSDF{
        Lambertian{
            kd = {0.75, 0.75, 0.75},
            bsdf_sampler = sampler_type
        }
    },
    emission = no_emission
}

mat_red = Material{
    bsdf = LayeredBSDF{
        Lambertian{
            kd = {0.75, 0.25, 0.25},
            bsdf_sampler = sampler_type
        }
    },
    emission = no_emission
}

mat_blue = Material{
    bsdf = LayeredBSDF{
        Lambertian{
            kd = {0.25, 0.25, 0.75},
            bsdf_sampler = sampler_type
        }
    },
    emission = no_emission
}

mat_light = Material{
    bsdf = LayeredBSDF{
        Lambertian{
            kd = {0, 0, 0},
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{17, 17, 17}
}

local a = 2

-- back wall
Triangle{
    v1 = {-a,-a,-a},
    v2 = { a,-a,-a},
    v3 = { a, a,-a},
    material = mat_white
}

Triangle{
    v1 = { a, a,-a},
    v2 = {-a, a,-a},
    v3 = {-a,-a,-a},
    material = mat_white
}

-- top
Triangle{
    v1 = {-a, a,-a},
    v2 = { a, a,-a},
    v3 = { a, a, a},
    material = mat_white
}

Triangle{
    v1 = {-a, a,-a},
    v2 = { a, a, a},
    v3 = {-a, a, a},
    material = mat_white
}

-- bottom
Triangle{
    v1 = { a,-a,-a},
    v2 = {-a,-a,-a},
    v3 = { a,-a, a},
    material = mat_white
}

Triangle{
    v1 = { a,-a, a},
    v2 = {-a,-a,-a},
    v3 = {-a,-a, a},
    material = mat_white
}


-- left wall ( red )
Triangle{
    v1 = {-a, a,-a},
    v2 = {-a, a, a},
    v3 = {-a,-a,-a},
    material = mat_red
}

Triangle{
    v1 = {-a,-a,-a},
    v2 = {-a, a, a},
    v3 = {-a,-a, a},
    material = mat_red
}

-- right wall (blue)
Triangle{
    v1 = { a, a, a},
    v2 = { a, a,-a},
    v3 = { a,-a,-a},
    material = mat_blue
}

Triangle{
    v1 = { a, a, a},
    v2 = { a,-a,-a},
    v3 = { a,-a, a},
    material = mat_blue
}

-- light source
local offset = 0.001

Triangle{
    v1 = { 0.5, a - offset,-0.5},
    v2 = {-0.5, a - offset,-0.5},
    v3 = { 0.5, a - offset, 0.5},
    material = mat_light
}

Triangle{
    v1 = {-0.5, a - offset,-0.5},
    v2 = { 0.5, a - offset, 0.5},
    v3 = {-0.5, a - offset, 0.5},
    material = mat_light
}

Mesh{
    filename = "../samples/meshes/monkey.obj",
    material = mat_white
}