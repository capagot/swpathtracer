import gdb
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d.art3d import Poly3DCollection, Line3DCollection

local_ray_direction = []
world_ray_direction = []

class BreakpointReadLocalRays(gdb.Breakpoint):
    def stop (self):
        local_x_ray_dir = float(gdb.parse_and_eval('local_ray_direction.x'))
        local_y_ray_dir = float(gdb.parse_and_eval('local_ray_direction.z')) # <<<< swap(y,z)
        local_z_ray_dir = float(gdb.parse_and_eval('local_ray_direction.y')) # <<<< swap(y,z)

        local_ray_direction.append([local_x_ray_dir, local_y_ray_dir, local_z_ray_dir])
        #print("+--+--+> " + str(local_y_ray_dir))

        world_x_ray_dir = float(gdb.parse_and_eval('world_ray_direction.x'))
        world_y_ray_dir = float(gdb.parse_and_eval('world_ray_direction.z')) # <<<< swap(y,z)
        world_z_ray_dir = float(gdb.parse_and_eval('world_ray_direction.y')) # <<<< swap(y,z)

        world_ray_direction.append([world_x_ray_dir, world_y_ray_dir, world_z_ray_dir])

        return False

class BreakpointPrintLocalRays(gdb.Breakpoint):
    def stop (self):

        local_ray_dir_x_coord = []
        local_ray_dir_y_coord = []
        local_ray_dir_z_coord = []

        world_ray_dir_x_coord = []
        world_ray_dir_y_coord = []
        world_ray_dir_z_coord = []

        #print(">>>> num local rays: " + str(len(local_ray_direction)))

        for i in range(len(local_ray_direction)):
            local_ray_dir_x_coord.append(local_ray_direction[i][0])
            local_ray_dir_y_coord.append(local_ray_direction[i][1])
            local_ray_dir_z_coord.append(local_ray_direction[i][2])
            #print("=-=-=-> " + str(local_ray_dir_x_coord[i]) + ", " + str(local_ray_dir_y_coord[i]) + ", " + str(local_ray_dir_z_coord[i]))

        #print(">>>> num world rays: " + str(len(world_ray_direction)))

        for i in range(len(world_ray_direction)):
            world_ray_dir_x_coord.append(world_ray_direction[i][0])
            world_ray_dir_y_coord.append(world_ray_direction[i][1])
            world_ray_dir_z_coord.append(world_ray_direction[i][2])



        fig = plt.figure()
        ax = fig.add_subplot(1, 1, 1)

        major_ticks = np.arange(-2, 101, 1)
        #major_ticks = np.arange(min(min_x, min_y), 101, 1)
        # minor_ticks = np.arange(0, 101, 1)
        ax.set_xticks(major_ticks)
        # ax.set_xticks(minor_ticks, minor=True)
        ax.set_yticks(major_ticks)
        # ax.set_yticks(minor_ticks, minor=True)
        ax.grid(which='both')
        # ax.grid(which='minor', alpha=0.2)
        # ax.grid(which='major', alpha=0.5)

        #plt.axis([min_x, max_x, min_y, max_y])
        plt.axis([-2, 2, -2, 2])

        #print ("++++++ " + str(len(local_ray_dir_x_coord)))
        #print ("++++++ " + str(len(local_ray_dir_y_coord)))

        plt.plot(local_ray_dir_x_coord, local_ray_dir_z_coord, 'ro')
        plt.show()

        return True


def PlotLocalSamples():
    BreakpointReadLocalRays('pinhole_camera.cpp:29')
    BreakpointPrintLocalRays('raycaster.cpp:37')
    gdb.execute('run')

