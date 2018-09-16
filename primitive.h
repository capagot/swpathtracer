#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include <memory>

#include "ray.h"
#include "intersection_record.h"
#include "lambertian.h"
#include "aabb.h"

class Primitive
{
public:

    using PrimitiveUniquePtr = std::unique_ptr< Primitive >;

    Primitive( void );

    Primitive( long unsigned int material_id );

    virtual ~Primitive( void );

    virtual bool intersect( const Ray &ray,
                            IntersectionRecord &intersection_record ) = 0;

    //virtual AABB computeSBVHAABB(float p0, float p1, int axis) = 0;
    virtual bool computeSBVHAABB(float min_x, float max_x, float min_y, float max_y,  float min_z, float max_z,  int axis, AABB& aabb) = 0;

    virtual AABB getAABB( void ) const = 0;

    virtual void printData( void ) const = 0;

    long unsigned int material_id_;
};

#endif /* PRIMITIVE_H_ */
