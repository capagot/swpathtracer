require "validation"

function updateRendererMaterialList(material)
    _G["__RENDERER_ELEMENTS__"]["__MATERIAL_COUNT__"] = _G["__RENDERER_ELEMENTS__"]["__MATERIAL_COUNT__"] + 1
    _G["__RENDERER_ELEMENTS__"]["__MATERIALS__"][_G["__RENDERER_ELEMENTS__"]["__MATERIAL_COUNT__"]] = material
    return _G["__RENDERER_ELEMENTS__"]["__MATERIAL_COUNT__"]
end

function Material(self)
    if (not CheckNumberField(self, "bsdf")) then
        print("Lua ERROR: Invalid material layered BSDF. Exiting...")
        os.exit()
    end    

    if (not CheckNumberField(self, "emission")) then
        print("Lua ERROR: Invalid material emission. Exiting...")
        os.exit()
    end    

    if (self.bsdf < 1) or (self.bsdf > _G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"]) then
        print("Lua ERROR: Invalid material layered BSDF: index out of range. Exiting...")
        os.exit()
    end 

    if (self.emission < 1) or (self.emission > _G["__RENDERER_ELEMENTS__"]["__EMISSION_COUNT__"]) then
        print("Lua ERROR: Invalid material emission: index out of range. Exiting...")
        os.exit()
    end 

    return updateRendererMaterialList(self)
end
