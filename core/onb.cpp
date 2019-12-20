#include "onb.h"

ONB::ONB(const glm::vec3 &v) {
    // Sets up a right-hand ONB from an normalized input vector 'v' that
    // will be assumed to be aligned to the up vector of the ONB.
    v_ = v;

    if (fabs(v_.x) > fabs(v_.y))
        w_ = glm::normalize(glm::vec3(v_.z, 0.0f, -v_.x));
    else
        w_ = glm::normalize(glm::vec3(0.0f, -v_.z, v_.y));

    u_ = glm::cross(v_, w_);

    setBasisMatrix();
}

ONB::ONB(const glm::vec3 &u, const glm::vec3 &w) {
    // Sets up an right-hand ONB from the normalized input vectors 'u' and 'w', where
    // 'u' will be assumed to be aligned to the right vector of the ONB and 'w'
    // will be aligned to the not{up, right} vector.
    u_ = u;
    w_ = w;
    v_ = glm::cross(w_, u_);

    setBasisMatrix();
}

void ONB::setBasisMatrix(void) {
    local_to_world_matrix_[0] = u_;  // sets the column 0 of m_.
    local_to_world_matrix_[1] = v_;  // sets the column 1 of m_.
    local_to_world_matrix_[2] = w_;  // sets the column 2 of m_.

    world_to_local_matrix_ = glm::transpose(local_to_world_matrix_);
}
