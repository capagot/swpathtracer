require "validation"

function GlobalSettings( self )
    if (not CheckStringField(self, "acceleration_structure", "bvh-sah", {"none", "bvh-sah"})) then
        print("Lua ERROR: Invalid acceleration structure. Exiting...")
        os.exit()
    end

    if (not CheckVector3Field(self, "background_color", {0, 0, 0})) then
        print("Lua ERROR: Invalid image background color. Exiting...")
        os.exit()
    end

    if (self.acceleration_structure == "bvh-sah") then
        self.overlap_threshold = math.huge
    end

    if (self.acceleration_structure == "sbvh-sah") then
        if (self.overlap_threshold == nil) then
            self.overlap_threshold = 1e-05
        end
    end

    _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"] = self
end
