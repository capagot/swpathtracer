package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
local image_width = 512
local image_height = 512

Camera{
    type     = "orthographic",
    position = { 0, 0, 6 },
    look_at  = { 0, 0, 0 },
    up       = { 0, 1, 0 },
    min_x      = -1.25,
    max_x      = 1.25,
    min_y      = -1.25,
    max_y      = 1.25
}

PixelSampler{
    type = "jittered",
    spp = 64
}

ImageBuffer{
    width = image_width,
    height = image_height,
    file_name = "furnace_test.ppm",
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

local perf_reflectance = {0.999, 0.999, 0.999}

local mat_light = Material{
    bsdf = LayeredBSDF{
        Lambertian{
            kd = {0, 0, 0},
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0.5, 0.5, 0.5}
}

local mat_lambertian = Material{
    bsdf = LayeredBSDF{
        Lambertian{
            kd = perf_reflectance,
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0, 0, 0}
}

local mat_smooth_conductor = Material{
    bsdf = LayeredBSDF{
        SmoothConductor{
            reflectance_at_normal_incidence = perf_reflectance,
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0, 0, 0}
}

local mat_smooth_dielectric = Material{
    bsdf = LayeredBSDF{
        SmoothDielectric{
            ior = 1.49,
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0, 0, 0}
}

local mat_cook_torrance = Material{
    bsdf = LayeredBSDF{
        CookTorrance{
            roughness = 0.2,
            reflectance_at_normal_incidence = perf_reflectance,
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0, 0, 0}
}

local mat_plastic = Material{
    bsdf = LayeredBSDF{
        SmoothDielectric{
            ior = 1.49,
            bsdf_sampler = sampler_type
        },
        Lambertian{
            kd = perf_reflectance,
            bsdf_sampler = sampler_type
        }
    },

    emission = Emission{0, 0, 0}
}

local mat_coating = Material{
    bsdf = LayeredBSDF{
        SmoothDielectric{
            ior = 1.49,
            bsdf_sampler = sampler_type
        },
        CookTorrance{
            roughness = 0.2,
            reflectance_at_normal_incidence = perf_reflectance,
            bsdf_sampler = sampler_type
        }
    },

    emission = Emission{0, 0, 0}
}

-- light source
Sphere{
    center = {0, 0, 0},
    radius = 10,
    material = mat_light
}

-- sphere
Sphere{
    center = { 0, 0, 0 },
    radius = 1,
    --material = mat_lambertian
    --material = mat_smooth_conductor
    --material = mat_cook_torrance
    --material = mat_smooth_dielectric
    --material = mat_plastic
    material = mat_coating
}
