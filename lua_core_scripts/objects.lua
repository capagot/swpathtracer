function updateRendererObjectList(object)
    _G["__RENDERER_ELEMENTS__"]["__OBJECT_COUNT__"] = _G["__RENDERER_ELEMENTS__"]["__OBJECT_COUNT__"] + 1
    _G["__RENDERER_ELEMENTS__"]["__OBJECTS__"][_G["__RENDERER_ELEMENTS__"]["__OBJECT_COUNT__"]] = object
end

function Triangle(self)
    self.type = "triangle"
    updateRendererObjectList(self)
end

function SmoothTriangle(self)
    self.type = "smooth_triangle"
    updateRendererObjectList(self)
end

function Sphere(self)
    self.type = "sphere"
    updateRendererObjectList(self)
end

function Mesh(self)
    self.type = "mesh"
    updateRendererObjectList(self)
end

function SmoothMesh(self)
    self.type = "smooth_mesh"
    updateRendererObjectList(self)
end
