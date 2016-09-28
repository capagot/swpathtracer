function color( self )
    self.object_type = "color"
    return self
end

blue = color{ 5, 6, 7 }
red = color{ 1, 0, 0 }
white = color{ 1, 1, 1 }
weird = color{ 2 * 5, math.abs( -100 ), math.min(200, 300) }

