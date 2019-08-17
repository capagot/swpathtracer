require "validation"

function Integrator( self )
    if (not CheckStringField(self, "type", "ray-caster", {"ray-caster", "path-tracer"})) then
        print("Lua ERROR: Invalid integrator type. Exiting...")
        os.exit()
    end

    if (self.type == "path-tracer") then
        if (not CheckStringField(self, "path_termination", "russian-roulette", {"max-length", "russian-roulette"})) then
            print("Lua ERROR: Invalid integrator path termination criterion. Exiting...")
            os.exit()
        end    

        if (not CheckNumberField(self, "path_length", 5)) then
            print("Lua ERROR: Invalid integrator path length. Exiting...")
            os.exit()
        end
    end

    if (self.type == "ray-caster") then
        if (not CheckStringField(self, "pixel_value", "normal", {"depth", "normal", "intersection-test-count"})) then
            print("Lua ERROR: Invalid ray caster type (pixel format). Exiting...")
            os.exit()
        end    
    end

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
