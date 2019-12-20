require "validation"

function updateRendererObjectList(object)
    _G["__RENDERER_ELEMENTS__"]["__OBJECT_COUNT__"] = _G["__RENDERER_ELEMENTS__"]["__OBJECT_COUNT__"] + 1
    _G["__RENDERER_ELEMENTS__"]["__OBJECTS__"][_G["__RENDERER_ELEMENTS__"]["__OBJECT_COUNT__"]] = object
end

function Triangle(self)
    self.type = "triangle"

    if (not CheckVector3Field(self, "v1")) then
        print("Lua ERROR: Invalid triangle vertex (v1). Exiting...")
        os.exit()
    end    

    if (not CheckVector3Field(self, "v2")) then
        print("Lua ERROR: Invalid triangle vertex (v2). Exiting...")
        os.exit()
    end    

    if (not CheckVector3Field(self, "v3")) then
        print("Lua ERROR: Invalid triangle vertex (v3). Exiting...")
        os.exit()
    end    

    updateRendererObjectList(self)
end

function SmoothTriangle(self)
    self.type = "smooth_triangle"

    if (not CheckVector3Field(self, "v1")) then
        print("Lua ERROR: Invalid smooth triangle vertex (v1). Exiting...")
        os.exit()
    end    

    if (not CheckVector3Field(self, "v2")) then
        print("Lua ERROR: Invalid smooth triangle vertex (v2). Exiting...")
        os.exit()
    end    

    if (not CheckVector3Field(self, "v3")) then
        print("Lua ERROR: Invalid smooth triangle vertex (v3). Exiting...")
        os.exit()
    end

    if (not CheckVector3Field(self, "n1")) then
        print("Lua ERROR: Invalid smooth triangle normal vector (n1). Exiting...")
        os.exit()
    end    

    if (not CheckVector3Field(self, "n2")) then
        print("Lua ERROR: Invalid smooth triangle normal vector (n2). Exiting...")
        os.exit()
    end    

    if (not CheckVector3Field(self, "n3")) then
        print("Lua ERROR: Invalid smooth triangle normal vector (n3). Exiting...")
        os.exit()
    end    

    updateRendererObjectList(self)
end

function Sphere(self)
    self.type = "sphere"

    if (not CheckVector3Field(self, "center")) then
        print("Lua ERROR: Invalid sphere center. Exiting...")
        os.exit()
    end    

    if (not CheckNumberField(self, "radius", 1)) then
        print("Lua ERROR: Invalid sphere radius. Exiting...")
        os.exit()
    end

    updateRendererObjectList(self)
end

function Mesh(self)
    self.type = "mesh"

    if (not CheckStringField(self, "filename")) then
        print("Lua ERROR: Invalid mesh file name. Exiting...")
        os.exit()
    end

    updateRendererObjectList(self)
end
