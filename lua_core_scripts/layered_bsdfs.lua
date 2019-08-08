function updateRendererLayeredBSDFList(layered_bsdf)
    _G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDF_COUNT__"] = _G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDF_COUNT__"] + 1
    _G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDFS__"][_G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDF_COUNT__"]] = layered_bsdf
    return _G["__RENDERER_ELEMENTS__"]["__LAYERED_BSDF_COUNT__"]
end

function LayeredBSDF(self)
    self.num_layers = #self
    return updateRendererLayeredBSDFList(self)
end