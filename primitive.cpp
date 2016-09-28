/*
 * primitive.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: christian
 */

#include "primitive.h"

Primitive::Primitive( void )
{ }

Primitive::Primitive( const Material *material ) :
        material_( material )
{ }

Primitive::~Primitive( void )
{ }
