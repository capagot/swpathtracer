function updateRendererBSDFList(bsdf)
    _G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"] = _G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"] + 1
    _G["__RENDERER_ELEMENTS__"]["__BSDFS__"][_G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"]] = bsdf
    return _G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"]
end

function Lambertian(self)
    self.type = "lambertian"
    if (self.surface_sampler == nil) then
        self.surface_sampler = "importance"
    end
    return updateRendererBSDFList(self)
end

function SmoothDielectric(self)
    self.type = "smooth_dielectric"
    if (self.surface_sampler == nil) then
        self.surface_sampler = "importance"
    end
    return updateRendererBSDFList(self)
end

function SmoothConductor(self)
    self.type = "smooth_conductor"
    if (self.surface_sampler == nil) then
        self.surface_sampler = "importance"
    end
    return updateRendererBSDFList(self)
end

function CookTorrance(self)
    self.type = "cook_torrance"
    if (self.surface_sampler == nil) then
        self.surface_sampler = "importance"
    end
    return updateRendererBSDFList(self)
end

