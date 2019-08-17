require "validation"

function ImageBuffer( self )
   if (not CheckNumberField(self, "width", 512)) then
      print("Lua ERROR: Image buffer width is wrong. Exiting...")
      os.exit()
   end

   if (not CheckNumberField(self, "height", 512)) then
      print("Lua ERROR: Image buffer height is wrong. Exiting...")
      os.exit()
   end

   if (not CheckStringField(self, "file_name", "image.ppm")) then
      print("Lua ERROR: Image buffer file name is wrong. Exiting...")
      os.exit()
   end
 
   if (not CheckNumberField(self, "viewport_top", 0)) then
      print("Lua ERROR: Image buffer viewport_top is wrong. Exiting...")
      os.exit()
   end

   if (not CheckNumberField(self, "viewport_left", 0)) then
      print("Lua ERROR: Image buffer viewport_left is wrong. Exiting...")
      os.exit()
   end

   if (not CheckNumberField(self, "viewport_width")) then
      print("Lua ERROR: Image buffer viewport_width is wrong. Exiting...")
      os.exit()
   end

   if (not CheckNumberField(self, "viewport_height")) then
      print("Lua ERROR: Image buffer viewport_height is wrong. Exiting...")
      os.exit()
   end

   _G["__RENDERER_ELEMENTS__"]["__IMAGE_BUFFER__"] = self
end
