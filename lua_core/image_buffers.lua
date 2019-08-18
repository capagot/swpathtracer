require "validation"

function ImageBuffer( self )
   if (not CheckNumberField(self, "width", 512)) then
      print("Lua ERROR: Invalid image buffer width. Exiting...")
      os.exit()
   end

   if (not CheckNumberField(self, "height", 512)) then
      print("Lua ERROR: Invalid image buffer height. Exiting...")
      os.exit()
   end

   if (not CheckStringField(self, "file_name", "image.ppm")) then
      print("Lua ERROR: Invalid image buffer file name. Exiting...")
      os.exit()
   end
 
   if (not CheckNumberField(self, "viewport_top", 0)) then
      print("Lua ERROR: Invalid image buffer 'viewport_top'. Exiting...")
      os.exit()
   end

   if (not CheckNumberField(self, "viewport_left", 0)) then
      print("Lua ERROR: Invalid image buffer 'viewport_left'. Exiting...")
      os.exit()
   end

   if (not CheckNumberField(self, "viewport_width")) then
      print("Lua ERROR: Invalid image buffer 'viewport_width'. Exiting...")
      os.exit()
   end

   if (not CheckNumberField(self, "viewport_height")) then
      print("Lua ERROR: Invalid image buffer 'viewport_height'. Exiting...")
      os.exit()
   end

   _G["__RENDERER_ELEMENTS__"]["__IMAGE_BUFFER__"] = self
end
