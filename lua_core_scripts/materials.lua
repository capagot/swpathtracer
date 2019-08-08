function updateRendererMaterialList(material)
    _G["__RENDERER_ELEMENTS__"]["__MATERIAL_COUNT__"] = _G["__RENDERER_ELEMENTS__"]["__MATERIAL_COUNT__"] + 1
    _G["__RENDERER_ELEMENTS__"]["__MATERIALS__"][_G["__RENDERER_ELEMENTS__"]["__MATERIAL_COUNT__"]] = material
    return _G["__RENDERER_ELEMENTS__"]["__MATERIAL_COUNT__"]
end

function Material(self)
    return updateRendererMaterialList(self)
end
