function GlobalSettings( self )
    if (self.acceleration_structure == "bvh-sah") then
        self.overlap_threshold = math.huge
    end

    if (self.acceleration_structure == "sbvh-sah") then
        if (self.overlap_threshold == nil) then
            self.overlap_threshold = 1e-05
        end
    end

    _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"] = self

--[[    _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].background_color = self.background_color
    _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].path_termination = self.path_termination
    _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].path_length = self.path_length
    _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].output_filename = self.output_filename
    _G["__RENDERER_ELEMENTS__"]["__GLOBAL_SETTINGS__"].acceleration_data_structure = self.acceleration_data_structure--]]
end
