#ifndef AABB_H_
#define AABB_H_

//#include <iostream>
//#include <iomanip>

#include <glm/glm.hpp>
//#include "onb.h"
#include "ray.h"
#include "intersection_record.h"

//#include "primitive.h"

class AABB
{
public:

    AABB( void )
    {};

    AABB( const glm::vec3 &min,
          const glm::vec3 &max ) :
        min_{ min },
        max_{ max },
        centroid_{ 0.5f * ( min_ + max_ ) }
    {};

    ~AABB( void ){};

    float getArea( void ) const;

    bool getIntersection(const AABB& aabb, AABB& result_aabb) const {
        bool result = false;

        if ((aabb.min_[0] < max_[0]) && (aabb.max_[0] > min_[0]) &&
            (aabb.min_[1] < max_[1]) && (aabb.max_[1] > min_[1]) &&
            (aabb.min_[2] < max_[2]) && (aabb.max_[2] > min_[2])) {
            result = true;
            result_aabb = AABB(glm::max(min_, aabb.min_), glm::min(max_, aabb.max_));
        }

        return result;
    }

    glm::vec3 getCentroid( void ) const;

    bool intersect( const Ray &ray ) const;
                    //IntersectionRecord &intersection_record,
                    //long unsigned int &num_intersection_tests_,
                    //long unsigned int &num_intersections_ ) const;

    AABB operator + ( const AABB &rhs ) const
    {
        return AABB{ glm::min( min_, rhs.min_ ),
                     glm::max( max_, rhs.max_ ) };
    }

    glm::vec3 min_;

    glm::vec3 max_;

    glm::vec3 centroid_;

};

#endif /* AABB_H_ */
