#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include "ray.h"
#include "intersection_record.h"
#include "lambertian.h"

class Primitive
{
public:

    Primitive( void );

    Primitive( const Material *material );

    virtual ~Primitive( void );

    virtual bool intersect( const Ray &ray,
                            IntersectionRecord &intersection_record ) = 0;

    const Material *material_ = nullptr;
};

#endif /* PRIMITIVE_H_ */
