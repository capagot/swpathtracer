#ifndef ONB_H_
#define ONB_H_

#include <glm/glm.hpp>

class ONB
{
public:

    // The default constructor creates a "standard" ONB.
    ONB( void );

    // Set up an ONB from an normalized input vector 'v' that will be assumed to be aligned to
    // the 'v' (up) vector of the ONB to be created.
    void setFromV( const glm::vec3 &v );

    // Set up an ONB from the normalized input vectors 'u' and 'w', that will be assumed to be aligned to
    // the vectors 'u' (right) and 'w' vectors of the ONB to be created.
    void setFromUW( const glm::vec3 &u,
                    const glm::vec3 &w );

    glm::mat3x3 getBasisMatrix( void ) const
    {
        return m_;
    }

    // The default ONB is the standard orthonormal basis.
    glm::vec3 u_ = { 1.0f, 0.0f, 0.0f };

    glm::vec3 v_ = { 0.0f, 1.0f, 0.0f };

    glm::vec3 w_ = { 0.0f, 0.0f, 1.0f };

private:

    // Builds 'm_' from 'u_', 'v_' and 'w_'.
    void setBasisMatrix( void );

    // The 'm_' matrix transforms a vector from the ONB to the "external" space (i.e. the space into which the ONB is defined).
    // Also, 'm_' helps to keep the code shorter by avoiding the explicit use of dot products in the case of a change of basis.
    glm::mat3x3 m_;

};

#endif /* ONB_H_ */

