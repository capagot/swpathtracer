#include "primitive.h"

Primitive::Primitive( void )
{ }

Primitive::Primitive( const Material *material ) :
        material_( material )
{ }

Primitive::~Primitive( void )
{ }

