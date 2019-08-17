require "validation"

function Camera( self )
    if (not CheckStringField(self, "type", "pinhole", {"pinhole", "orthographic"})) then
        print("Lua ERROR: Camera type  is wrong. Exiting...")
        os.exit()
    end

    if (not CheckVector3Field(self, "position", {0, 0, 5})) then
        print("Lua ERROR: Camera position vector is wrong. Exiting...")
        os.exit()
    end

    if (not CheckVector3Field(self, "look_at", {0, 0, 0})) then
        print("Lua ERROR: Camera 'look at' vector is wrong. Exiting...")
        os.exit()
    end

    if (not CheckVector3Field(self, "up", {0, 1, 0})) then
        print("Lua ERROR: Camera up vector is wrong. Exiting...")
        os.exit()
    end

    if (self.type == "pinhole") then
        if (not CheckNumberField(self, "fov", 55)) then
            print("Lua ERROR: Camera up vector is wrong. Exiting...")
            os.exit()
        end
    else
        if (not CheckNumberField(self, "min_x")) then
            print("Lua ERROR: Camera min_x is wrong. Exiting...")
            os.exit()
        end

        if (not CheckNumberField(self, "max_x")) then
            print("Lua ERROR: Camera max_x is wrong. Exiting...")
            os.exit()
        end

        if (not CheckNumberField(self, "min_y")) then
            print("Lua ERROR: Camera min_y is wrong. Exiting...")
            os.exit()
        end

        if (not CheckNumberField(self, "max_y")) then
            print("Lua ERROR: Camera max_y is wrong. Exiting...")
            os.exit()
        end
    end
        
    _G["__RENDERER_ELEMENTS__"]["__CAMERA__"] = self
end
