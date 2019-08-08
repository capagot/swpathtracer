import gdb
import matplotlib.pyplot as plt
import numpy as np

pixels = {}

for y in range(4):
    pixels[y]={}
    for x in range(4):
        pixels[y][x]=[]

class BreakpointSamplingPoints(gdb.Breakpoint):
    def stop (self):
        x_coord = int(gdb.parse_and_eval('x'))
        y_coord = int(gdb.parse_and_eval('y'))

        if (x_coord < 4) and (y_coord < 4):
            pixels[y_coord][x_coord] = [x_coord + 0.5, y_coord + 0.5]

        return False

class BreakpointPrintSamples(gdb.Breakpoint):
    def stop (self):
        for y in range(4):
            for x in range(4):
                print(str(pixels[y][x][0]) + " " + str(pixels[y][x][1]))
        return True

BreakpointSamplingPoints('raycaster.cpp:44')
BreakpointPrintSamples('raycaster.cpp:49')

gdb.execute('run')
