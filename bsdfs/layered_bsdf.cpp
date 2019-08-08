#include "layered_bsdf.h"
#include "bsdf_sample.h"

#include <iostream>

void LayeredBSDF::addBSDF(const BSDF* bsdf) {
    bsdfs_.push_back(bsdf);
}

void LayeredBSDF::evalBSDF(const glm::vec3& local_wo, glm::vec3& bsdf, glm::vec3& reflectance, float& pdf,
                           glm::vec3& local_wi) const {
    int bsdf_id;
    glm::vec3 final_bsdf = glm::vec3(1.0f);
    glm::vec3 final_reflectance = glm::vec3(1.0f);
    float final_pdf = 1.0f;
    glm::vec3 new_local_wo = local_wo;

    if (new_local_wo.y > 0.0f)  // camera ray from outside
        bsdf_id = 0;
    else
        bsdf_id = bsdfs_.size() - 1;  // camera ray from inside

    // Loops over all material layers (all the BSDFs), accumulating the contribution of 
    // each BSDF and the probabilities of each new sample generated.
    while ((bsdf_id > -1) && (bsdf_id < static_cast<long int>(bsdfs_.size())) && (final_pdf > 0.0f)) {
        BSDFSample bsdf_sample = bsdfs_[bsdf_id]->getSample(new_local_wo);
        local_wi = bsdf_sample.sample_;
        final_pdf *= bsdf_sample.probability_;

        if (local_wi.y > 0.0f) {
            if (bsdf_id == 0)
                final_bsdf *= bsdfs_[bsdf_id]->f(new_local_wo, local_wi);
            else
                final_bsdf *= bsdfs_[bsdf_id]->f(new_local_wo, local_wi) * local_wi.y;

            final_reflectance *= bsdfs_[bsdf_id]->getReflectance(new_local_wo, local_wi);

            --bsdf_id;
        } else if ((local_wi.y < 0.0f) && (bsdfs_[bsdf_id]->getType() != BSDF::Type::OPAQUE)) {
            if (bsdf_id == static_cast<long int>(bsdfs_.size()) - 1)
                final_bsdf *= bsdfs_[bsdf_id]->f(new_local_wo, local_wi);
            else
                final_bsdf *= bsdfs_[bsdf_id]->f(new_local_wo, local_wi) * local_wi.y;

            final_reflectance *= bsdfs_[bsdf_id]->getReflectance(new_local_wo, local_wi);

            ++bsdf_id;
        } else                 // if local_wi.y == 0.0f, i.e., local_wi is tangent to the surface
            final_pdf = 0.0f;  // stop the layered bsdf traversal

        new_local_wo = -local_wi;
    }

    if (final_pdf > 0.0f) {
        pdf = final_pdf;
        bsdf = final_bsdf;
        reflectance = final_reflectance;
    } else {
        pdf = 1.0f;  // dummy value
        bsdf = glm::vec3(0.0f);
        reflectance = glm::vec3(0.0f);
    }
}
