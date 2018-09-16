#include "triangle.h"

const float Triangle::kIntersectionTestEpsilon_ = 0.000001f;

Triangle::Triangle( void )
{}

Triangle::Triangle( const glm::vec3 &v0,
                    const glm::vec3 &v1,
                    const glm::vec3 &v2,
                    long unsigned int material_id ) :
        Primitive{ material_id },
        v0_{ v0 },
        v1_{ v1 },
        v2_{ v2 },
        normal_{ glm::normalize( glm::cross( v1_ - v0_, v2_ - v0_ ) ) }
{}

Triangle::Triangle( const glm::vec3 &v0,
                    const glm::vec3 &v1,
                    const glm::vec3 &v2,
                    const glm::vec3 &normal,
                    long unsigned int material_id ) :
        Primitive{ material_id },
        v0_{ v0 },
        v1_{ v1 },
        v2_{ v2 },
        normal_{ normal }
{}

bool Triangle::intersect( const Ray &ray,
                          IntersectionRecord &intersection_record  )
{
    /* Ray-triangle intersection test based on the very efficient algorithm presented in the paper:
     *
     *     "Fast, Minimum Storage Ray/Triangle Intersection".
     *     Tomas Moller and Ben Trumbore.
     *     Journal of Graphics Tools.
     *     Volume 2 Issue 1, 1997.
     *     Pages 21-28
     */

    glm::vec3 edge1 = v1_ - v0_;
    glm::vec3 edge2 = v2_ - v0_;
    glm::vec3 pvec = glm::cross( ray.direction_, edge2 );

    float det = glm::dot( edge1, pvec );

//#define CULL_TEST

#ifdef CULL_TEST // culling branch

    if ( det < Triangle::kIntersectionTestEpsilon_ )
        return false;

    glm::vec3 tvec{ ray.origin_ - v0_ };

    float u = glm::dot( tvec, pvec );

    if ( ( u < 0.0f ) || ( u > det ) )
        return false;

    glm::vec3 qvec{ glm::cross( tvec, edge1 ) };

    float v = glm::dot( ray.direction_, qvec );

    if ( ( v < 0.0f ) || ( ( u + v ) > det ) )
        return false;

    float t = glm::dot( edge2, qvec );

    float inv_det = 1.0f / det;

    t *= inv_det;
    u *= inv_det;
    v *= inv_det;

#else // non-culling branch

    if ( ( det > -Triangle::kIntersectionTestEpsilon_ ) && ( det < Triangle::kIntersectionTestEpsilon_ ) )
        return false;

    float inv_det = 1.0f / det;

    glm::vec3 tvec{ ray.origin_ - v0_ };

    float u = glm::dot( tvec, pvec ) * inv_det;

    if ( ( u < 0.0f ) || ( u > 1.0f ) )
        return false;

    glm::vec3 qvec{ glm::cross( tvec, edge1 ) };

    float v = glm::dot( ray.direction_, qvec ) * inv_det;

    if ( ( v < 0.0f ) || ( ( u + v ) > 1.0f ) )
        return false;

    float t = glm::dot( edge2, qvec ) * inv_det;
#endif

    intersection_record.t_ = t;
    intersection_record.position_ = ray.origin_ + ray.direction_ * t;
    intersection_record.normal_ = normal_;
    intersection_record.material_id_ = material_id_;

    return true;
}

/*AABB Triangle::computeSBVHAABB(float p0, float p1, int axis) {

    glm::vec3 aabb_min(std::numeric_limits<float>::infinity(),
                       std::numeric_limits<float>::infinity(),
                       std::numeric_limits<float>::infinity());

    glm::vec3 aabb_max(-std::numeric_limits<float>::infinity(),
                       -std::numeric_limits<float>::infinity(),
                       -std::numeric_limits<float>::infinity());

    if ((v0_[axis] >= p0) && (v0_[axis] <= p1)) {
        aabb_min = glm::min(aabb_min, v0_);
        aabb_max = glm::max(aabb_max, v0_);
    }

    if ((v1_[axis] >= p0) && (v1_[axis] <= p1)) {
        aabb_min = glm::min(aabb_min, v1_);
        aabb_max = glm::max(aabb_max, v1_);
    }

    if ((v2_[axis] >= p0) && (v2_[axis] <= p1)) {
        aabb_min = glm::min(aabb_min, v2_);
        aabb_max = glm::max(aabb_max, v2_);
    }

    float t;
    glm::vec3 edge;
    glm::vec3 intersec;

    // edge v0--v1 intersection test
    edge = v1_ - v0_;
    if (edge[axis] != 0.0f) {
        t = (p0 - v0_[axis]) / edge[axis];
        if ((t >= 0.0f) && (t <= 1.0f)) {
            intersec = v0_ + t * edge;
            aabb_min = glm::min(aabb_min, intersec);
            aabb_max = glm::max(aabb_max, intersec);
        }

        t = (p1 - v0_[axis]) / edge[axis];
        if ((t >= 0.0f) && (t <= 1.0f)) {
            intersec = v0_ + t * edge;
            aabb_min = glm::min(aabb_min, intersec);
            aabb_max = glm::max(aabb_max, intersec);
        }
    }

    // edge v0--v2 intersection test
    edge = v2_ - v0_;
    if (edge[axis] != 0.0f) {
        t = (p0 - v0_[axis]) / edge[axis];
        if ((t >= 0.0f) && (t <= 1.0f)) {
            intersec = v0_ + t * edge;
            aabb_min = glm::min(aabb_min, intersec);
            aabb_max = glm::max(aabb_max, intersec);
        }

        t = (p1 - v0_[axis]) / edge[axis];
        if ((t >= 0.0f) && (t <= 1.0f)) {
            intersec = v0_ + t * edge;
            aabb_min = glm::min(aabb_min, intersec);
            aabb_max = glm::max(aabb_max, intersec);
        }
    }

    // edge v1--v2 intersection test
    edge = v2_ - v1_;
    if (edge[axis] != 0.0f) {
        t = (p0 - v1_[axis]) / edge[axis];
        if ((t >= 0.0f) && (t <= 1.0f)) {
            intersec = v1_ + t * edge;
            aabb_min = glm::min(aabb_min, intersec);
            aabb_max = glm::max(aabb_max, intersec);
        }

        t = (p1 - v1_[axis]) / edge[axis];
        if ((t >= 0.0f) && (t <= 1.0f)) {
            intersec = v1_ + t * edge;
            aabb_min = glm::min(aabb_min, intersec);
            aabb_max = glm::max(aabb_max, intersec);
        }
    }

    return AABB(aabb_min, aabb_max);
}
*/

bool Triangle::computeSBVHAABB(float min_x, float max_x, float min_y, float max_y,  float min_z, float max_z,  int axis, AABB& aabb) {

    bool result = false;

    glm::vec3 min_p(std::numeric_limits<float>::infinity(),
                    std::numeric_limits<float>::infinity(),
                    std::numeric_limits<float>::infinity());

    glm::vec3 max_p(-std::numeric_limits<float>::infinity(),
                    -std::numeric_limits<float>::infinity(),
                    -std::numeric_limits<float>::infinity());

    int inner_vertex_count = 0;

    // vertex v0 is inside the AABB
    if ((v0_[0] >= min_x) && (v0_[0] <= max_x) &&
        (v0_[1] >= min_y) && (v0_[1] <= max_y) &&
        (v0_[2] >= min_z) && (v0_[2] <= max_z)) {
        min_p = glm::min(min_p, v0_);
        max_p = glm::max(max_p, v0_);
        ++inner_vertex_count;
    }

    // vertex v1 is inside the AABB
    if ((v1_[0] >= min_x) && (v1_[0] <= max_x) &&
        (v1_[1] >= min_y) && (v1_[1] <= max_y) &&
        (v1_[2] >= min_z) && (v1_[2] <= max_z)) {
        min_p = glm::min(min_p, v1_);
        max_p = glm::max(max_p, v1_);
        ++inner_vertex_count;
    }

    // vertex v2 is inside the AABB
    if ((v2_[0] >= min_x) && (v2_[0] <= max_x) &&
        (v2_[1] >= min_y) && (v2_[1] <= max_y) &&
        (v2_[2] >= min_z) && (v2_[2] <= max_z)) {
        min_p = glm::min(min_p, v2_);
        max_p = glm::max(max_p, v2_);
        ++inner_vertex_count;
    }

    if (inner_vertex_count == 3) {  // triangle completely enclosed by the AABB
        aabb.min_ = min_p;
        aabb.max_ = max_p;
        result = true;
    } else {
        std::vector<glm::vec3> vertex_list = {v0_, v1_, v2_};
        std::vector<float> min_val = {min_x, min_y, min_z};
        std::vector<float> max_val = {max_x, max_y, max_z};

        float t;
        glm::vec3 d;
        glm::vec3 ip;

        for (int i = 0; i <= 1; ++i) {
            for (int j = i+1; j <= 2; ++j) {
                glm::vec3 d = vertex_list[j] - vertex_list[i];

                if (d[0] != 0.0f) {
                    t = (min_val[0] - vertex_list[i][0]) / d[0];
                    if ((t <= 1.0f) && (t > 0.0f)) {
                        ip = vertex_list[i] + t * d;
                        if ((ip[1] >= min_y) && (ip[1] <= max_y) && (ip[2] >= min_z) && (ip[2] <= max_z)) {
                            min_p = glm::min(min_p, ip);
                            max_p = glm::max(max_p, ip);
                            aabb.min_ = min_p;
                            aabb.max_ = max_p;
                            result = true;
                        }
                    }

                    t = (max_val[0] - vertex_list[i][0]) / d[0];
                    if ((t <= 1.0f) && (t > 0.0f)) {
                        ip = vertex_list[i] + t * d;
                        if ((ip[1] >= min_y) && (ip[1] <= max_y) && (ip[2] >= min_z) && (ip[2] <= max_z)) {
                            min_p = glm::min(min_p, ip);
                            max_p = glm::max(max_p, ip);
                            aabb.min_ = min_p;
                            aabb.max_ = max_p;
                            result = true;
                        }
                    }
                }

                if (d[1] != 0.0f) {
                    t = (min_val[1] - vertex_list[i][1]) / d[1];
                    if ((t <= 1.0f) && (t > 0.0f)) {
                        ip = vertex_list[i] + t * d;
                        if ((ip[0] >= min_x) && (ip[0] <= max_x) && (ip[2] >= min_z) && (ip[2] <= max_z)) {
                            min_p = glm::min(min_p, ip);
                            max_p = glm::max(max_p, ip);
                            aabb.min_ = min_p;
                            aabb.max_ = max_p;
                            result = true;
                        }
                    }

                    t = (max_val[1] - vertex_list[i][1]) / d[1];
                    if ((t <= 1.0f) && (t > 0.0f)) {
                        ip = vertex_list[i] + t * d;
                        if ((ip[0] >= min_x) && (ip[0] <= max_x) && (ip[2] >= min_z) && (ip[2] <= max_z)) {
                            min_p = glm::min(min_p, ip);
                            max_p = glm::max(max_p, ip);
                            aabb.min_ = min_p;
                            aabb.max_ = max_p;
                            result = true;
                        }
                    }
                }

                if (d[2] != 0.0f) {
                    t = (min_val[2] - vertex_list[i][2]) / d[2];
                    if ((t <= 1.0f) && (t > 0.0f)) {
                        ip = vertex_list[i] + t * d;
                        if ((ip[0] >= min_x) && (ip[0] <= max_x) && (ip[1] >= min_y) && (ip[1] <= max_y)) {
                            min_p = glm::min(min_p, ip);
                            max_p = glm::max(max_p, ip);
                            aabb.min_ = min_p;
                            aabb.max_ = max_p;
                            result = true;
                        }
                    }

                    t = (max_val[2] - vertex_list[i][2]) / d[2];
                    if ((t <= 1.0f) && (t > 0.0f)) {
                        ip = vertex_list[i] + t * d;
                        if ((ip[0] >= min_x) && (ip[0] <= max_x) && (ip[1] >= min_y) && (ip[1] <= max_y)) {
                            min_p = glm::min(min_p, ip);
                            max_p = glm::max(max_p, ip);
                            aabb.min_ = min_p;
                            aabb.max_ = max_p;
                            result = true;
                        }
                    }
                }
            }
        }

        std::vector<std::pair<glm::vec3, glm::vec3>> aabb_edges;
        aabb_edges.push_back(std::pair<glm::vec3, glm::vec3>(glm::vec3(min_x, min_y, min_z), glm::vec3(1.0f, 0.0f, 0.0f)));
        aabb_edges.push_back(std::pair<glm::vec3, glm::vec3>(glm::vec3(min_x, max_y, min_z), glm::vec3(1.0f, 0.0f, 0.0f)));
        aabb_edges.push_back(std::pair<glm::vec3, glm::vec3>(glm::vec3(min_x, max_y, max_z), glm::vec3(1.0f, 0.0f, 0.0f)));
        aabb_edges.push_back(std::pair<glm::vec3, glm::vec3>(glm::vec3(min_x, min_y, max_z), glm::vec3(1.0f, 0.0f, 0.0f)));

        aabb_edges.push_back(std::pair<glm::vec3, glm::vec3>(glm::vec3(min_x, min_y, min_z), glm::vec3(0.0f, 1.0f, 0.0f)));
        aabb_edges.push_back(std::pair<glm::vec3, glm::vec3>(glm::vec3(max_x, min_y, min_z), glm::vec3(0.0f, 1.0f, 0.0f)));
        aabb_edges.push_back(std::pair<glm::vec3, glm::vec3>(glm::vec3(max_x, min_y, max_z), glm::vec3(0.0f, 1.0f, 0.0f)));
        aabb_edges.push_back(std::pair<glm::vec3, glm::vec3>(glm::vec3(min_x, min_y, max_z), glm::vec3(0.0f, 1.0f, 0.0f)));

        aabb_edges.push_back(std::pair<glm::vec3, glm::vec3>(glm::vec3(min_x, min_y, min_z), glm::vec3(0.0f, 0.0f, 1.0f)));
        aabb_edges.push_back(std::pair<glm::vec3, glm::vec3>(glm::vec3(max_x, min_y, min_z), glm::vec3(0.0f, 0.0f, 1.0f)));
        aabb_edges.push_back(std::pair<glm::vec3, glm::vec3>(glm::vec3(max_x, max_y, min_z), glm::vec3(0.0f, 0.0f, 1.0f)));
        aabb_edges.push_back(std::pair<glm::vec3, glm::vec3>(glm::vec3(min_x, max_y, min_z), glm::vec3(0.0f, 0.0f, 1.0f)));

        IntersectionRecord i_rec;

        for (const std::pair<glm::vec3, glm::vec3>& aabb_edge : aabb_edges) {
            if (intersect(Ray(aabb_edge.first, aabb_edge.second), i_rec)) {
                if ((i_rec.position_[0] >= min_x) && (i_rec.position_[0] <= max_x) &&
                    (i_rec.position_[1] >= min_y) && (i_rec.position_[1] <= max_y) &&
                    (i_rec.position_[2] >= min_z) && (i_rec.position_[2] <= max_z)) {
                    min_p = glm::min(min_p, i_rec.position_);
                    max_p = glm::max(max_p, i_rec.position_);
                    aabb.min_ = min_p;
                    aabb.max_ = max_p;
                    result = true;
                }
            }
        }

        return result;
    }
}

AABB Triangle::getAABB( void ) const
{
    AABB aabb;

    aabb.min_ = glm::min( glm::min( v0_, v1_ ), v2_ );
    aabb.max_ = glm::max( glm::max( v0_, v1_ ), v2_ );
    aabb.centroid_ = ( 1.0f / 3.0f ) * ( v0_ + v1_ + v2_ );
    return aabb;
}

void Triangle::printData( void ) const
{
    //std::cout << v0_[0] << ", "  << v0_[1] << ", "  << v0_[2] << "\n";
    //std::cout << v1_[0] << ", "  << v1_[1] << ", "  << v1_[2] << "\n";
    //std::cout << v2_[0] << ", "  << v2_[1] << ", "  << v2_[2] << "\n";

    // in python format
    std::cout << "primitives.append(Triangle(np.array([" << v0_[0] << ", "  << v0_[1] << ", "  << v0_[2] << "]),\n";
    std::cout << "                           np.array([" << v1_[0] << ", "  << v1_[1] << ", "  << v1_[2] << "]),\n";
    std::cout << "                           np.array([" << v2_[0] << ", "  << v2_[1] << ", "  << v2_[2] << "]),\n";
    std::cout << "                           [0.75, 0.0, 0.1],\n";
    std::cout << "                           0.85,\n";
    std::cout << "                           False))\n\n";


}
