package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
local image_width = 1200
local image_height = 207

Camera{
    type     = "pinhole",
    position = { 0, 0.7, 12},
    look_at  = { 0, 0, 0},
    up       = { 0, 1, 0},
    fov      = 12,
}

PixelSampler{
    type = "jittered",
    spp = 256 * 4 * 4 * 4
}

ImageBuffer{
    width = image_width,
    height = image_height,
    file_name = "materials_test.ppm",
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
    path_length = 3,
 }

local sampler_type = "importance"

local mat_light = Material{
    bsdf = LayeredBSDF{
        Lambertian{
            kd = {0, 0, 0},
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{10, 10, 10}
}

local mat_weak_light = Material{
    bsdf = LayeredBSDF{
        Lambertian{
            kd = {0, 0, 0},
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0.15, 0.15, 0.15}
}

-- Rough conductor (copper)
local mat_rough_conductor = Material{
    bsdf = LayeredBSDF{
        CookTorrance{
            roughness = 0.2,
            reflectance_at_normal_incidence = {0.95, 0.64, 0.54},
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0, 0, 0}
}

-- Smooth conductor
local mat_smooth_conductor = Material{
    bsdf = LayeredBSDF{
        SmoothConductor{
            reflectance_at_normal_incidence = {0.95, 0.64, 0.54},
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0, 0, 0}
}

-- Smooth dielectric
local mat_smooth_dielectric = Material{
    bsdf = LayeredBSDF{
        SmoothDielectric{
            ior = 1.3,
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0, 0, 0}
}


-- Diffuse white
local mat_white = Material{
    bsdf = LayeredBSDF{
        Lambertian{
            kd = {0.75, 0.75, 0.75},
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0, 0, 0}
}

local mat_red = Material{
    bsdf = LayeredBSDF{
        Lambertian{
            kd = {0.75, 0, 0},
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0, 0, 0}
}

-- Red plastic made up of two layers: 
local mat_red_plastic = Material{
    bsdf = LayeredBSDF{
        SmoothDielectric{
            ior = 1.49,
            bsdf_sampler = sampler_type
        },
        Lambertian{
            kd = {0.75, 0, 0},
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0, 0, 0}
}

-- Rough conductor covered with clear varnish (2 layers)
local mat_coated_copper = Material{
    bsdf = LayeredBSDF{
        SmoothDielectric{
            ior = 1.49,
            bsdf_sampler = sampler_type
        },
        CookTorrance{
            roughness = 0.3,
            reflectance_at_normal_incidence = {0.95, 0.64, 0.54},
            bsdf_sampler = sampler_type
        }
    },
    emission = Emission{0, 0, 0}
}

-- Small spherical light source
Sphere{
   center = {6, 6, 6},
   radius = 3,
   material = mat_light
}

-- Sky light source (large sphere)
Sphere{
    center = {0, 0, 0},
    radius = 2000,
    material = mat_weak_light
 }
 
-- Floor
local a = 20000

Triangle{
    v1 = { a, -1, -a},
    v2 = {-a, -1, -a},
    v3 = { a, -1,  a},
    material = mat_white
}

Triangle{
    v1 = { a, -1,  a},
    v2 = {-a, -1, -a},
    v3 = {-a, -1,  a},
    material = mat_white
}

-- Spheres
Sphere{
   center = {-5.5, 0, 0},
   radius = 1,
   material = mat_red
}

Sphere{
    center = {-3.3, 0, 0},
    radius = 1,
    material = mat_smooth_dielectric
 }
 
Sphere{
    center = {-1.1, 0, 0},
    radius = 1,
    material = mat_rough_conductor
 }

Sphere{
    center = {1.1, 0, 0},
    radius = 1,
    material = mat_smooth_conductor
 }

Sphere{
    center = {3.3, 0, 0},
    radius = 1,
    material = mat_red_plastic
 }

Sphere{
    center = {5.5, 0, 0},
    radius = 1,
    material = mat_coated_copper
}
