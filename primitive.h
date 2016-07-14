/*
 * primitive.h
 *
 *  Created on: May 14, 2016
 *      Author: christian
 */

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include "ray.h"
#include "intersection_record.h"
#include "material.h"

class Primitive
{
public:

    //Primitive( const Spectrum &spectrum ) :
    //    spectrum_( spectrum )
    //{ }

    Primitive( const Material &material ) :
        material_( material )
    { }

    virtual bool intersect( const Ray &ray,
                            IntersectionRecord &intersection_record ) = 0;

    // TODO: this must be changed to account for BRSFs ..
    //const Spectrum &spectrum_;
    const Material &material_;
};

#endif /* PRIMITIVE_H_ */
