#include "sbvh.h"

SBVH::SBVH(float alpha)
    : AccelerationStructure(((alpha == std::numeric_limits<float>::infinity()) ? AccelerationStructure::Type::BVH_SAH
                                                                               : AccelerationStructure::Type::SBVH_SAH)),
      alpha_(alpha) {}

void SBVH::build() {
    if (primitives_->size() > 0) {
        root_node_ = std::make_unique<SBVHNode>();
        root_node_->primitive_ref_list_ = std::make_unique<std::vector<PrimitiveRef>>();

        for (long int i = 0; i < primitives_->size(); ++i) {
            root_node_->primitive_ref_list_->emplace_back(i, (*primitives_)[i]->getAABB(), (*primitives_)[i]->getCentroid());
            if (!i)
                root_node_->aabb_ = (*root_node_->primitive_ref_list_)[i].aabb_;
            else
                root_node_->aabb_ = root_node_->aabb_ + (*root_node_->primitive_ref_list_)[i].aabb_;
        }

        Timer timer;
        timer.start();
        splitNode(root_node_, root_node_->aabb_.getArea(), "", "none", "root");
        timer.finish();
        statistics_.build_time_ = timer.getElapsedTime();

        std::clog << "\r"; // removes progress indicator string

        dumpTree("sbvh_accel_data_dump.dat");
        collectTreeStatistics();
    }
}

bool SBVH::intersect(const Ray &ray, IntersectionRecord &intersection_record,
                     std::size_t& num_intersection_tests, std::size_t& num_intersections) const {
    intersection_record.t_ = std::numeric_limits<float>::infinity();
    num_intersection_tests = 0;
    num_intersections = 0;
    return traverse(root_node_, ray, intersection_record, num_intersection_tests, num_intersections);
}

float SBVH::SAH(std::size_t s1_size, float s1_area, std::size_t s2_size, float s2_area, float root_aabb_area) {
    if ((s1_area == std::numeric_limits<float>::infinity()) && (s1_size == 0))
        return std::numeric_limits<float>::infinity();

    if ((s2_area == std::numeric_limits<float>::infinity()) && (s2_size == 0))
        return std::numeric_limits<float>::infinity();

    return 2.0f * cost_intersec_aabb_ + ((s1_area / root_aabb_area) * s1_size * cost_intersec_tri_) +
           ((s2_area / root_aabb_area) * s2_size * cost_intersec_tri_);
}

AABB ComputeAABB(const Primitive& primitive, float p0_offset, float p1_offset, int axis) {
    return AABB(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}

void SBVH::splitNode(std::unique_ptr<SBVHNode>& node,
                     float root_aabb_area,
                     const std::string& ident,
                     const std::string& split_method,
                     const std::string& child_side) {

    if (node->getNumReferences() == 0) {
        std::clog << split_method << "\n";
        std::clog << child_side << "\n";
        throw;
    }


    ///////////////////////////////////////////////////////////////////////////
    // object split evaluation
    ///////////////////////////////////////////////////////////////////////////

    float best_cost = cost_intersec_tri_ * node->getNumReferences();
    int best_axis = -1;
    int best_event = -1;
    AABB best_left_aabb;
    AABB best_right_aabb;

    for (int axis = 0; axis < 3; ++axis) { // 0 = X axis, 1 = Y axis, 2 = Z axis
        node->sortReferences(axis);
        std::vector<AABB> left_aabb(node->getNumReferences() - 1);
        std::vector<float> left_area(node->getNumReferences() - 1);

        for (long int i = 0; i < node->getNumReferences() - 1; ++i) {
            if (i == 0) {
                left_aabb[0] = node->getReferenceAt(0).aabb_;
                left_area[0] = left_aabb[0].getArea();
            } else {
                left_aabb[i] = left_aabb[i - 1] + node->getReferenceAt(i).aabb_;
                left_area[i] = left_aabb[i].getArea();
            }
        }

        std::vector<AABB> right_aabb(node->getNumReferences() - 1);
        std::vector<float> right_area(node->getNumReferences() - 1);

        for (long int i = node->getNumReferences() - 2; i >= 0; --i) {
            if (i == node->getNumReferences() - 2) {
                right_aabb[i] = node->getReferenceAt(i + 1).aabb_;
                right_area[i] = right_aabb[i].getArea();
            } else {
                right_aabb[i] = right_aabb[i + 1] + node->getReferenceAt(i + 1).aabb_;
                right_area[i] = right_aabb[i].getArea();
            }

            float this_cost = SAH(i + 1,
                                 left_area[i],
                                 node->getNumReferences() - 1 - i,
                                 right_area[i],
                                 node->aabb_.getArea());
                                 //root_aabb_area);

            if ( child_side == "root->L->R->R->L->R->L->L->L->L->R->R")
                float dummy = this_cost;

            if (this_cost < best_cost) {
                best_left_aabb = left_aabb[i];
                best_right_aabb = right_aabb[i];
                best_cost = this_cost;
                best_event = i;
                best_axis = axis;
            }
        }
    }

    if (best_axis == -1) { // this is a leaf node
        primitives_inserted_ += node->getNumReferences();

        printProgress();

    } else {

        ///////////////////////////////////////////////////////////////////////////
        // spatial split evaluation
        ///////////////////////////////////////////////////////////////////////////

        //constexpr float kAlpha = 1.0e-5f;  // if kAlpha == 0.0f then always compute SPATIAL SPLIT
        constexpr int kBinListSize = 32;
        constexpr float kHashingEpsilon = 1e-3f; //RESTORE: 1.0e-3f;
        AABB intersection_aabb;
        float sbvh_best_cost = cost_intersec_tri_ * node->getNumReferences();
        int sbvh_best_axis = -1;
        int sbvh_best_event = -1;
        std::vector<SplitPlane> sbvh_best_split_plane_list;
        std::vector<Bin> sbvh_best_bin_list;

        if (best_left_aabb.getIntersection(best_right_aabb, intersection_aabb)) {
            if ((intersection_aabb.getArea() / root_aabb_area) > alpha_) {  // if true, compute the spatial split
                for (int axis = 0; axis < 3; ++axis) {  // 0 = X axis, 1 = Y axis, 2 = Z axis
                    std::vector<SplitPlane> split_plane_list(kBinListSize + 1);
                    std::vector<Bin> bin_list(kBinListSize);

                    float node_axis_span = node->aabb_.max_[axis] - node->aabb_.min_[axis];
                    float bin_width = node_axis_span / static_cast<float>(kBinListSize);
                    float this_cost;

                    if (bin_width > 1e-2f) {
                        // set up the split plane list
                        for (int i = 0; i <= bin_list.size(); ++i)
                            split_plane_list[i].offset_ = node->aabb_.min_[axis] + i * bin_width;

                        for (unsigned int i = 0; i < node->getNumReferences(); ++i) {
                            // compute the entry/exit point of the primitive over the bin list
                            int in_bin = static_cast<int>(((*node->primitive_ref_list_)[i].aabb_.min_[axis] - node->aabb_.min_[axis]) / (node_axis_span + kHashingEpsilon) * kBinListSize);
                            int out_bin = static_cast<int>(((*node->primitive_ref_list_)[i].aabb_.max_[axis] - node->aabb_.min_[axis]) / (node_axis_span + kHashingEpsilon) * kBinListSize);

                            assert(in_bin >= 0);
                            assert(in_bin < kBinListSize);
                            assert(out_bin >= 0);
                            assert(out_bin < kBinListSize);
                            assert(in_bin <= out_bin);

                            bin_list[in_bin].in_primitive_refs_.push_back(i);
                            bin_list[out_bin].out_primitive_refs_.push_back(i);

                            // DEBUG
                            std::vector<std::vector<glm::vec3>> poly_list;

                            // Clips the current reference against every bin it projects to and updates
                            // the corresponding bins' AABBs
                            for (int k = in_bin; k <= out_bin; ++k) {
                                AABB clipping_aabb;

                                // Builds the clipping AABB for the bin 'k'
                                switch(axis) {
                                    case 0:
                                        clipping_aabb.min_ = glm::vec3(split_plane_list[k].offset_, node->aabb_.min_.y, node->aabb_.min_.z);
                                        clipping_aabb.max_ = glm::vec3(split_plane_list[k+1].offset_, node->aabb_.max_.y, node->aabb_.max_.z);
                                        break;
                                    case 1:
                                        clipping_aabb.min_ = glm::vec3(node->aabb_.min_.x, split_plane_list[k].offset_, node->aabb_.min_.z);
                                        clipping_aabb.max_ = glm::vec3(node->aabb_.max_.x, split_plane_list[k+1].offset_, node->aabb_.max_.z);
                                        break;
                                    case 2:
                                        clipping_aabb.min_ = glm::vec3(node->aabb_.min_.x, node->aabb_.min_.y, split_plane_list[k].offset_);
                                        clipping_aabb.max_ = glm::vec3(node->aabb_.max_.x, node->aabb_.max_.y, split_plane_list[k+1].offset_);
                                        break;
                                }

                                AABB new_aabb;
                                glm::vec3 centroid;
                                std::vector<glm::vec3> dummy;

                                if ((*primitives_)[node->getActualPrimitiveId(i)]->clipAgainstAABB(clipping_aabb, dummy, new_aabb, centroid)) {
                                    poly_list.push_back(dummy);

                                    if (!bin_list[k].has_aabb_) {
                                        bin_list[k].aabb_ = new_aabb;
                                        bin_list[k].has_aabb_ = true;
                                    } else
                                        bin_list[k].aabb_ = bin_list[k].aabb_ + new_aabb;
                                }
                            }
                        }

                        // Compute AABB area to the left of each split
                        AABB aux_aabb;
                        bool is_first_aabb = true;
                        split_plane_list[0].left_area_ = std::numeric_limits<float>::infinity();
                        split_plane_list[0].num_left_prim_refs_ = 0;

                        for (int i = 1; i < split_plane_list.size(); ++i) {
                            if (bin_list[i-1].has_aabb_) {
                                if (is_first_aabb) {
                                    is_first_aabb = false;
                                    split_plane_list[i].left_aabb_ = bin_list[i-1].aabb_;
                                    split_plane_list[i].left_area_ = split_plane_list[i].left_aabb_.getArea();
                                    split_plane_list[i].num_left_prim_refs_ = bin_list[i-1].in_primitive_refs_.size();
                                } else {
                                    split_plane_list[i].left_aabb_ = split_plane_list[i-1].left_aabb_ + bin_list[i-1].aabb_;
                                    split_plane_list[i].left_area_ = split_plane_list[i].left_aabb_.getArea();
                                    split_plane_list[i].num_left_prim_refs_ = split_plane_list[i-1].num_left_prim_refs_ + bin_list[i-1].in_primitive_refs_.size();
                                }
                            } else {
                                split_plane_list[i].left_aabb_ = split_plane_list[i-1].left_aabb_;
                                split_plane_list[i].left_area_ = split_plane_list[i].left_aabb_.getArea();
                                split_plane_list[i].num_left_prim_refs_ = split_plane_list[i-1].num_left_prim_refs_;
                            }
                        }

                        // Compute AABB area to the right of each split and the corresponding SAH cost
                        is_first_aabb = true;
                        int last_plane_id = split_plane_list.size() - 1;
                        split_plane_list[last_plane_id].right_area_ = std::numeric_limits<float>::infinity();
                        split_plane_list[last_plane_id].num_right_prim_refs_ = 0;

                        for (int i = last_plane_id - 1; i >= 0; --i) {
                            if (bin_list[i].has_aabb_) {
                                if (is_first_aabb) {
                                    is_first_aabb = false;
                                    split_plane_list[i].right_aabb_ = bin_list[i].aabb_;
                                    split_plane_list[i].right_area_ = split_plane_list[i].right_aabb_.getArea();
                                    split_plane_list[i].num_right_prim_refs_ = bin_list[i].out_primitive_refs_.size();
                                } else {
                                    split_plane_list[i].right_aabb_ = split_plane_list[i+1].right_aabb_ + bin_list[i].aabb_;
                                    split_plane_list[i].right_area_ = split_plane_list[i].right_aabb_.getArea();
                                    split_plane_list[i].num_right_prim_refs_ = split_plane_list[i+1].num_right_prim_refs_ + bin_list[i].out_primitive_refs_.size();
                                }
                            } else {
                                split_plane_list[i].right_aabb_ = split_plane_list[i+1].right_aabb_;
                                split_plane_list[i].right_area_ = split_plane_list[i].right_aabb_.getArea();
                                split_plane_list[i].num_right_prim_refs_ = split_plane_list[i+1].num_right_prim_refs_;
                            }

                            this_cost = SAH(split_plane_list[i].num_left_prim_refs_,
                                            split_plane_list[i].left_aabb_.getArea(),
                                            split_plane_list[i].num_right_prim_refs_,
                                            split_plane_list[i].right_aabb_.getArea(),
                                            node->aabb_.getArea());
                                            //root_aabb_area);

                            split_plane_list[i].sah_cost_ = this_cost;

                            if ( child_side == "root->L->R->R->L->R->L->L->L->L->R->R")
                                float dummy = this_cost;

                            if (this_cost < sbvh_best_cost) {
                                sbvh_best_cost = this_cost;
                                sbvh_best_event = i;
                                sbvh_best_axis = axis;
                            }
                        }
                    }


                    // If the current axis was selected as the one that gives the
                    // best SAH cost (up to this moment, at least),
                    // save the current bin_list and split_plane_list
                    if (sbvh_best_axis == axis) {
                        sbvh_best_split_plane_list = split_plane_list;
                        sbvh_best_bin_list = bin_list;
                    }
                } // end of the loop over the three axis (now with have a best split plane!)
            } // end of the "intersection of aabbs' area > alpha_" test
        } // end of the BVH "best left/right aabbs do intersect?" test


        if (best_cost <= sbvh_best_cost) {
        //if (false) { // force for SPATIAL SPLIT
            // split according to the BVH criterion
/*            std::clog << "    - Let's apply OBJECT split... \n";*/

            ++statistics_.num_object_splits_;

            node->sortReferences(best_axis);
            node->left_node_ = std::make_unique<SBVHNode>();
            node->left_node_->primitive_ref_list_ = std::make_unique<std::vector<PrimitiveRef>>(
                node->primitive_ref_list_->begin(), node->primitive_ref_list_->begin() + best_event + 1);

            for (long int i = 0; i < node->left_node_->primitive_ref_list_->size(); ++i) {
                if (!i)
                    node->left_node_->aabb_ = (*node->left_node_->primitive_ref_list_)[i].aabb_;
                else
                    node->left_node_->aabb_ =
                        node->left_node_->aabb_ + (*node->left_node_->primitive_ref_list_)[i].aabb_;
            }

            node->left_node_->right_node_ = nullptr;
            node->left_node_->left_node_ = nullptr;
            node->right_node_ = std::make_unique<SBVHNode>();
            node->right_node_->primitive_ref_list_ = std::make_unique<std::vector<PrimitiveRef>>(
                node->primitive_ref_list_->begin() + best_event + 1, node->primitive_ref_list_->end());

            for (long int i = 0; i < node->right_node_->primitive_ref_list_->size(); ++i) {
                if (!i)
                    node->right_node_->aabb_ = (*node->right_node_->primitive_ref_list_)[i].aabb_;
                else
                    node->right_node_->aabb_ =
                        node->right_node_->aabb_ + (*node->right_node_->primitive_ref_list_)[i].aabb_;
            }

            node->right_node_->right_node_ = nullptr;
            node->right_node_->left_node_ = nullptr;

        } else {
            // split according to the SBVH criterion
            ++statistics_.num_spatial_splits_;

            //AABB clipping_aabb;
            //clipping_aabb.min_ = glm::vec3(node->aabb_.min_.x, node->aabb_.min_.y, node->aabb_.min_.z);
            //clipping_aabb.max_ = glm::vec3(node->aabb_.max_.x, node->aabb_.max_.y, node->aabb_.max_.z);

            // node->left_node_ = SBVHNode::UniquePtr(new SBVHNode());
            node->left_node_ = std::make_unique<SBVHNode>();
            node->left_node_->left_node_ = nullptr;
            node->left_node_->right_node_ = nullptr;
            node->left_node_->aabb_ = sbvh_best_split_plane_list[sbvh_best_event].left_aabb_;

            //glm::vec3 left_range = node->left_node_->aabb_.max_ - node->left_node_->aabb_.min_;
            //float min_left_range = std::min(std::min(left_range[0], left_range[1]), left_range[2]);
            //if (min_left_range < 1.0e-2f)
            //{
            //node->left_node_->aabb_.min_ = node->left_node_->aabb_.min_ - 1.0e-4f;
            //node->left_node_->aabb_.max_ = node->left_node_->aabb_.max_ + 1.0e-4f;
            //}

            // node->left_node_->primitive_ref_list_ = SBVHNode::PrimitiveRefListUniquePtr(new std::vector<PrimitiveRef>());
            node->left_node_->primitive_ref_list_ = std::make_unique<std::vector<PrimitiveRef>>();

            for (unsigned int i = 1; i <= sbvh_best_event; ++i) {
                for (unsigned int k = 0; k < sbvh_best_bin_list[i - 1].in_primitive_refs_.size(); ++k) {
                    AABB new_aabb;
                    glm::vec3 centroid;
                    std::vector<glm::vec3> dummy;

                    unsigned int ref_id = sbvh_best_bin_list[i - 1].in_primitive_refs_[k];

                    if ((*primitives_)[node->getActualPrimitiveId(ref_id)]->clipAgainstAABB(node->left_node_->aabb_, dummy, new_aabb, centroid)) {
                        node->left_node_->primitive_ref_list_->emplace_back(node->getActualPrimitiveId(ref_id),
                                                                            new_aabb,
                                                                            centroid);
                    }
                }
            }

            // node->right_node_ = SBVHNode::UniquePtr(new SBVHNode());
            node->right_node_ = std::make_unique<SBVHNode>();
            node->right_node_->left_node_ = nullptr;
            node->right_node_->right_node_ = nullptr;
            node->right_node_->aabb_ = sbvh_best_split_plane_list[sbvh_best_event].right_aabb_;
            node->right_node_->primitive_ref_list_ = std::make_unique<std::vector<PrimitiveRef>>();

            for (int i = sbvh_best_split_plane_list.size() - 2; i >= sbvh_best_event; --i) {
                for (unsigned int k = 0; k < sbvh_best_bin_list[i].out_primitive_refs_.size(); ++k) {
                    AABB new_aabb;
                    glm::vec3 centroid;
                    std::vector<glm::vec3> dummy;

                    unsigned int ref_id = sbvh_best_bin_list[i].out_primitive_refs_[k];

                    if ((*primitives_)[node->getActualPrimitiveId(ref_id)]->clipAgainstAABB(node->right_node_->aabb_, dummy, new_aabb, centroid)) {
                        node->right_node_->primitive_ref_list_->emplace_back(node->getActualPrimitiveId(ref_id),
                                                                            new_aabb,
                                                                            centroid);
                    }
                }
            }
        }

        std::string split_str;
        split_str = (best_cost <= sbvh_best_cost) ? "Object Split" : "Spatial Split";


        node->left_node_->aabb_.min_ = node->left_node_->aabb_.min_ - 1.0e-3f;
        node->left_node_->aabb_.max_ = node->left_node_->aabb_.max_ + 1.0e-3f;
        node->right_node_->aabb_.min_ = node->right_node_->aabb_.min_ - 1.0e-3f;
        node->right_node_->aabb_.max_ = node->right_node_->aabb_.max_ + 1.0e-3f;

        if ( child_side == "root->L->R->R->L->R->L->L->L->L->R->R")
            std::cout << "!!!\n";

        node->primitive_ref_list_.reset();

        splitNode(node->left_node_, root_aabb_area, ident + " .", split_str, child_side + "->L");
        splitNode(node->right_node_, root_aabb_area, ident + " .", split_str, child_side + "->R");
    }
}

bool SBVH::traverse(const std::unique_ptr<SBVHNode>& node,
                    const Ray &ray,
                    IntersectionRecord &intersection_record,
                    std::size_t& num_intersection_tests,
                    std::size_t& num_intersections) const {
    bool primitive_intersect = false;

//#define NO_LEAF_TRI_BOX
#ifdef NO_LEAF_TRI_BOX

    if (node) {
        if ((!node->left_node_) && (!node->right_node_)) { // is a leaf node
            IntersectionRecord tmp_intersection_record;

            if (node->getNumReferences() == 1) { // node contains 1 triangle
                ++num_intersection_tests;
                if (primitives_[(*node->primitive_ref_list_)[0].id_]->intersect(ray, tmp_intersection_record)) { // if intersects...
                    ++num_intersections_;
                    if ((tmp_intersection_record.t_ < intersection_record.t_) && (tmp_intersection_record.t_ > 0.0f)) {
                        intersection_record = tmp_intersection_record;
                        primitive_intersect = true;
                    }
                }
            } else { // node contains more than one triangle
                ++num_intersection_tests;
                if (node->aabb_.intersect(ray)) {
                    ++num_intersections;
                    for (std::size_t primitive_id = 0; primitive_id < node->getNumReferences(); primitive_id++) {
                        ++num_intersection_tests;
                        if (primitives_[(*node->primitive_ref_list_)[primitive_id].id_]->intersect(ray, tmp_intersection_record)) {
                            ++num_intersections;
                            if ((tmp_intersection_record.t_ < intersection_record.t_) && (tmp_intersection_record.t_ > 0.0f)) {
                                intersection_record = tmp_intersection_record;
                                primitive_intersect = true;
                            }
                        }
                    }
                }
            }
        } else { // this is not a leaf node
            ++num_intersection_tests;
            if (node->aabb_.intersect(ray)) {
                ++num_intersections;

                if (traverse(node->left_node_, ray, intersection_record, num_intersection_tests, num_intersections))
                    primitive_intersect = true;

                if (traverse(node->right_node_, ray, intersection_record, num_intersection_tests_ num_intersections))
                    primitive_intersect = true;
            }
        }
    }

#else

    if (node) {
        ++num_intersection_tests;

        if (node->aabb_.intersect(ray)) {
            ++num_intersections;

            if ((!node->left_node_) && (!node->right_node_)) { // is a leaf node
                IntersectionRecord tmp_intersection_record;

                for (std::size_t primitive_id = 0; primitive_id < node->getNumReferences(); ++primitive_id) {
                    ++num_intersection_tests;

                    if ((*primitives_)[(*node->primitive_ref_list_)[primitive_id].id_]->intersect(ray, tmp_intersection_record)) {
                        ++num_intersections;

                        if ((tmp_intersection_record.t_ > 0.0f) &&
                            (tmp_intersection_record.t_ < intersection_record.t_) &&
                            (node->aabb_.isPointInside(tmp_intersection_record.position_))) {
                           intersection_record = tmp_intersection_record;
                           primitive_intersect = true;
                        }
                    }
                }
            } else {
                if (traverse(node->left_node_, ray, intersection_record, num_intersection_tests, num_intersections))
                    primitive_intersect = true;

                if (traverse(node->right_node_, ray, intersection_record, num_intersection_tests, num_intersections))
                    primitive_intersect = true;
            }
        }
    }

#endif // NO_LEAF_TRI_BOX

    return primitive_intersect;
}

void SBVH::collectTreeStatistics() {
    std::queue<SBVHNode *> queue;
    SBVHNode *node = nullptr;
    std::vector<unsigned int> primitives(primitives_->size(), 0);

    if (root_node_) {
        queue.push(root_node_.get());
        queue.push(nullptr);

        while (!queue.empty()) {
            node = queue.front();
            queue.pop();

            if (node) {
                if ((!node->left_node_) && (!node->right_node_)) {  //the current node is a leaf node
                    ++statistics_.num_leaf_nodes_;
                    statistics_.num_references_ += node->getNumReferences();
                    if (statistics_.min_num_primitives_leaf_ > node->getNumReferences())
                        statistics_.min_num_primitives_leaf_ = node->getNumReferences();

                    if (statistics_.max_num_primitives_leaf_ < node->getNumReferences())
                        statistics_.max_num_primitives_leaf_ = node->getNumReferences();

                    for (unsigned int i = 0; i < node->getNumReferences(); ++i)
                        ++primitives[node->getActualPrimitiveId(i)];
                } else {  //the current node is a inner node
                    ++statistics_.num_inner_nodes_;
                }

                ++statistics_.num_nodes_;

                if (node->left_node_)
                    queue.push(node->left_node_.get());
                if (node->right_node_)
                    queue.push(node->right_node_.get());
            } else if (!queue.empty()) {
                queue.push(nullptr);
                ++statistics_.tree_height_;
            }
        }
    }

    for (unsigned int i = 0; i < primitives.size(); ++i)
        if (primitives[i] == 0)
            ++statistics_.num_unreferenced_primitives_;

    // std::clog << "\n";
    // std::clog << "  Scene dimensions: \n"
    //           << "    x: [" << root_node_->aabb_.min_.x << ", " << root_node_->aabb_.max_.x << "]\n"
    //           << "    y: [" << root_node_->aabb_.min_.y << ", " << root_node_->aabb_.max_.y << "]\n"
    //           << "    z: [" << root_node_->aabb_.min_.z << ", " << root_node_->aabb_.max_.z << "]\n";
    // std::clog << "  Tree height: " << statistics_.tree_height_ << "\n";
    // std::clog << "  Number of object / spatial splits: " << statistics_.num_object_splits_ << " / " << statistics_.num_spatial_splits_ << "\n";
    // std::clog << "  Number of primitives: " << statistics_.num_primitives_ << "\n";
    // std::clog << "    Min/max primitives per leaf node: "
    //           << statistics_.min_num_primitives_leaf_ << " / "
    //           << statistics_.max_num_primitives_leaf_ << "\n";
    // std::clog << "  Number of unreferenced primitives: " << statistics_.num_unreferenced_primitives_ << "\n";
    // std::clog << "  Number of nodes: " << statistics_.num_nodes_ << "\n";
    // std::clog << "    Number of inner nodes: " << statistics_.num_inner_nodes_ << "\n";
    // std::clog << "    Number of leaf nodes: " << statistics_.num_leaf_nodes_ << "\n";
}

void SBVH::dumpTree(const std::string& file_name) const {
    std::queue<SBVHNode *> queue;
    SBVHNode *node = nullptr;
    int depth = 0;
    std::ofstream output_file(file_name);

    if (!output_file) {
        std::cerr << "Error creating output file!\n";
        exit(1);
    }

    if (root_node_) {
        queue.push(root_node_.get());
        queue.push(nullptr);

        output_file << "\n";

        while (!queue.empty()) {
            node = queue.front();
            queue.pop();

            if (node) {
                if ((!node->left_node_) && (!node->right_node_))
                    output_file << "l";
                else
                    output_file << "i";

                output_file << " " << node->aabb_.min_.x << " " << node->aabb_.min_.y << " " << node->aabb_.min_.z;
                output_file << " " << node->aabb_.max_.x << " " << node->aabb_.max_.y << " " << node->aabb_.max_.z;
                output_file << "\n";

                if (node->left_node_)
                    queue.push(node->left_node_.get());
                if (node->right_node_)
                    queue.push(node->right_node_.get());

            } else if (!queue.empty()) {
                queue.push(nullptr);
                output_file << "\n";
                depth++;
            }
        }
    }
}
