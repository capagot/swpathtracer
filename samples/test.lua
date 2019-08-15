package.path = package.path .. ';../lua_core_scripts/?.lua'
package.path = package.path .. ';./lua_core_scripts/?.lua'

require "core"

-------------------------------------------------------------------------------

-- Camera{
--     type     = "pinhole",
--     position = { -15, -10, 0},
--     look_at  = { 0, -10, 0},
--     up       = { 0, 1, 0},
--     fov      = 40,
-- }

Camera{
    type     = "pinhole",
    position = { -10, 4, 0},
    look_at  = { 0, 4, 0},
    up       = { 0, 1, 0},
    fov      = 17,
}
    
-- Camera{
--     type     = "pinhole",
--     position = { -25, 4, 0},
--     look_at  = { 0, 4, 0},
--     up       = { 0, 1, 0},
--     fov      = 40,
-- }

-- Camera{
--     type     = "pinhole",
--     position = { -35, 7, 50},
--     look_at  = { 0, -4, -1},
--     up       = { 0, 1, 0},
--     fov      = 40,
-- }


-- Camera{
--     type     = "pinhole",
--     position = { 0, 5, 18},
--     look_at  = { 0, 5, 0},
--     up       = { 0, 1, 0},
--     fov      = 40,
-- }


-- Camera{
--     type     = "orthographic",
--     position = { 0, 0, 5},
--     look_at  = { 0, 0, 0},
--     up       = { 0, 1, 0 },
--     min_x    = -2,
--     max_x    = 2,
--     min_y    = -2,
--     max_y    = 2,
-- }


PixelSampler{
    type = "jittered", -- "uniform", "regular", "jittered"
    spp = 4 * 4-- * 4 * 4 --* 4 --* 4 --* 4 --* 4
}

local image_width = 128--1584 / 2
local image_height = 128--396 / 2

ImageBuffer{
    width = image_width,
    height = image_height,
    file_name = "sample_image2.ppm",
    viewport_top = 0, --300, --290,
    viewport_left = 0, --100, --270,
    viewport_width = image_width, --140, --160,
    viewport_height = image_height --160
}

GlobalSettings{
    background_color = {0, 0, 0},
    --acceleration_structure = "none"

    acceleration_structure = "bvh-sah"

    --acceleration_structure = "sbvh-sah",
    --overlap_threshold = 1e-05
}

-- Integrator{
--    type = "ray-caster",
--    --pixel_value = "normal",
--    pixel_value = "depth",
--    --minimum_depth = 0, -- will be considered as 0
--    --maximum_depth = 100 -- will be considered as 1
--    -- pixel_value = "intersection-test-count",
--    -- minimum_count = 0, -- will be dark blue
--    --maximum_count= 450 -- will be red
-- }

Integrator{
   type = "path-tracer",
   path_termination = "max-length",
   --path_termination = "russian-roulette",
   path_length = 5, -- or min path length when using RR
}


no_emission = Emission{0, 0, 0}


local sampler_type = "importance" -- "uniform"
--local sampler_type = "uniform"

mat_white = Material{
        bsdf = LayeredBSDF{
            Lambertian{
                kd = {0.75, 0.75, 0.75},
                --kd = {0.85, 0.85, 0.85},
                bsdf_sampler = sampler_type
            }
        },
        emission = no_emission
    }

mat_blue = Material{
        bsdf = LayeredBSDF{
            Lambertian{
                kd = {0.25, 0.25, 0.75},
                --kd = {0.85, 0.85, 0.85},
                bsdf_sampler = sampler_type
            }
        },
        emission = no_emission
    }

mat_red = Material{
        bsdf = LayeredBSDF{
            Lambertian{
                kd = {0.75, 0.25, 0.25},
                --kd = {0.85, 0.85, 0.85},
                bsdf_sampler = sampler_type
            }
        },
        emission = no_emission
    }

light = Material{
        bsdf = LayeredBSDF{
            Lambertian{
                kd = {0, 0, 0},
                bsdf_sampler = sampler_type
            }
        },
        emission = Emission{1.25, 1.25, 1.25}
    }

local a = 5
local ty = 5

-- -- back wall
-- Triangle{
--     v1 = {-a, -a + ty, -a},
--     v2 = { a, -a + ty, -a},
--     v3 = { a,  a + ty, -a},
--     material = mat_white
-- }

-- Triangle{
--     v1 = { a,  a + ty, -a},
--     v2 = {-a,  a + ty, -a},
--     v3 = {-a, -a + ty, -a},
--     material = mat_white
-- }

-- --top
-- Triangle{
--     v1 = {-a, a + ty, -a},
--     v2 = { a, a + ty, -a},
--     v3 = { a, a + ty,  a},
--     material = mat_white
-- }

-- Triangle{
--     v1 = {-a, a + ty, -a},
--     v2 = { a, a + ty,  a},
--     v3 = {-a, a + ty,  a},
--     material = mat_white
-- }

--light source

local b = 9.999
local c = 4.5

Triangle{
    v1 = {-c, b, -c},
    v2 = { c, b, -c},
    v3 = { c, b,  c},
    material = light
}

Triangle{
    v1 = {-c, b, -c},
    v2 = { c, b,  c},
    v3 = {-c, b,  c},
    material = light
}

--bottom
Triangle{
    v1 = { a, -a + ty, -a},
    v2 = {-a, -a + ty, -a},
    v3 = { a, -a + ty,  a},
    material = mat_white
}

Triangle{
    v1 = { a, -a + ty,  a},
    v2 = {-a, -a + ty, -a},
    v3 = {-a, -a + ty,  a},
    material = mat_white
}

-- --left wall ( red )
-- Triangle{
--     v1 = {-a,  a + ty, -a},
--     v2 = {-a,  a + ty,  a},
--     v3 = {-a, -a + ty, -a},

--     -- n1 = {0.57, 0.57, -0.57},
--     -- n2 = {0.57, 0.57, 0.57},
--     -- n3 = {0.57, -0.57, -0.57},

--     material = mat_red
-- }

-- Triangle{
--     v1 = {-a, -a + ty, -a},
--     v2 = {-a,  a + ty,  a},
--     v3 = {-a, -a + ty,  a},

--     -- n1 = {0.57, -0.57, -0.57},
--     -- n2 = {0.57, 0.57, 0.57},
--     -- n3 = {0.57, -0.57, 0.57},

--     material = mat_red
-- }

-- --right wall (blue)
-- Triangle{
--     v1 = { a,  a + ty,  a},
--     v2 = { a,  a + ty, -a},
--     v3 = { a, -a + ty, -a},
--     material = mat_blue
-- }

-- Triangle{
--     v1 = { a,  a + ty,  a},
--     v2 = { a, -a + ty, -a},
--     v3 = { a, -a + ty,  a},
--     material = mat_blue
-- }

-- mat_smooth_copper = Material{
--     bsdf = LayeredBSDF{
--         SmoothConductor{
--             reflectance_at_normal_incidence = {0.95, 0.64, 0.54},
--             bsdf_sampler = "importance"
--         }
--     },
--     emission = Emission{0, 0, 0}
-- }

-- mat_perf_mirror = Material{
--     bsdf = LayeredBSDF{
--         SmoothConductor{
--             reflectance_at_normal_incidence = {1, 1, 1},
--             bsdf_sampler = "importance"
--         }
--     },
--     emission = Emission{0, 0, 0}
-- }

-- mat_rough_copper = Material{
--     bsdf = LayeredBSDF{
--         CookTorrance{
--             roughness = 0.1,
--             reflectance_at_normal_incidence = {0.95, 0.64, 0.54},
--             bsdf_sampler = "importance"
--         }
--     },
--     emission = Emission{0, 0, 0}
-- }

-- mat_glass = Material{
--     bsdf = LayeredBSDF{
--         SmoothDielectric{
--             ior = 1.5,
--             bsdf_sampler = "importance"
--         }
--     },
--     emission = Emission{0, 0, 0}
-- }

-- mat_plastic  = Material{
--     bsdf = LayeredBSDF{
--         SmoothDielectric{
--             ior = 1.49,
--             bsdf_sampler = "importance"
--         }
--         -- ,
--         -- Lambertian{
--         --     kd = {0.85, 0, 0},
--         --     bsdf_sampler = "importance"
--         -- }
--         -- ,
--         -- CookTorrance{
--         --     roughness = 0.2,
--         --     reflectance_at_normal_incidence = {0.95, 0.64, 0.54},
--         --     bsdf_sampler = "importance"
--         -- }
--         -- ,
--         -- Lambertian{
--         --     kd = {0, 0, 1},
--         --     bsdf_sampler = "importance"
--         -- }
--     },
--     emission = Emission{0, 0, 0}
-- }

-- mat_metallic_painting = Material{
--     bsdf = LayeredBSDF{
--         SmoothDielectric{
--             ior = 1.5,
--             bsdf_sampler = "importance"
--         }
--         ,
--         CookTorrance{
--             roughness = 0.2,
--             reflectance_at_normal_incidence = {0.95, 0.64, 0.54},
--             --reflectance_at_normal_incidence = {0.6, 0.1, 0.2},
--             bsdf_sampler = "importance"
--         }
--     },
--     emission = Emission{0, 0, 0}
-- }


-- Sphere{
--    center = {1, -1.25, 0.35},
--    radius = 0.7,
--    --material = mat_rough_copper
--    --material = mat_smooth_copper
--    material = mat_glass
--    --material = mat_white
-- }


-- Sphere{
--    center = {0, 4, 0},
--    radius = 2,
--    --material = mat_perf_mirror
--    --material = mat_rough_copper
--    --material = mat_white
--    material = mat_plastic
--    --material = mat_metallic_painting
-- }

local i = 0;

math.randomseed(1)

for c1 = 1, 10 do
    for c2 = 1, 10 do

        local y_offset = math.random()

        local r = math.random()
        local g = math.random()
        local b = math.random()

        -- r = r * b * 0.25
        -- g = g * b * 0.15

        Sphere{
            center = { c1 - 5.5, 0.5 + y_offset * 9, c2 - 5.5 },
            radius = 0.5,
            material  = Material{
                bsdf = LayeredBSDF{
                    SmoothDielectric{
                        ior = 1.49,
                        bsdf_sampler = "importance"
                    }
                    ,
                    Lambertian{
                        --kd = {math.random(), math.random(), math.random()},
                        kd = {r, g, b},
                        bsdf_sampler = "importance"
                    }
                },
                emission = Emission{0, 0, 0}
            }
        }

        i = i + 1
    end
end

--  SmoothMesh{
--  --Mesh{
--       filename = "../samples/meshes/happy_buddha.obj",
--       material = mat_copper
-- }


-- mat_sib_light = Material{
--         bsdf = LayeredBSDF{
--             Lambertian{
--                 kd = {0, 0, 0},
--                 bsdf_sampler = sampler_type
--             }
--         },
--         emission = Emission{7, 7, 7}
--     }

-- local h = 20
-- local sx = 7
-- local sz = 1.5

-- --top
-- Triangle{
--     v1 = {-sx, h, -sz},
--     v2 = { sx, h, -sz},
--     v3 = { sx, h,  sz},
--     material = mat_sib_light
-- }

-- Triangle{
--     v1 = {-sx, h, -sz},
--     v2 = { sx, h,  sz},
--     v3 = {-sx, h,  sz},
--     material = mat_sib_light
-- }


 --SmoothMesh{
--  Mesh{
--       --filename = "../samples/meshes/monkey_no_normals.obj",
--       --filename = "../samples/meshes/happy_buddha.obj",
--       --filename = "../samples/meshes/sibernik2.obj",
--       filename = "../samples/meshes/sponza2.obj",
--       --material = mat_rough_copper
--       material = mat_white
--       --filename = "../samples/meshes/sibernik2.obj",
--       --material = mat_white
--   }

--TellMe()

--PrintBSDFList()

--PrintLayeredBSDFList()

--PrintEmissionList()

--PrintMaterialList()
