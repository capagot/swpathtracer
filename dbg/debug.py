import gdb
import matplotlib.pyplot as plt
import numpy as np

screen_samples = {}

class BreakpointGetImageRes(gdb.Breakpoint):
    def stop (self):

        width = int(gdb.parse_and_eval('camera_.getImage().getImageWidth()'))
        height = int(gdb.parse_and_eval('camera_.getImage().getImageHeight()'))

        print(">> image dim (width/height): " + str(width) + " x " + str(height) + " pixels")

        for y in range(height):
            screen_samples[y]={}
            for x in range(width):
                screen_samples[y][x]=[]

        return False

class BreakpointCollectScreenSpaceSamples(gdb.Breakpoint):
    def stop (self):
        x = int(gdb.parse_and_eval('x'))
        y = int(gdb.parse_and_eval('y'))

        sample_x = float(gdb.parse_and_eval('x_screen'))
        sample_y = float(gdb.parse_and_eval('y_screen'))

        screen_samples[y][x].append([sample_x, sample_y ])

        return False

class BreakpointPlotScreenSamples(gdb.Breakpoint):
    def stop (self):
        x_coord = []
        y_coord = []

        for y in range(len(screen_samples)):
            for x in range(len(screen_samples[0])):
                for s in range(len(screen_samples[y][x])):
                    x_coord.append(screen_samples[y][x][s][0])
                    y_coord.append(screen_samples[y][x][s][1])

        fig = plt.figure()
        ax = fig.add_subplot(1, 1, 1)

        major_ticks = np.arange(0, 101, 1)
        ax.set_xticks(major_ticks)
        ax.set_yticks(major_ticks)

        minor_ticks = np.arange(0, 101, 1/4)
        ax.set_xticks(minor_ticks, minor=True)
        ax.set_yticks(minor_ticks, minor=True)

        ax.grid(which='both')
        ax.grid(which='minor', alpha=0.5)
        ax.grid(which='major', alpha=1)

        plt.axis([-1, 6, -1, 6])
        plt.plot(x_coord, y_coord, 'ro')
        plt.show()

        return True

def GetRayCasterScreenSamples():
    gdb.execute('delete')
    BreakpointGetImageRes('raycaster.h:67')
    BreakpointCollectScreenSpaceSamples('raycaster.h:75')
    BreakpointPlotScreenSamples('raycaster.h:88')
    gdb.execute('run')

def GetPathtracerScreenSamples():
    gdb.execute('delete')
    BreakpointGetImageRes('pathtracer.cpp:73')
    BreakpointCollectScreenSpaceSamples('pathtracer.cpp:82')
    BreakpointPlotScreenSamples('pathtracer.cpp:102')
    gdb.execute('run')
