from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection, Line3DCollection
import numpy as np

class AABB (object):
    def __init__(self, min_point, max_point, color=[0.5, 0.5, 0.5], linewidth=1.0, linecolor=[0.0, 0.0, 0.0], alpha=0.1):
        self.min_point = min_point
        self.max_point = max_point
        self.color = color
        self.linewidth = linewidth
        self.linecolor = linecolor
        self.alpha = alpha

    def draw(self, ax):
        v = np.array([[self.min_point[0], self.min_point[2], self.min_point[1]],   # v0
                      [self.max_point[0], self.min_point[2], self.min_point[1]],   # v1
                      [self.max_point[0], self.min_point[2], self.max_point[1]],   # v2
                      [self.min_point[0], self.min_point[2], self.max_point[1]],   # v3
                      [self.min_point[0], self.max_point[2], self.min_point[1]],   # v4
                      [self.max_point[0], self.max_point[2], self.min_point[1]],   # v5
                      [self.max_point[0], self.max_point[2], self.max_point[1]],   # v6
                      [self.min_point[0], self.max_point[2], self.max_point[1]]])  # v7
        f = [[v[0], v[1], v[2], v[3]],   # bottom
             [v[4], v[5], v[6], v[7]],   # top
             [v[0], v[4], v[7], v[3]],   # left
             [v[1], v[5], v[6], v[2]],   # right
             [v[3], v[2], v[6], v[7]],   # front
             [v[0], v[1], v[5], v[4]]]   # back

        collection = Poly3DCollection(f, linewidths=self.linewidth, edgecolors=self.linecolor, alpha=self.alpha)
        collection.set_facecolor(self.color)
        ax.add_collection3d(collection)

def buildSBVHAABB(aabb_str, color=None, linewidth=1.0, linecolor=[0.0, 0.0, 0.0], alpha=0.1):
    min_x = float(gdb.parse_and_eval(aabb_str + '.min_.x'))
    min_y = float(gdb.parse_and_eval(aabb_str + '.min_.y'))
    min_z = float(gdb.parse_and_eval(aabb_str + '.min_.z'))

    max_x = float(gdb.parse_and_eval(aabb_str + '.max_.x'))
    max_y = float(gdb.parse_and_eval(aabb_str + '.max_.y'))
    max_z = float(gdb.parse_and_eval(aabb_str + '.max_.z'))

    return AABB(np.array([min_x, min_y, min_z]),
                np.array([max_x, max_y, max_z]),
                color,
                linewidth,
                linecolor,
                alpha)

class Triangle (object):
    def __init__(self, v0, v1, v2, color, alpha, show_aabb = False):
        self.color = color
        self.alpha = alpha
        self.show_aabb = show_aabb
        self.v = []
        self.v.extend([v0, v1, v2])

    def draw(self, ax):
        v = np.array([[self.v[0][0], self.v[0][2], self.v[0][1]],   # v0
                      [self.v[1][0], self.v[1][2], self.v[1][1]],   # v1
                      [self.v[2][0], self.v[2][2], self.v[2][1]]])  # v2

        f = [[v[0], v[1], v[2]]]

        collection = Poly3DCollection(f, linewidths=1, edgecolors='r', alpha=self.alpha)
        collection.set_facecolor(self.color)
        ax.add_collection3d(collection)

def getAABBCentroid(aabb_str):
    x = float(gdb.parse_and_eval(aabb_str + '.centroid_.x'))
    y = float(gdb.parse_and_eval(aabb_str + '.centroid_.y'))
    z = float(gdb.parse_and_eval(aabb_str + '.centroid_.z'))

    return np.array([x, y, z])

class DrawNodeAABB(gdb.Breakpoint):
    def stop (self):
        print("")

        bvh_best_cost = float(gdb.parse_and_eval('best_cost'))
        print(">>   bvh_best_cost: " + str(bvh_best_cost))
        sbvh_best_cost = float(gdb.parse_and_eval('sbvh_best_cost'))
        print(">>   sbvh_best_cost: " + str(sbvh_best_cost))



        num_references = int(gdb.parse_and_eval('node->getNumReferences()'))
        print(">>   number of refs: " + str(num_references))
        node_aabb = buildSBVHAABB('node->aabb_')

        print(">>   AABB min: " + str(node_aabb.min_point[0]) + " -- y: " + str(node_aabb.min_point[1]) + " -- z: " + str(node_aabb.min_point[2]))
        print(">>   AABB max: " + str(node_aabb.max_point[0]) + " -- y: " + str(node_aabb.max_point[1]) + " -- z: " + str(node_aabb.max_point[2]))

        x_range = (node_aabb.max_point[0] - node_aabb.min_point[0])
        y_range = (node_aabb.max_point[1] - node_aabb.min_point[1])
        z_range = (node_aabb.max_point[2] - node_aabb.min_point[2])

        print(">>   AABB range x: " + str(x_range) + " -- y: " + str(y_range) + " -- z: " + str(z_range))

        primitives = []
        for i in range(num_references):
            v0_x = float(gdb.parse_and_eval('((Triangle*)((*primitives_)[node->getActualPrimitiveId(' + str(i) + ')].get()))->v_[0].x'))
            v0_y = float(gdb.parse_and_eval('((Triangle*)((*primitives_)[node->getActualPrimitiveId(' + str(i) + ')].get()))->v_[0].y'))
            v0_z = float(gdb.parse_and_eval('((Triangle*)((*primitives_)[node->getActualPrimitiveId(' + str(i) + ')].get()))->v_[0].z'))
            v1_x = float(gdb.parse_and_eval('((Triangle*)((*primitives_)[node->getActualPrimitiveId(' + str(i) + ')].get()))->v_[1].x'))
            v1_y = float(gdb.parse_and_eval('((Triangle*)((*primitives_)[node->getActualPrimitiveId(' + str(i) + ')].get()))->v_[1].y'))
            v1_z = float(gdb.parse_and_eval('((Triangle*)((*primitives_)[node->getActualPrimitiveId(' + str(i) + ')].get()))->v_[1].z'))
            v2_x = float(gdb.parse_and_eval('((Triangle*)((*primitives_)[node->getActualPrimitiveId(' + str(i) + ')].get()))->v_[2].x'))
            v2_y = float(gdb.parse_and_eval('((Triangle*)((*primitives_)[node->getActualPrimitiveId(' + str(i) + ')].get()))->v_[2].y'))
            v2_z = float(gdb.parse_and_eval('((Triangle*)((*primitives_)[node->getActualPrimitiveId(' + str(i) + ')].get()))->v_[2].z'))

            primitives.append(Triangle(np.array([v0_x, v0_y, v0_z]),
                                       np.array([v1_x, v1_y, v1_z]),
                                       np.array([v2_x, v2_y, v2_z]),
                                       [0.75, 0.0, 0.1],
                                       0.85,
                                       False))



        if ((x_range >= y_range) and (x_range >= z_range)):
            max_range = x_range
        elif ((y_range >= x_range) and (y_range >= z_range)):
            max_range = y_range
        else:
            max_range = z_range

        max_h_range = max_range * 2.0

        node_aabb_centroid = getAABBCentroid('node->aabb_')

        x_min_lim = node_aabb_centroid[0] - max_h_range
        x_max_lim = node_aabb_centroid[0] + max_h_range
        y_min_lim = node_aabb_centroid[1] - max_h_range
        y_max_lim = node_aabb_centroid[1] + max_h_range
        z_min_lim = node_aabb_centroid[2] - max_h_range
        z_max_lim = node_aabb_centroid[2] + max_h_range

        fig = plt.figure(figsize=(6, 6))
        ax1 = fig.add_subplot(111, projection='3d')
        ax1.set_title("Path: " + str(gdb.parse_and_eval('child_side')))
        ax1.set_proj_type('persp')
        ax1.set_xlim(x_min_lim, x_max_lim)
        ax1.set_ylim(z_min_lim, z_max_lim)
        ax1.set_zlim(y_min_lim, y_max_lim)
        ax1.set_xlabel('X')
        ax1.set_ylabel('Z')
        ax1.set_zlabel('Y')
        ax1.invert_yaxis()

        node_aabb.draw(ax1)  # green

        for i in range(num_references):
            primitives[i].draw(ax1)

        plt.show()

        return False

class PrintBVHSAHCost(gdb.Breakpoint):
    def stop (self):
        i = int(gdb.parse_and_eval('i'))
        sah_cost = float(gdb.parse_and_eval('this_cost'))
        print("BVH - " + str(i) + " : " + str(sah_cost))

class PrintSBVHSAHCost(gdb.Breakpoint):
    def stop (self):
        i = int(gdb.parse_and_eval('i'))
        sah_cost = float(gdb.parse_and_eval('this_cost'))
        print("SBVH - " + str(i) + " : " + str(sah_cost))

def ShowNode():
    gdb.execute('delete')
    DrawNodeAABB('sbvh.cpp:411')
    PrintBVHSAHCost('sbvh.cpp:115')
    PrintSBVHSAHCost('sbvh.cpp:276')
    gdb.execute('run')
