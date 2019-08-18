function updateRendererLayeredBSDFList(layered_bsdf)
    _G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDF_COUNT__"] = _G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDF_COUNT__"] + 1
    _G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDFS__"][_G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDF_COUNT__"]] = layered_bsdf
    return _G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDF_COUNT__"]
end

function LayeredBSDF(self)
    self.num_layers = #self

    for layer_count = 1, self.num_layers do
        if (not (type(self[layer_count]) == "number")) then
            print("Lua ERROR: Invalid layered BSDF layer. Exiting...")
            os.exit()
        end

        if (self[layer_count] < 1) or (self[layer_count] > _G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"]) then
            print("Lua ERROR: Invalid layered BSDF layer: index out of range. Exiting...")
            os.exit()            
        end
    end

    return updateRendererLayeredBSDFList(self)
end
