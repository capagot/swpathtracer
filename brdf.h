/*
 * brdf.h
 *
 *  Created on: Nov 13, 2016
 *      Author: christian
 */

#ifndef BRDF_H_
#define BRDF_H_

#include<memory>

class BRDF
{
public:

    typedef std::unique_ptr< BRDF > BRDFUniquePtr;

    BRDF( void ){};

    virtual glm::dvec3 fr( const glm::dvec3 &w_i,
                           const glm::dvec3 &w_r ) const = 0;

    virtual glm::dvec3 getNewDirection( const glm::dvec3 &normal,
                                        RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) const = 0;

};

#endif /* BRDF_H_ */
