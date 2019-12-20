import gdb
import matplotlib.pyplot as plt
import numpy as np

light_ray_count = 0
tracking = False

class StartTrackingObj(gdb.Breakpoint):
    def stop (self):
        global tracking
        tracking = True
        return False

class TrackPathObj(gdb.Breakpoint):
    def stop(self):
        global light_ray_count

        local_wi_x = float(gdb.parse_and_eval('local_wi.x'))
        local_wi_y = float(gdb.parse_and_eval('local_wi.y'))
        local_wi_z = float(gdb.parse_and_eval('local_wi.z'))

        bsdf_sample_probability_ = float(gdb.parse_and_eval('bsdf_sample.probability_'))

        final_pdf = float(gdb.parse_and_eval('final_pdf'))

        # f_bsdf_r = float(gdb.parse_and_eval('final_bsdf.x'))
        # f_bsdf_g = float(gdb.parse_and_eval('final_bsdf.y'))
        # f_bsdf_b = float(gdb.parse_and_eval('final_bsdf.z'))

        light_ray_count = light_ray_count + 1

        print("===[" + str(light_ray_count) + "]=============================================================================")
        print ("  local_wi: [" + str(local_wi_x) + ", " + str(local_wi_y) + ", " + str(local_wi_z) + "]")
        print ("  bsdf_sample.probability_: " + str(bsdf_sample_probability_))
        print ("  final_pdf: " + str(final_pdf))


        # print ("  final_bsdf: [" + str(f_bsdf_r) + ", " + str(f_bsdf_g) + ", " + str(f_bsdf_b) + "]")
        return False

class EndTrackingObj(gdb.Breakpoint):
    def stop (self):
        if (tracking):
            print("Light tracking finished!")
            tracking = False

        return False

def TrackPath():
    global light_ray_count
    light_ray_count = 0

    gdb.execute('delete')
    StartTrackingObj('pathtracer.cpp:108')
    TrackPathObj('layered_bsdf.cpp:51')
    EndTrackingObj('layered_bsdf.cpp:54')
    gdb.execute('run')
