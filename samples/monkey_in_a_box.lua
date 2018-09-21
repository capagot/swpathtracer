package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "core"

-------------------------------------------------------------------------------
local image_width = 512
local image_height = 512

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
    spp = 100
}

b = buffer{
    hres = image_width,
    vres = image_height
}

g = globals{
    background_color = { 0, 0, 0 },
    path_termination = "max-length",
    path_length = 5,
    output_filename = "monkey_in_a_box_SBVH.ppm",
    acceleration_data_structure = "sbvh-sah" -- "none"
}

local a = 2

local white = { 0.75, 0.75, 0.75 }

local red = { 0.75, 0.25, 0.25 }

local blue = { 0.25, 0.25, 0.75 }

local light = { 17, 17, 17 }

local mat_white = {
    lambertian{
        kd = white,
        surface_sampler = "importance"
    },
    emission = { 0, 0, 0 }
}

local mat_red = {
    lambertian{
        kd = red,
        surface_sampler = "importance"
    },
    emission = { 0, 0, 0 }
}

local mat_blue = {
    lambertian{
        kd = blue,
        surface_sampler = "importance"
    },
    emission = { 0, 0, 0 }
}

local mat_light = {
    lambertian{
        kd = { 0, 0, 0 },
        surface_sampler = "importance"
    },
    emission = light
}

local mat_copper = {
    cook_torrance{
        m = 0.25, -- beckmann distribution
        reflectance_at_normal_incidence = { 0.95, 0.64, 0.54 }, -- copper reflectance at normal incidence
        surface_sampler = "importance"
    },
    emission = { 0, 0, 0 }
}

--[[
-- back wall
t1 = triangle{
    vertices = {
        { -a, -a, -a },
        {  a, -a, -a },
        {  a,  a, -a }
    },
    material = mat_white
}

t2 = triangle{
    vertices = {
        {  a,  a, -a },
        { -a,  a, -a },
        { -a, -a, -a }
    },
    material = mat_white
}

-- top
t3 = triangle{
    vertices = {
        { -a, a, -a },
        {  a, a, -a },
        {  a, a,  a }
    },
    material = mat_white
}

t4 = triangle{
    vertices = {
        { -a, a, -a },
        {  a, a,  a },
        { -a, a,  a }
    },
    material = mat_white
}

-- bottom
t5 = triangle{
    vertices = {
        {  a, -a, -a },
        { -a, -a, -a },
        {  a, -a,  a }
    },
    material = mat_white
}

t6 = triangle{
    vertices = {
        {  a, -a,  a },
        { -a, -a, -a },
        { -a, -a,  a }
    },
    material = mat_white
}

-- left wall ( red )
t7 = triangle{
    vertices = {
        { -a,  a, -a },
        { -a,  a,  a },
        { -a, -a, -a }
    },
    material = mat_red
}

t8 = triangle{
    vertices = {
        { -a, -a, -a },
        { -a,  a,  a },
        { -a, -a,  a }
    },
    material = mat_red
}

-- right wall (blue)
t9 = triangle{
    vertices = {
        { a,  a,  a },
        { a,  a, -a },
        { a, -a, -a }
    },
    material = mat_blue
}

t10 = triangle{
    vertices = {
        { a,  a,  a },
        { a, -a, -a },
        { a, -a,  a }
    },
    material = mat_blue
}

-- light source
local offset = 0.001

t11 = triangle{
    vertices = {
        {  0.5, a - offset, -0.5 },
        { -0.5, a - offset, -0.5 },
        {  0.5, a - offset,  0.5 }
    },
    material = mat_light
}

t12 = triangle{
    vertices = {
        { -0.5, a - offset, -0.5 },
        {  0.5, a - offset,  0.5 },
        { -0.5, a - offset,  0.5 }
    },
    material = mat_light
}
--]]

mesh1 = mesh{
    filename = "samples/meshes/monkey.obj",
    material = mat_white
}

