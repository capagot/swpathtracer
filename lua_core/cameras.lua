require "validation"

function Camera( self )
    if (not CheckStringField(self, "type", "pinhole", {"pinhole", "orthographic"})) then
        print("Lua ERROR: Invalid camera type. Exiting...")
        os.exit()
    end

    if (not CheckVector3Field(self, "position", {0, 0, 5})) then
        print("Lua ERROR: Invalid camera position vector. Exiting...")
        os.exit()
    end

    if (not CheckVector3Field(self, "look_at", {0, 0, 0})) then
        print("Lua ERROR: Invalid camera 'look at' vector. Exiting...")
        os.exit()
    end

    if (not CheckVector3Field(self, "up", {0, 1, 0})) then
        print("Lua ERROR: Invalid camera up vector. Exiting...")
        os.exit()
    end

    if (self.type == "pinhole") then
        if (not CheckNumberField(self, "fov", 55)) then
            print("Lua ERROR: Invalid camera FOV. Exiting...")
            os.exit()
        end
    else
        if (not CheckNumberField(self, "min_x")) then
            print("Lua ERROR: Invalid camera min_x. Exiting...")
            os.exit()
        end

        if (not CheckNumberField(self, "max_x")) then
            print("Lua ERROR: Invalid camera max_x. Exiting...")
            os.exit()
        end

        if (not CheckNumberField(self, "min_y")) then
            print("Lua ERROR: Invalid camera min_y. Exiting...")
            os.exit()
        end

        if (not CheckNumberField(self, "max_y")) then
            print("Lua ERROR: Invalid camera max_y. Exiting...")
            os.exit()
        end
    end
        
    _G["__RENDERER_ELEMENTS__"]["__CAMERA__"] = self
end
