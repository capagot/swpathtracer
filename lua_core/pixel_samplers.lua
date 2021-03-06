require "validation"

function PixelSampler( self )
    if (not CheckStringField(self, "type", "jittered", {"uniform", "regular", "jittered"})) then
        print("Lua ERROR: Invalid pixel sampler type. Exiting...")
        os.exit()
    end

    if (not CheckNumberField(self, "spp", 16)) then
        print("Lua ERROR: Invalid pixel sampler spp. Exiting...")
        os.exit()
    end

    _G["__RENDERER_ELEMENTS__"]["__PIXEL_SAMPLER__"] = self
end
