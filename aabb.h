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
