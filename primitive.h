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

    typedef std::unique_ptr< Primitive > PrimitiveUniquePtr;

    Primitive( void );

    Primitive( long unsigned int material_id );

    virtual ~Primitive( void );

    virtual bool intersect( const Ray &ray,
                            IntersectionRecord &intersection_record ) = 0;

    virtual AABB getAABB( void ) const = 0;

    virtual void printData( void ) const = 0;

    long unsigned int material_id_;
};

#endif /* PRIMITIVE_H_ */
