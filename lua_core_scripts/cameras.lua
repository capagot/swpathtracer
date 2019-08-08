function Camera( self )
    _G["__RENDERER_ELEMENTS__"]["__CAMERA__"] = self
--[[    _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].type = self.type
    if (self.type == "pinhole") then
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].position = self.position
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].up = self.up
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].look_at = self.look_at
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].aspect = self.aspect
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].fov = self.fov
    end

    if (self.type == "orthographic") then
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].position = self.position
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].up = self.up
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].look_at = self.look_at
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].aspect = self.aspect
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].min_x = self.min_x
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].max_x = self.max_x
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].min_y = self.min_y
        _G["__RENDERER_ELEMENTS__"]["__CAMERA__"].max_y = self.max_y
    end--]]
end
