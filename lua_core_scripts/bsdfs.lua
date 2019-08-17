require "validation"

function updateRendererBSDFList(bsdf)
    _G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"] = _G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"] + 1
    _G["__RENDERER_ELEMENTS__"]["__BSDFS__"][_G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"]] = bsdf
    return _G["__RENDERER_ELEMENTS__"]["__BSDF_COUNT__"]
end

bsdf_samplers = {"uniform", "importance"}

function Lambertian(self)
    self.type = "lambertian"

    if (not CheckVector3Field(self, "kd", {0.75, 0.75, 0.75})) then
        print("Lua ERROR: Lambertian reflectance (kd field) is wrong. Exiting...")
        os.exit()
    end

    if (not CheckStringField(self, "bsdf_sampler", "importance", bsdf_samplers)) then
        print("Lua ERROR: BSDF sampler is wrong. Exiting...")
        os.exit()
    end

    return updateRendererBSDFList(self)
end

function SmoothConductor(self)
    self.type = "smooth_conductor"

    if (not CheckVector3Field(self, "reflectance_at_normal_incidence", {0.99, 0.99, 0.99})) then
        print("Lua ERROR: Smooth conductor reflectance at normal incidence is wrong. Exiting...")
        os.exit()
    end

    if (not CheckStringField(self, "bsdf_sampler", "importance", bsdf_samplers)) then
        print("Lua ERROR: BSDF sampler is wrong. Exiting...")
        os.exit()
    end

    return updateRendererBSDFList(self)
end

function SmoothDielectric(self)
    self.type = "smooth_dielectric"

    if (not CheckNumberField(self, "ior", 1.5)) then
        print("Lua ERROR: Smooth dielectric IOR is wrong. Exiting...")
        os.exit()
    end

    if (not CheckStringField(self, "bsdf_sampler", "importance", bsdf_samplers)) then
        print("Lua ERROR: BSDF sampler is wrong. Exiting...")
        os.exit()
    end

    return updateRendererBSDFList(self)
end

function CookTorrance(self)
    self.type = "cook_torrance"

    if (not CheckNumberField(self, "roughness", 0.2)) then
        print("Lua ERROR: Cook-Torrance roughness is wrong. Exiting...")
        os.exit()
    end

    if (not CheckVector3Field(self, "reflectance_at_normal_incidence", {0.99, 0.99, 0.99})) then
        print("Lua ERROR: Cook-Torrance reflectance at normal incidence is wrong. Exiting...")
        os.exit()
    end

    if (not CheckStringField(self, "bsdf_sampler", "importance", bsdf_samplers)) then
        print("Lua ERROR: BSDF sampler is wrong. Exiting...")
        os.exit()
    end

    return updateRendererBSDFList(self)
end
