--[[
  The code below must not be altered (at least until I find a more elegant way 
  to inform the C++ code about the custom global variables created by the user in 
  the Lua configuration script).
--]]

local USER_VARS = {}

_G["user variables"] = USER_VARS

local function trace(t,k,v)
    USER_VARS[k]=v
    rawset(t,k,v)
end   

setmetatable(_G,{ __newindex = trace })

-------------------------------------------------------------------------------
package.path = package.path .. ';../lua_core/?.lua'
package.path = package.path .. ';./lua_core/?.lua'

require "primitives"
require "cameras"
require "samplers"
require "colors"

-------------------------------------------------------------------------------
local function tellme(offset, story)
    local n,v
    for n,v in pairs(story) do
        if n ~= "loaded" and n ~= "_G" then
            io.write (offset .. n .. " " )
            print (v)
            if type(v) == "table" then
                tellme(offset .. "--> ",v)
            end
        end
    end
end

-------------------------------------------------------------------------------

Camera1 = camera{
                    type     = "perspective",
                    position = { 0, 0, 6 },
                    up       = { 0, 1, 0 }, 
                    look_at  = { 0, 0, 0 },
                    aspect   = 1, 
                    fov      = 55
}
--]]

--[[
Camera2 = camera{
                    type     = "orthographic",
                    position = { 0, -1, 6 },
                    up       = { 0, 1, 0 }, 
                    look_at  = { 0, 0, 0 },
                    minx     = -2.5,
                    maxx     =  2.5,
                    miny     = -2.5,
                    maxy     =  2.5
}
--]]

Sampler1 = sampler{
                    spp = 16
}

Sphere1 = sphere{ 
    center = { 1, 1, 1 },
    radius = 1, 
    color = weird
}

Sphere2 = sphere{ 
    center = { 2, 2, 2 },
    radius = 2, 
    color = weird
}

Triangle1 = triangle{ 
    vertices = { 
        { 0, 0, 0 },
        { 11, 0, 0 },
        { 11, 11, 0 } 
    }, 
    color = blue,
    emission = {0, 0, 0 }
}

Triangle2 = triangle{ 
    vertices = { 
        { 0, 0, 0 },
        { 22, 0, 0 },
        { 22, 22, 0 } 
    }, 
    color = { 111, 222, 333 },
    emission = {0, 0, 0 }
}

local tri = union{}

for i = 1,3 do
    tri[i] =  triangle{ 
        vertices = { 
            { 0, 0, 0 },
            { i, 0, 0 },
            { i, i, 0 } 
        }, 
        color = { 111, 222, 333 },
        emission = {0, 0, 0 }
    }
end

s = union{}

for i = 1,5 do
    s[i] =  sphere{ 
                    center = { 1000  + i, 2000 + i, 3000 + i },
                    radius = 500 + i, 
                    color = { i, i, i },
                    emission = {0, 0, 0 }
            }
end

s[6] = tri;

y = 10

--tellme( "", _G["user variables"] )

