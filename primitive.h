#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include "ray.h"
#include "intersection_record.h"
#include "lambertian.h"

class Primitive
{
public:

    Primitive( const Material &material ) :
        material_( material )
    { }

    virtual bool intersect( const Ray &ray,
                            IntersectionRecord &intersection_record ) = 0;

    const Material &material_;
};

#endif /* PRIMITIVE_H_ */

