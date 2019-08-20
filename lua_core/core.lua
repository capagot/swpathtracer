require "registervars"
require "materials"
require "bsdfs"
require "layered_bsdfs"
require "emissions"
require "objects"
require "cameras"
require "pixel_samplers"
require "image_buffers"
require "global_settings"
require "integrators"

function IsIn(value, value_list)
    result = false

    for value_count = 1, #value_list do
        if (value == value_list[value_count]) then
            result = true
        end
    end    
    
    return result
end

-- Below is function that is run at the end of the script, to check
-- if the basic objects (camera, image buffer, integrator, etc.)
-- were selected.
-- TODO: It is still buggy...
-- Obtained from: https://stackoverflow.com/a/48938895/6107321

local m = {onexit = function() 
    if(not IsIn(_G["__RENDERER_ELEMENTS__"]["__CAMERA__"].type, {"pinhole", "orthographic"})) then
        print("Camera not set. Exiting...")
        os.exit()
    end

    -- check for pixel sampler
    if(not IsIn(_G["__RENDERER_ELEMENTS__"]["__PIXEL_SAMPLER__"].type, {"uniform", "regular", "jittered"})) then
        print("Pixel sampler not set. Exiting...")
        os.exit()
    end

    -- check for image biffer
    if(_G["__RENDERER_ELEMENTS__"]["__IMAGE_BUFFER__"].width == nil) then
        print("Image buffer not set. Exiting...")
        os.exit()
    end

    -- check for global settings
    if(not IsIn(_G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].acceleration_structure, {"none", "bvh-sah"})) then
        print("Global settings not set. Exiting...")
        os.exit()
    end

    -- check for integrator
    if(not IsIn(_G["__RENDERER_ELEMENTS__"]["__INTEGRATOR__"].type, {"ray-caster", "path-tracer"})) then
        print("Integrator not set. Exiting...")
        os.exit()
    end

    -- check at least one primitive
    if(_G["__RENDERER_ELEMENTS__"]["__OBJECT_COUNT__"] == 0) then
        print("Primitives not set. Exiting...")
        os.exit()
    end
end}

if _VERSION >= "Lua 5.2" then
  setmetatable(m, {__gc = m.onexit})
else
  m.sentinel = newproxy(true)
  getmetatable(m.sentinel).__gc = m.onexit
end
