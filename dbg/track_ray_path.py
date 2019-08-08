import gdb
import matplotlib.pyplot as plt
import numpy as np

# target_x_coord = 11
# target_y_coord = 80
# target_sample = 1

# target_x_coord = 10
# target_y_coord = 10
# target_sample = 1

tracking_path = False

class PathSegment:
    def __init__(self):
        self.depth = None
        self.world_ray_origin = None
        self.world_ray_direction = None
        self.hit_point = None
        self.hit_point_normal = None
        self.local_wo = None
        self.local_wi = None
        self.original_bsdf = None
        self.adjusted_bsdf = None
        self.emission = None
        self.pdf = None
        self.stop_reason = None
        self.returned_radiance = None

full_path = []

def DumpFullPathData():
    print("-- Full path data dump BEGIN ---------------------------------------------------")
    print("  Path length: " + str(len(full_path)))

    for i in range(len(full_path)):
        print ("  -- [" + str(i) + "] - depth " + str(full_path[i].depth) + " -------------------------------------------------------------------")

        if (full_path[i].world_ray_origin is not None):
            print ("    world ray origin: " + str(full_path[i].world_ray_origin[0]) + ", " + str(full_path[i].world_ray_origin[1]) + ", "  + str(full_path[i].world_ray_origin[2]))

        if (full_path[i].world_ray_direction is not None):
            print ("    world ray direction: " + str(full_path[i].world_ray_direction[0]) + ", " + str(full_path[i].world_ray_direction[1]) + ", "  + str(full_path[i].world_ray_direction[2]))

        if (full_path[i].hit_point is not None):
            print ("    hit point: " + str(full_path[i].hit_point[0]) + ", " + str(full_path[i].hit_point[1]) + ", "  + str(full_path[i].hit_point[2]))

        if (full_path[i].hit_point_normal is not None):
            print ("    hit point normal: " + str(full_path[i].hit_point_normal[0]) + ", " + str(full_path[i].hit_point_normal[1]) + ", "  + str(full_path[i].hit_point_normal[2]))

        if (full_path[i].local_wo is not None):
            print ("    local_wo: " + str(full_path[i].local_wo[0]) + ", " + str(full_path[i].local_wo[1]) + ", "  + str(full_path[i].local_wo[2]))

        if (full_path[i].local_wi is not None):
            print ("    local_wi: " + str(full_path[i].local_wi[0]) + ", " + str(full_path[i].local_wi[1]) + ", "  + str(full_path[i].local_wi[2]))

        if (full_path[i].original_bsdf is not None):
            print ("    original bsdf: " + str(full_path[i].original_bsdf[0]) + ", " + str(full_path[i].original_bsdf[1]) + ", "  + str(full_path[i].original_bsdf[2]))

        if (full_path[i].adjusted_bsdf is not None):
            print ("    adjusted bsdf: " + str(full_path[i].adjusted_bsdf[0]) + ", " + str(full_path[i].adjusted_bsdf[1]) + ", "  + str(full_path[i].adjusted_bsdf[2]))

        if (full_path[i].emission is not None):
            print ("    emission: " + str(full_path[i].emission[0]) + ", " + str(full_path[i].emission[1]) + ", "  + str(full_path[i].emission[2]))

        if (full_path[i].pdf is not None):
            print ("    pdf: " + str(full_path[i].pdf))

        if (full_path[i].stop_reason is not None):
            print ("    stop reason: " + full_path[i].stop_reason)

        if (full_path[i].returned_radiance is not None):
            print ("    returned_radiance: " + str(full_path[i].returned_radiance[0]) + ", " + str(full_path[i].returned_radiance[1]) + ", "  + str(full_path[i].returned_radiance[2]))

    print("-- Full path data dump END -----------------------------------------------------")

class StartTracking(gdb.Breakpoint):
    def stop (self):
        global target_x_coord
        global target_y_coord
        global target_sample
        global tracking_path

        x = int(gdb.parse_and_eval('x'))
        y = int(gdb.parse_and_eval('y'))
        sample = int(gdb.parse_and_eval('sample'))

        if ((x == target_x_coord) and (y == target_y_coord) and (sample == target_sample)):
            tracking_path = True;
            print("")
            print("================================================================================")
            print("== PATH TRACKING STARTED : " + "screen coord [" + str(x) + "," + str(y) + "], sample = " + str(sample))
            print("================================================================================")
        else:
            if (tracking_path == True):
                DumpFullPathData()
                print("================================================================================")
                print("== PATH TRACKING FINISHED")
                print("================================================================================")
                tracking_path = False;

        return False

class RecurringIntoNewSegment(gdb.Breakpoint):
    def stop (self):
        global tracking_path
        global full_path

        if (tracking_path == True):
            depth = int(gdb.parse_and_eval('depth'))
            print ("  >> analysing path segment at depth: " + str(depth))
            full_path.append(PathSegment())
            full_path[depth].depth = depth

        return False

class TerminateTrackingMaxDepth(gdb.Breakpoint):
    def stop (self):
        global tracking_path

        if (tracking_path == True):
            depth = int(gdb.parse_and_eval('depth'))
            print ("  Returning... max depth reached!")
            full_path[depth].stop_reason = "max depth reached"

class TerminateTrackingPrimaryRayMiss(gdb.Breakpoint):
    def stop (self):
        global tracking_path

        if (tracking_path == True):
            depth = int(gdb.parse_and_eval('depth'))
            print ("  Returning... primary ray missed surface (background color)!")
            full_path[depth].stop_reason = "primary ray missed surface (background color)"

class TerminateTrackingRayMiss(gdb.Breakpoint):
    def stop (self):
        global tracking_path

        if (tracking_path == True):
            depth = int(gdb.parse_and_eval('depth'))
            print ("  Returning... ray (not primary) missed surface (return zero)!")
            full_path[depth].stop_reason = "ray (not primary) missed surface (return zero)"

class TerminateTrackingRR(gdb.Breakpoint):
    def stop (self):
        global tracking_path

        if (tracking_path == True):
            depth = int(gdb.parse_and_eval('depth'))

            r_o_x = float(gdb.parse_and_eval('world_ray.origin_.x'))
            r_o_y = float(gdb.parse_and_eval('world_ray.origin_.y'))
            r_o_z = float(gdb.parse_and_eval('world_ray.origin_.z'))
            full_path[depth].world_ray_origin = np.array([r_o_x, r_o_y, r_o_z])

            r_d_x = float(gdb.parse_and_eval('world_ray.direction_.x'))
            r_d_y = float(gdb.parse_and_eval('world_ray.direction_.y'))
            r_d_z = float(gdb.parse_and_eval('world_ray.direction_.z'))
            full_path[depth].world_ray_direction = np.array([r_d_x, r_d_y, r_d_z])

            hp_x = float(gdb.parse_and_eval('intersection_record.position_.x'))
            hp_y = float(gdb.parse_and_eval('intersection_record.position_.y'))
            hp_z = float(gdb.parse_and_eval('intersection_record.position_.z'))
            full_path[depth].hit_point = np.array([hp_x, hp_y, hp_z])

            hpn_x = float(gdb.parse_and_eval('intersection_record.normal_.x'))
            hpn_y = float(gdb.parse_and_eval('intersection_record.normal_.y'))
            hpn_z = float(gdb.parse_and_eval('intersection_record.normal_.z'))
            full_path[depth].hit_point_normal = np.array([hpn_x, hpn_y, hpn_z])

            l_wo_x = float(gdb.parse_and_eval('local_wo.x'))
            l_wo_y = float(gdb.parse_and_eval('local_wo.y'))
            l_wo_z = float(gdb.parse_and_eval('local_wo.z'))
            full_path[depth].local_wo = np.array([l_wo_x, l_wo_y, l_wo_z])

            l_wi_x = float(gdb.parse_and_eval('local_wi.x'))
            l_wi_y = float(gdb.parse_and_eval('local_wi.y'))
            l_wi_z = float(gdb.parse_and_eval('local_wi.z'))
            full_path[depth].local_wi = np.array([l_wi_x, l_wi_y, l_wi_z])

            ob_x = float(gdb.parse_and_eval('bsdf.x'))
            ob_y = float(gdb.parse_and_eval('bsdf.y'))
            ob_z = float(gdb.parse_and_eval('bsdf.z'))
            full_path[depth].original_bsdf = np.array([ob_x, ob_y, ob_z])

            e_x = float(gdb.parse_and_eval('emission.x'))
            e_y = float(gdb.parse_and_eval('emission.y'))
            e_z = float(gdb.parse_and_eval('emission.z'))
            full_path[depth].emission = np.array([e_x, e_y, e_z])

            full_path[depth].returned_radiance = np.array([e_x, e_y, e_z])

            full_path[depth].pdf = float(gdb.parse_and_eval('pdf'))

            print ("  Returning... russian roulette!")
            full_path[depth].stop_reason = "russian roulette"

class AfterIntersectionEvaluation(gdb.Breakpoint):
    def stop (self):
        global tracking_path
        global full_path

        if (tracking_path == True):
            depth = int(gdb.parse_and_eval('depth'))

            r_o_x = float(gdb.parse_and_eval('world_ray.origin_.x'))
            r_o_y = float(gdb.parse_and_eval('world_ray.origin_.y'))
            r_o_z = float(gdb.parse_and_eval('world_ray.origin_.z'))
            full_path[depth].world_ray_origin = np.array([r_o_x, r_o_y, r_o_z])

            r_d_x = float(gdb.parse_and_eval('world_ray.direction_.x'))
            r_d_y = float(gdb.parse_and_eval('world_ray.direction_.y'))
            r_d_z = float(gdb.parse_and_eval('world_ray.direction_.z'))
            full_path[depth].world_ray_direction = np.array([r_d_x, r_d_y, r_d_z])

            hp_x = float(gdb.parse_and_eval('intersection_record.position_.x'))
            hp_y = float(gdb.parse_and_eval('intersection_record.position_.y'))
            hp_z = float(gdb.parse_and_eval('intersection_record.position_.z'))
            full_path[depth].hit_point = np.array([hp_x, hp_y, hp_z])

            hpn_x = float(gdb.parse_and_eval('intersection_record.normal_.x'))
            hpn_y = float(gdb.parse_and_eval('intersection_record.normal_.y'))
            hpn_z = float(gdb.parse_and_eval('intersection_record.normal_.z'))
            full_path[depth].hit_point_normal = np.array([hpn_x, hpn_y, hpn_z])

            l_wo_x = float(gdb.parse_and_eval('local_wo.x'))
            l_wo_y = float(gdb.parse_and_eval('local_wo.y'))
            l_wo_z = float(gdb.parse_and_eval('local_wo.z'))
            full_path[depth].local_wo = np.array([l_wo_x, l_wo_y, l_wo_z])

            l_wi_x = float(gdb.parse_and_eval('local_wi.x'))
            l_wi_y = float(gdb.parse_and_eval('local_wi.y'))
            l_wi_z = float(gdb.parse_and_eval('local_wi.z'))
            full_path[depth].local_wi = np.array([l_wi_x, l_wi_y, l_wi_z])

            ab_x = float(gdb.parse_and_eval('bsdf.x'))
            ab_y = float(gdb.parse_and_eval('bsdf.y'))
            ab_z = float(gdb.parse_and_eval('bsdf.z'))
            full_path[depth].adjusted_bsdf = np.array([ab_x, ab_y, ab_z])

            e_x = float(gdb.parse_and_eval('emission.x'))
            e_y = float(gdb.parse_and_eval('emission.y'))
            e_z = float(gdb.parse_and_eval('emission.z'))
            full_path[depth].emission = np.array([e_x, e_y, e_z])

            full_path[depth].pdf = float(gdb.parse_and_eval('pdf'))

        return False

class GetOriginalBSDF(gdb.Breakpoint):
    def stop (self):
        global tracking_path
        global full_path

        if (tracking_path == True):
            depth = int(gdb.parse_and_eval('depth'))

            ob_x = float(gdb.parse_and_eval('bsdf.x'))
            ob_y = float(gdb.parse_and_eval('bsdf.y'))
            ob_z = float(gdb.parse_and_eval('bsdf.z'))
            full_path[depth].original_bsdf = np.array([ob_x, ob_y, ob_z])

        return False

class ReturnedRadiance(gdb.Breakpoint):
    def stop (self):
        global tracking_path
        global full_path

        if (tracking_path == True):
            depth = int(gdb.parse_and_eval('depth'))

            r_x = float(gdb.parse_and_eval('radiance.x'))
            r_y = float(gdb.parse_and_eval('radiance.y'))
            r_z = float(gdb.parse_and_eval('radiance.z'))
            full_path[depth].returned_radiance = np.array([r_x, r_y, r_z])

        return False

class SampleValues(gdb.Breakpoint):
    def stop (self):
        x = int(gdb.parse_and_eval('x'))
        y = int(gdb.parse_and_eval('y'))

        if ((x == target_x_coord) and (y == target_y_coord)):
            sample = int(gdb.parse_and_eval('sample'))
            r_x = float(gdb.parse_and_eval('radiance.x'))
            r_y = float(gdb.parse_and_eval('radiance.y'))
            r_z = float(gdb.parse_and_eval('radiance.z'))
            print(">>>> " + str(sample) + " : " + str(r_x) + ", " + str(r_y) + ", " + str(r_z))

        return False


def TrackPath(x, y, sample):
    global target_x_coord
    global target_y_coord
    global target_sample

    target_x_coord = x
    target_y_coord = y
    target_sample = sample

    gdb.execute('delete')
    StartTracking('pathtracer.cpp:90')
    RecurringIntoNewSegment('pathtracer.cpp:18')
    TerminateTrackingMaxDepth('pathtracer.cpp:19')
    TerminateTrackingPrimaryRayMiss('pathtracer.cpp:23')
    TerminateTrackingRayMiss('pathtracer.cpp:25')
    GetOriginalBSDF('pathtracer.cpp:39')
    TerminateTrackingRR('pathtracer.cpp:45')
    AfterIntersectionEvaluation('pathtracer.cpp:48')
    ReturnedRadiance('pathtracer.cpp:50')
    gdb.execute('run')

def ShowSampleValues(x, y):
    global target_x_coord
    global target_y_coord

    target_x_coord = x
    target_y_coord = y

    gdb.execute('delete')
    SampleValues('pathtracer.cpp:100')
    gdb.execute('run')

