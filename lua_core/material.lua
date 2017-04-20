function lambertian( self )
    self.object_type = "lambertian_brdf"
    return self
end

function cook_torrance( self )
    self.object_type = "cook_torrance_brdf"
    return self
end

function smooth_specular_reflection( self )
    self.object_type = "smooth_specular_reflection"
    return self
end

function smooth_dielectric( self )
    self.object_type = "smooth_dielectric"
    return self
end

