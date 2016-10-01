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
