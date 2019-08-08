function Integrator( self )
    if (self.type == "ray-caster") then
        if (self.pixel_value == "depth") then
            if (self.minimum_depth == nil) then
                self.minimum_depth = -1
            end
            if (self.maximum_depth == nil) then
                self.maximum_depth = -1
            end
        end
        if (self.pixel_value == "intersection-test-count") then
            if (self.minimum_count == nil) then
                self.minimum_count = -1
            end
            if (self.maximum_count == nil) then
                self.maximum_count = -1
            end
        end
    end

    _G["__RENDERER_ELEMENTS__"]["__INTEGRATOR__"] = self
end
