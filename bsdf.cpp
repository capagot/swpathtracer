#include "bsdf.h"

BSDF::BSDF( RNG< std::uniform_real_distribution, float, std::mt19937 > &rng ) :
    rng_( rng )
{}

glm::vec3 BSDF::fr( const glm::vec3 &w_i,
                     const std::vector< glm::vec3 > &bsdf_path ) const
{
    ( void ) w_i;

    unsigned int current_bxdf = 1;

    glm::vec3 spectrum{ 0.0f };

    // The current implementation handles only materials composed by one or two layers.
    if ( bsdf_path.size() == 2 ) // if the material contains one BxDF
        spectrum = bxdf_layers_[ 0 ]->fr( bsdf_path[ 0 ], bsdf_path[ 1 ] ) /
                   bxdf_layers_[ 0 ]->surface_sampler_->getProbability( bsdf_path[ 0 ], bsdf_path[ 1 ] ) *
                   bsdf_path[ 1 ].y;
    else // if the material contains two BxDFs
    {
        //while ( current_bxdf * 2 <= bsdf_path.size() )
        //{
        //    spectrum = ( bxdf_layers_[ 1 ]->fr( -bsdf_path[ current_bxdf ], bsdf_path[ current_bxdf + 1 ] ) /
        //                 bxdf_layers_[ 1 ]->surface_sampler_->getProbability( -bsdf_path[ current_bxdf ], bsdf_path[ current_bxdf + 1 ] ) ) *
        //                 bsdf_path[ current_bxdf + 1 ].y;
        //    current_bxdf += 2;
        //}

        while ( current_bxdf * 2 <= bsdf_path.size() )
        {
            if ( current_bxdf == 1)
                spectrum = ( bxdf_layers_[ 1 ]->fr( -bsdf_path[ current_bxdf ], bsdf_path[ current_bxdf + 1 ] ) /
                             bxdf_layers_[ 1 ]->surface_sampler_->getProbability( -bsdf_path[ current_bxdf ], bsdf_path[ current_bxdf + 1 ] ) ) *
                             bsdf_path[ current_bxdf + 1 ].y;
            else
                spectrum *= ( bxdf_layers_[ 1 ]->fr( -bsdf_path[ current_bxdf ], bsdf_path[ current_bxdf + 1 ] ) /
                              bxdf_layers_[ 1 ]->surface_sampler_->getProbability( -bsdf_path[ current_bxdf ], bsdf_path[ current_bxdf + 1 ] ) ) *
                              bsdf_path[ current_bxdf + 1 ].y;

            current_bxdf += 2;
        }
    }

    return spectrum;
}

void BSDF::getNewDirection( const glm::vec3 &w_i, std::vector< glm::vec3 > &bsdf_path )
{
    int current_bxdf = 0;

    bsdf_path.push_back( w_i );

    float dir = 1.0f;

    while ( current_bxdf >= 0 )// && ( current_bxdf < bxdf_layers_.size() ) )
    {
        if ( ( bxdf_layers_[ current_bxdf ]->bxdf_type_ == BxDF::BxDFType::DIFFUSE ) ||
             ( bxdf_layers_[ current_bxdf ]->bxdf_type_ == BxDF::BxDFType::CONDUCTOR ) )
        {
            bsdf_path.push_back( bxdf_layers_[ current_bxdf ]->getNewDirection( dir * bsdf_path[ bsdf_path.size() - 1 ] ) );
            --current_bxdf;
            dir = -1.0f;
        }
        else // refractive
        {
            if ( ( dir * bsdf_path[ bsdf_path.size() - 1 ] ).y >= 0.0f ) // ray is incident on the dielectric surface
            {
                bsdf_path.push_back( bxdf_layers_[ current_bxdf ]->getNewDirection( dir * bsdf_path[ bsdf_path.size() - 1 ] ) );

                if ( bsdf_path[ bsdf_path.size() - 1 ].y >= 0.0f ) // y > 0, new ray is leaving the surface (up)
                {
                    if ( current_bxdf == 0 ) // if the current layer is the topmost one, dec layer and leave
                    {
                        --current_bxdf;
                    }
                    else // if the current layer is an inner layer, dec layer and invert ray
                    {
                        --current_bxdf;
                        dir = -1.0f;
                    }
                }
                else // y < 0, new ray is going down the brdf stack
                {
                    if ( current_bxdf == static_cast< int >( bxdf_layers_.size() ) - 1 ) // if the current layer is the bottommost one, just leave
                    {
                        current_bxdf = -1.0f;
                    }
                    else // if the current layer is an inner layer, inc layer and invert ray
                    {
                        ++current_bxdf;
                        dir = -1.0f;
                    }
                }
            }
            else // ray is coming from inside the object
            {
                bsdf_path.push_back( bxdf_layers_[ current_bxdf ]->getNewDirection( dir * bsdf_path[ bsdf_path.size() - 1 ] ) );

                if ( bsdf_path[ bsdf_path.size() - 1 ].y >= 0.0f ) // y > 0, new ray is leaving the surface (up)
                {
                    if ( current_bxdf == 0 ) // if the current layer is the topmost one, dec layer and leave
                    {
                        --current_bxdf;
                    }
                    else // if the current layer is an inner layer, dec layer and invert ray
                    {
                        --current_bxdf;
                        dir = -1.0f;
                    }
                }
                else // y < 0, new ray is going down the brdf stack (due TIR or Fresnel)
                {
                    if ( current_bxdf == static_cast< int >( bxdf_layers_.size() ) - 1 ) // if the current layer is the bottommost one, just leave
                    {
                        current_bxdf = -1.0f;
                    }
                    else // if the current layer is an inner layer, inc layer and invert ray
                    {
                        ++current_bxdf;
                        dir = -1.0f;
                    }
                }
            }
        }
    }
}
