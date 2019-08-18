require "validation"

function updateRendererEmissionList(emission)
    _G["__RENDERER_ELEMENTS__"]["__EMISSION_COUNT__"] = _G["__RENDERER_ELEMENTS__"]["__EMISSION_COUNT__"] + 1
    _G["__RENDERER_ELEMENTS__"]["__EMISSIONS__"][_G["__RENDERER_ELEMENTS__"]["__EMISSION_COUNT__"]] = emission
    return _G["__RENDERER_ELEMENTS__"]["__EMISSION_COUNT__"]
end

function Emission(self)
    if (not CheckVector3(self)) then
        print("Lua ERROR: Invalid emission value. Exiting...")
        os.exit()
    end

    return updateRendererEmissionList(self)
end
