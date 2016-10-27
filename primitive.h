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

    Primitive( const Material *material );

    virtual ~Primitive( void );

    virtual bool intersect( const Ray &ray,
                            IntersectionRecord &intersection_record ) = 0;

    virtual AABB getAABB( void ) const = 0;

    const Material *material_ = nullptr;
};

#endif /* PRIMITIVE_H_ */

