#include "sbvh.h"

SBVH::SBVH(const std::vector<Primitive::PrimitiveUniquePtr> &primitives) : primitives_(primitives) {
    Timer t;
    t.start();

    if (primitives_.size() > 0) {
        root_node_ = SBVHNode::UniquePtr(new SBVHNode());
        root_node_->primitive_ref_list_ = SBVHNode::PrimitiveRefListUniquePtr(new std::vector<PrimitiveRef>());

        //std::cout << "--- Primitives -----------------------\n";

        for (long int i = 0; i < primitives_.size(); i++) {
            //primitives_[i]->printData();
            //std::cout << "--\n";

            root_node_->primitive_ref_list_->emplace_back(i, primitives_[i]->getAABB());

            if (!i)
                root_node_->aabb_ = (*root_node_->primitive_ref_list_)[i].aabb_;
            else
                root_node_->aabb_ = root_node_->aabb_ + (*root_node_->primitive_ref_list_)[i].aabb_;
        }

/*
        std::cout << "root AABB: " << root_node_->aabb_.min_[0] << ", "
                  << root_node_->aabb_.min_[1] << ", "
                  << root_node_->aabb_.min_[2] << " --- "
                  << root_node_->aabb_.max_[0] << ", "
                  << root_node_->aabb_.max_[1] << ", "
                  << root_node_->aabb_.max_[2] << "\n";
        std::cout << "root num primitives: " << root_node_->primitive_ref_list_->size() << "\n";
*/

        splitNode(root_node_, root_node_->aabb_.getArea(), "", "none", "root");
    }

    //dump();

    //exit(1);

    t.stop();

    std::cout << "  total SBVH construction time ......: "
              << t.getElapsedSeconds() << " sec, "
              << t.getElapsedNanoSeconds() << " nsec.";
}

bool SBVH::intersect( const Ray &ray,
                     IntersectionRecord &intersection_record,
                     long unsigned int &num_intersection_tests_,
                     long unsigned int &num_intersections_ ) const
{
    return traverse( root_node_, ray, intersection_record, num_intersection_tests_, num_intersections_ );
    //return true;
}

float SBVH::SAH(std::size_t s1_size,
                float s1_area,
                std::size_t s2_size,
                float s2_area,
                float root_aabb_area) {
    if ((s2_area == std::numeric_limits<float>::infinity()) && (s2_size == 0))
        return std::numeric_limits<float>::infinity();

    return 2.0f * cost_intersec_aabb_ + ((s1_area / root_aabb_area) * s1_size * cost_intersec_tri_) +
           ((s2_area / root_aabb_area) * s2_size * cost_intersec_tri_);
}

AABB ComputeAABB(const Primitive& primitive, float p0_offset, float p1_offset, int axis) {
    return AABB(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}

void SBVH::splitNode(SBVHNode::UniquePtr& node,
                     float root_aabb_area,
                     const std::string& ident,
                     const std::string& split_method,
                     const std::string& child_side) {

#define DUMP_TREE_DATA

    ///////////////////////////////////////////////////////////////////////////
    // object split evaluation
    ///////////////////////////////////////////////////////////////////////////

#ifdef DUMP_TREE_DATA
    std::cout << "\n" << ident << "=== " << split_method << " --> Node [ " << child_side << " ] ========================================================\n";
    std::cout <<  ident << "    num primitives: " << node->primitive_ref_list_->size() << "\n";
    std::cout <<  ident << "    aabb area: " << node->aabb_.getArea() << "\n";
    std::cout <<  ident << "    aabb: [" << node->aabb_.min_[0] << ", "
                                         << node->aabb_.min_[1] << ", "
                                         << node->aabb_.min_[2] << "] - ["
                                         << node->aabb_.max_[0] << ", "
                                         << node->aabb_.max_[1] << ", "
                                         << node->aabb_.max_[2] << "]\n";
#endif

    float best_cost = cost_intersec_tri_ * node->primitive_ref_list_->size();
    int best_axis = -1;
    int best_event = -1;

    for (int axis = 1; axis <= 3; axis++) {
        switch (axis) {
            case 1:
                std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInX);
                break;
            case 2:
                std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInY);
                break;
            case 3:
                std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInZ);
                break;
        }

        AABB aux_aabb;
        std::vector<float> left_area(node->primitive_ref_list_->size());

        for (long int i = 0; i < node->primitive_ref_list_->size(); ++i) {
            if (!i) {
                left_area[i] = std::numeric_limits<float>::infinity();
                aux_aabb = (*node->primitive_ref_list_)[i].aabb_;
            } else {
                left_area[i] = aux_aabb.getArea();
                aux_aabb = aux_aabb + (*node->primitive_ref_list_)[i].aabb_;
            }
        }

        std::vector<float> right_area(node->primitive_ref_list_->size());

        for (long int i = node->primitive_ref_list_->size() - 1; i >= 0; --i) {
            if (i == node->primitive_ref_list_->size() - 1) {
                right_area[i] = std::numeric_limits<float>::infinity();
                aux_aabb = (*node->primitive_ref_list_)[i].aabb_;
            } else {
                right_area[i] = aux_aabb.getArea();
                aux_aabb = aux_aabb + (*node->primitive_ref_list_)[i].aabb_;
            }

            float this_cost = SAH(i + 1,
                                  left_area[i],
                                  node->primitive_ref_list_->size() - 1 - i,
                                  right_area[i],
                                  root_aabb_area);

            if (this_cost < best_cost) {
                best_cost = this_cost;
                best_event = i;
                best_axis = axis;
            }
        }
    }

#ifdef DUMP_TREE_DATA
    std::cout <<  ident << "    ---- Object Split -------------------------------------\n";
    std::cout <<  ident << "        best axis ....: " << best_axis << "\n";
    std::cout <<  ident << "        best event ...: " << best_event << "\n";
    std::cout <<  ident << "        best cost ....: " << best_cost << "\n";
#endif

    ///////////////////////////////////////////////////////////////////////////
    // spatial split evaluation
    ///////////////////////////////////////////////////////////////////////////

    constexpr int kBinListSize = 32;

    float sbvh_best_cost = cost_intersec_tri_ * node->primitive_ref_list_->size();
    int sbvh_best_axis = -1;
    int sbvh_best_event = -1;

    struct SplitPlane {
        float offset_ = 0.0f;
        int left_bin_;
        int right_bin_;
        AABB left_aabb_;
        AABB right_aabb_;
        float left_area_ = 0.0f;
        float right_area_ = 0.0f;
        long int num_left_prim_refs_ = 0;
        long int num_right_prim_refs_ = 0;
        float sah_cost_ = std::numeric_limits<float>::infinity();
    };

    struct Bin {
        Bin() : aabb_(glm::vec3(0.0f, 0.0f, 0.0f),
                      glm::vec3(0.0f, 0.0f, 0.0f)) {}

        int left_split_plane_id_;
        int right_split_plane_id_;
        AABB aabb_;
        bool has_aabb_ = false;
        std::vector<long int> in_primitive_refs_;
        std::vector<long int> out_primitive_refs_;
    };

    std::vector<SplitPlane> sbvh_best_split_plane_list;
    std::vector<Bin> sbvh_best_bin_list;

    if ((node->primitive_ref_list_->size() > 32) &&
        ((node->aabb_.max_[0] - node->aabb_.min_[0]) > 0.01f) &&
        ((node->aabb_.max_[1] - node->aabb_.min_[1]) > 0.01f) &&
        ((node->aabb_.max_[2] - node->aabb_.min_[2]) > 0.01f))
   {
        for (int axis = 0; axis < 3; axis++) {
        //for (int axis = 0; axis <= 0; axis++) {


            std::vector<SplitPlane> split_plane_list(kBinListSize + 1);
            std::vector<Bin> bin_list(kBinListSize);
            float span = node->aabb_.max_[axis] - node->aabb_.min_[axis];
            float bin_width = span / static_cast<float>(kBinListSize);

            // set up the split plane list
            for (int i=0; i<=bin_list.size(); ++i) {
                split_plane_list[i].offset_ = node->aabb_.min_[axis] + i * bin_width;
                split_plane_list[i].left_bin_ = (i % (kBinListSize + 1)) - 1;
                split_plane_list[i].right_bin_ = ((i + 1) % (kBinListSize + 1)) - 1;
            }

            // set up the bin list
            for (int i=0; i<bin_list.size(); ++i) {
                bin_list[i].left_split_plane_id_ = i;
                bin_list[i].right_split_plane_id_ = i + 1;
            }

            for (long int i = 0; i < node->primitive_ref_list_->size(); ++i) {
                //std::cout << "-- " << (*node->primitive_ref_list_)[i].aabb_.min_[axis] << ", "
                //                   << (*node->primitive_ref_list_)[i].aabb_.max_[axis]
                //                   << "\n";

                // compute the entry/exit point of the primitive over the bin list
                int in_bin = static_cast<int>(((*node->primitive_ref_list_)[i].aabb_.min_[axis] - node->aabb_.min_[axis]) / (span + 0.00001f) * kBinListSize);
                int out_bin = static_cast<int>(((*node->primitive_ref_list_)[i].aabb_.max_[axis] - node->aabb_.min_[axis]) / (span + 0.00001f) * kBinListSize);

                if (in_bin < 0)
                    in_bin = 0;
                if (out_bin > (kBinListSize - 1))
                    out_bin = kBinListSize - 1;

                bin_list[in_bin].in_primitive_refs_.push_back(i);
                bin_list[out_bin].out_primitive_refs_.push_back(i);

                // update the bin's AABB
                for (int k = in_bin; k <= out_bin; ++k) {
                        AABB chopped_primitive_aabb;

                        if (axis == 0)
                            if (primitives_[(*node->primitive_ref_list_)[i].id_]->computeSBVHAABB(
                                                                           split_plane_list[bin_list[k].left_split_plane_id_].offset_,
                                                                           split_plane_list[bin_list[k].right_split_plane_id_].offset_,
                                                                           node->aabb_.min_.y,
                                                                           node->aabb_.max_.y,
                                                                           node->aabb_.min_.z,
                                                                           node->aabb_.max_.z,
                                                                           axis,
                                                                           chopped_primitive_aabb))
                                if (!bin_list[k].has_aabb_) {
                                    bin_list[k].aabb_ = chopped_primitive_aabb;
                                    bin_list[k].has_aabb_ = true;
                                } else
                                    bin_list[k].aabb_ = bin_list[k].aabb_ + chopped_primitive_aabb;

                        if (axis == 1)
                            if (primitives_[(*node->primitive_ref_list_)[i].id_]->computeSBVHAABB(
                                                                           node->aabb_.min_.x,
                                                                           node->aabb_.max_.x,
                                                                           split_plane_list[bin_list[k].left_split_plane_id_].offset_,
                                                                           split_plane_list[bin_list[k].right_split_plane_id_].offset_,
                                                                           node->aabb_.min_.z,
                                                                           node->aabb_.max_.z,
                                                                           axis,
                                                                           chopped_primitive_aabb))
                                if (!bin_list[k].has_aabb_) {
                                    bin_list[k].aabb_ = chopped_primitive_aabb;
                                    bin_list[k].has_aabb_ = true;
                                } else
                                    bin_list[k].aabb_ = bin_list[k].aabb_ + chopped_primitive_aabb;

                        if (axis == 2)
                            if (primitives_[(*node->primitive_ref_list_)[i].id_]->computeSBVHAABB(
                                                                           node->aabb_.min_.x,
                                                                           node->aabb_.max_.x,
                                                                           node->aabb_.min_.y,
                                                                           node->aabb_.max_.y,
                                                                           split_plane_list[bin_list[k].left_split_plane_id_].offset_,
                                                                           split_plane_list[bin_list[k].right_split_plane_id_].offset_,
                                                                           axis,
                                                                           chopped_primitive_aabb))
                                if (!bin_list[k].has_aabb_) {
                                    bin_list[k].aabb_ = chopped_primitive_aabb;
                                    bin_list[k].has_aabb_ = true;
                                } else
                                    bin_list[k].aabb_ = bin_list[k].aabb_ + chopped_primitive_aabb;
                }
            }

            // Compute AABB area at the left of each split
            AABB aux_aabb;
            for (int i = 0; i < split_plane_list.size(); ++i) {
                if (!i) {
                    split_plane_list[i].left_area_ = std::numeric_limits<float>::infinity();
                    split_plane_list[i].num_left_prim_refs_ = 0;
                } else {
                    if (bin_list[split_plane_list[i].left_bin_].has_aabb_)
                        if (i==1)
                            aux_aabb = bin_list[split_plane_list[i].left_bin_].aabb_;
                        else
                            aux_aabb = aux_aabb + bin_list[split_plane_list[i].left_bin_].aabb_;

                    split_plane_list[i].left_area_ = aux_aabb.getArea();
                    split_plane_list[i].left_aabb_ = aux_aabb;
                    split_plane_list[i].num_left_prim_refs_ = split_plane_list[i-1].num_left_prim_refs_ + bin_list[split_plane_list[i].left_bin_].in_primitive_refs_.size();
                }
            }

            // Compute AABB area at the right of each split
            for (int i = split_plane_list.size() - 1; i >=0 ; --i) {
                if (i == split_plane_list.size() - 1) {
                    split_plane_list[i].right_area_ = std::numeric_limits<float>::infinity();
                    split_plane_list[i].num_right_prim_refs_ = 0;
                } else {
                    if (bin_list[split_plane_list[i].right_bin_].has_aabb_)
                        if (i == (split_plane_list.size() - 2))
                            aux_aabb = bin_list[split_plane_list[i].right_bin_].aabb_;
                        else
                            aux_aabb = aux_aabb + bin_list[split_plane_list[i].right_bin_].aabb_;

                    split_plane_list[i].right_area_ = aux_aabb.getArea();
                    split_plane_list[i].right_aabb_ = aux_aabb;
                    split_plane_list[i].num_right_prim_refs_ = split_plane_list[i+1].num_right_prim_refs_ + bin_list[split_plane_list[i].right_bin_].out_primitive_refs_.size();

                    float this_cost = SAH(split_plane_list[i].num_left_prim_refs_,
                                          split_plane_list[i].left_area_,
                                          split_plane_list[i].num_right_prim_refs_,
                                          split_plane_list[i].right_area_,
                                          root_aabb_area);

                    split_plane_list[i].sah_cost_ = this_cost;

                    if (this_cost < sbvh_best_cost) {
                        sbvh_best_cost = this_cost;
                        sbvh_best_event = i;
                        sbvh_best_axis = axis;
                    }
                }
            }

            if (sbvh_best_axis == axis) {
                sbvh_best_split_plane_list = split_plane_list;
                sbvh_best_bin_list = bin_list;

                //std::cout << ">>> sbvh_best_event:" << sbvh_best_event << "\n";
            }

            //std::cout << "\n\n";
            //for (int i = 0; i < split_plane_list.size(); ++i) {
            //   std::cout << "== split plane " << i << " ====================\n";
            //   std::cout << "    area .....: " << split_plane_list[i].left_area_ << " / " << split_plane_list[i].right_area_ << "\n";
            //   std::cout << "    # prims ..: " << split_plane_list[i].num_left_prim_refs_ << " / " << split_plane_list[i].num_right_prim_refs_ << "\n";
            //   std::cout << "    SAH cost .: " << split_plane_list[i].sah_cost_ << "\n";
            //}
            //std::cout << "\n\n";

            // dump bin list content

            //for (int i = 0; i < bin_list.size(); ++i) {
            //   std::cout << "== bin " << i << " ====================\n";
            //   std::cout << "    in/out count : " << bin_list[i].in_primitive_refs_.size() << " / "
            //                                      << bin_list[i].out_primitive_refs_.size() << "\n";
            //   std::cout << "    bin begin/end : " << split_plane_list[bin_list[i].left_split_plane_id_].offset_ << " / "
            //                                       << split_plane_list[bin_list[i].right_split_plane_id_].offset_ << "\n";
            //   std::cout << "    in prim refs...: ";
            //   for (int j = 0; j < bin_list[i].in_primitive_refs_.size(); ++j) {
            //       std::cout << bin_list[i].in_primitive_refs_[j] << ", ";
            //   }
            //   std::cout << "\n";
            //   std::cout << "    out prim refs..: ";
            //   for (int j = 0; j < bin_list[i].out_primitive_refs_.size(); ++j) {
            //       std::cout << bin_list[i].out_primitive_refs_[j] << ", ";
            //   }
            //   std::cout << "\n";
            //   std::cout << "    aabb: [" << bin_list[i].aabb_.min_[0] << ", "
            //                              << bin_list[i].aabb_.min_[1] << ", "
            //                              << bin_list[i].aabb_.min_[2]
            //                              << "] - ["
            //                              << bin_list[i].aabb_.max_[0] << ", "
            //                              << bin_list[i].aabb_.max_[1] << ", "
            //                              << bin_list[i].aabb_.max_[2]
            //                              << "]\n";
            //}

            //exit(1);
        }
    }

#ifdef DUMP_TREE_DATA
    std::cout <<  ident << "    ---- Spatial Split -------------------------------------\n";
    std::cout <<  ident << "        best axis ....: " << sbvh_best_axis << "\n";
    std::cout <<  ident << "        best event ...: " << sbvh_best_event << "\n";
    std::cout <<  ident << "        best cost ....: " << sbvh_best_cost << "\n";
#endif

    ///////////////////////////////////////////////////////////////////////////
    // select the better one: object or spatial split
    ///////////////////////////////////////////////////////////////////////////

#ifdef DUMP_TREE_DATA
    std::cout << ident << "    SAH cost (BVH/SBVH) : " << best_cost << " / " << sbvh_best_cost;
    if (best_cost <= sbvh_best_cost)
        std::cout << " -- BVH wins! (go for object split)\n";
    else
        std::cout << " -- SBVH wins! (go for spatial split)\n";
#endif

    if (best_cost <= sbvh_best_cost) {
        if (best_axis == -1) { // this is a leaf node
            primitives_inserted_ += node->primitive_ref_list_->size();
            std::stringstream progress_stream;
            progress_stream << "\r  SBVH building progress ............: "
                           << std::fixed << std::setw( 6 )
                           << std::setprecision( 2 )
                           << 100.0f * static_cast< float >( primitives_inserted_ ) / primitives_.size()
                           << "%";
            std::clog << progress_stream.str();
        } else {
            switch (best_axis) {
                case 1:
                    std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInX);
                    break;
                case 2:
                    std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInY);
                    break;
                case 3:
                    std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInZ);
                    break;
            }

            node->left_node_ = SBVHNode::UniquePtr(new SBVHNode());
            node->left_node_->primitive_ref_list_ =
                SBVHNode::PrimitiveRefListUniquePtr(
                    new std::vector<PrimitiveRef>(node->primitive_ref_list_->begin(),
                                                  node->primitive_ref_list_->begin() + best_event));

            for ( long int i = 0; i < node->left_node_->primitive_ref_list_->size(); ++i) {
                if ( !i )
                    node->left_node_->aabb_ = (*node->left_node_->primitive_ref_list_)[i].aabb_;
                else
                    node->left_node_->aabb_ = node->left_node_->aabb_ + (*node->left_node_->primitive_ref_list_)[i].aabb_;
            }
            node->left_node_->right_node_ = nullptr;
            node->left_node_->left_node_ = nullptr;

            node->right_node_ = SBVHNode::UniquePtr(new SBVHNode());
            node->right_node_->primitive_ref_list_ =
                SBVHNode::PrimitiveRefListUniquePtr(
                    new std::vector<PrimitiveRef>(node->primitive_ref_list_->begin() + best_event,
                                                  node->primitive_ref_list_->end()));

            for ( long int i = 0; i < node->right_node_->primitive_ref_list_->size(); ++i) {
                if ( !i )
                    node->right_node_->aabb_ = (*node->right_node_->primitive_ref_list_)[i].aabb_;
                else
                    node->right_node_->aabb_ = node->right_node_->aabb_ + (*node->right_node_->primitive_ref_list_)[i].aabb_;
            }
            node->right_node_->right_node_ = nullptr;
            node->right_node_->left_node_ = nullptr;

            splitNode(node->left_node_, root_aabb_area, ident + " .", "Object Split", child_side + "->L");
            splitNode(node->right_node_, root_aabb_area, ident + " .", "Object Split", child_side + "->R");
        }
    } else { // Spatial split is better

        //std::cout << "+";

        if (sbvh_best_axis == -1) { // this is a leaf node
            primitives_inserted_ += node->primitive_ref_list_->size();
            std::stringstream progress_stream;
            progress_stream << "\r  SBVH building progress ............: "
                           << std::fixed << std::setw( 6 )
                           << std::setprecision( 2 )
                           << 100.0f * static_cast< float >( primitives_inserted_ ) / primitives_.size()
                           << "%";
            std::clog << progress_stream.str();
        } else {


            node->left_node_ = SBVHNode::UniquePtr(new SBVHNode());
            node->left_node_->primitive_ref_list_ = SBVHNode::PrimitiveRefListUniquePtr(new std::vector<PrimitiveRef>());

            for (int split_id = 1; split_id <= sbvh_best_event; ++split_id) {
                    for (long int prim_id = 0; prim_id < sbvh_best_bin_list[sbvh_best_split_plane_list[split_id].left_bin_].in_primitive_refs_.size(); ++prim_id) {
                        //node->left_node_->primitive_ref_list_->emplace_back(prim_id, primitives_[prim_id]->getAABB());

                        long int pid1 = sbvh_best_bin_list[sbvh_best_split_plane_list[split_id].left_bin_].in_primitive_refs_[prim_id];
                        long int pid2 = (*node->primitive_ref_list_)[pid1].id_;

                        node->left_node_->primitive_ref_list_->emplace_back(pid2, primitives_[pid2]->getAABB());

                    }
            }

            node->left_node_->aabb_ = sbvh_best_split_plane_list[sbvh_best_event].left_aabb_;

            node->left_node_->right_node_ = nullptr;
            node->left_node_->left_node_ = nullptr;

            node->right_node_ = SBVHNode::UniquePtr(new SBVHNode());
            node->right_node_->primitive_ref_list_ = SBVHNode::PrimitiveRefListUniquePtr(new std::vector<PrimitiveRef>());

            for (int split_id = sbvh_best_split_plane_list.size() - 2; split_id >= sbvh_best_event; --split_id) {
                    for (long int prim_id = 0; prim_id < sbvh_best_bin_list[sbvh_best_split_plane_list[split_id].right_bin_].out_primitive_refs_.size(); ++prim_id) {
                        //node->right_node_->primitive_ref_list_->emplace_back(prim_id, primitives_[prim_id]->getAABB());

                        long int pid1 = sbvh_best_bin_list[sbvh_best_split_plane_list[split_id].right_bin_].out_primitive_refs_[prim_id];
                        long int pid2 = (*node->primitive_ref_list_)[pid1].id_;

                        node->right_node_->primitive_ref_list_->emplace_back(pid2, primitives_[pid2]->getAABB());

                    }
            }

            node->right_node_->aabb_ = sbvh_best_split_plane_list[sbvh_best_event].right_aabb_;

            node->right_node_->right_node_ = nullptr;
            node->right_node_->left_node_ = nullptr;

            splitNode(node->left_node_, root_aabb_area, ident + " .", "Spatial Split", child_side + "->L");
            splitNode(node->right_node_, root_aabb_area, ident + " .", "Spatial Split", child_side + "->R");
        }
    }

    //exit(0);

/*    if ( best_axis == -1 ) // This is a leaf node
    {
        primitives_inserted_ += node->primitive_ref_list_->size();
        std::stringstream progress_stream;
        progress_stream << "\r  SBVH building progress ............: "
                        << std::fixed << std::setw( 6 )
                        << std::setprecision( 2 )
                        << 100.0f * static_cast< float >( primitives_inserted_ ) / primitives_.size()
                        << "%";
        std::clog << progress_stream.str();
    } else { // This is an inner node
        // Make inner node with best_axis / best_event
        switch (best_axis) {
            case 1:
                std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInX);
                break;
            case 2:
                std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInY);
                break;
            case 3:
                std::sort(node->primitive_ref_list_->begin(), node->primitive_ref_list_->end(), Comparator::sortInZ);
                break;
        }

        node->left_node_ = SBVHNode::UniquePtr(new SBVHNode());
        node->left_node_->primitive_ref_list_ =
            SBVHNode::PrimitiveRefListUniquePtr(
                new std::vector<PrimitiveRef>(node->primitive_ref_list_->begin(),
                                              node->primitive_ref_list_->begin() + best_event));

        for ( long int i = 0; i < node->left_node_->primitive_ref_list_->size(); ++i) {
            if ( !i )
                node->left_node_->aabb_ = (*node->left_node_->primitive_ref_list_)[i].aabb_;
            else
                node->left_node_->aabb_ = node->left_node_->aabb_ + (*node->left_node_->primitive_ref_list_)[i].aabb_;
        }
        node->left_node_->right_node_ = nullptr;
        node->left_node_->left_node_ = nullptr;

        node->right_node_ = SBVHNode::UniquePtr(new SBVHNode());
        node->right_node_->primitive_ref_list_ =
            SBVHNode::PrimitiveRefListUniquePtr(
                new std::vector<PrimitiveRef>(node->primitive_ref_list_->begin() + best_event,
                                              node->primitive_ref_list_->end()));

        for ( long int i = 0; i < node->right_node_->primitive_ref_list_->size(); ++i) {
            if ( !i )
                node->right_node_->aabb_ = (*node->right_node_->primitive_ref_list_)[i].aabb_;
            else
                node->right_node_->aabb_ = node->right_node_->aabb_ + (*node->right_node_->primitive_ref_list_)[i].aabb_;
        }
        node->right_node_->right_node_ = nullptr;
        node->right_node_->left_node_ = nullptr;

        splitNode(node->left_node_, root_aabb_area);
        splitNode(node->right_node_, root_aabb_area);
    }*/
}

bool SBVH::traverse(const SBVHNode::UniquePtr& node,
                    const Ray &ray,
                    IntersectionRecord &intersection_record,
                    long unsigned int &num_intersection_tests_,
                    long unsigned int &num_intersections_ ) const
{
    bool primitive_intersect = false;

#define NO_LEAF_TRI_BOX

#ifdef NO_LEAF_TRI_BOX

    if (node) {
        if ((!node->left_node_) && (!node->right_node_)) { // is a leaf node
            IntersectionRecord tmp_intersection_record;

            if (node->primitive_ref_list_->size() == 1) { // node contains 1 triangle
                num_intersection_tests_++;
                if (primitives_[(*node->primitive_ref_list_)[0].id_]->intersect(ray, tmp_intersection_record)) { // if intersects...
                    num_intersections_++;
                    if ((tmp_intersection_record.t_ < intersection_record.t_) && (tmp_intersection_record.t_ > 0.0f)) {
                        intersection_record = tmp_intersection_record;
                        primitive_intersect = true;
                    }
                }
            } else { // node contains more than one triangle
                num_intersection_tests_++;
                if (node->aabb_.intersect(ray)) {
                    num_intersections_++;
                    for (std::size_t primitive_id = 0; primitive_id < node->primitive_ref_list_->size(); primitive_id++) {
                        num_intersection_tests_++;
                        if (primitives_[(*node->primitive_ref_list_)[primitive_id].id_]->intersect(ray, tmp_intersection_record)) {
                            num_intersections_++;
                            if ((tmp_intersection_record.t_ < intersection_record.t_) && (tmp_intersection_record.t_ > 0.0f)) {
                                intersection_record = tmp_intersection_record;
                                primitive_intersect = true;
                            }
                        }
                    }
                }
            }
        } else { // this is not a leaf node
            num_intersection_tests_++;
            if (node->aabb_.intersect(ray)) {
                num_intersections_++;

                if (traverse(node->left_node_, ray, intersection_record, num_intersection_tests_, num_intersections_))
                    primitive_intersect = true;

                if (traverse(node->right_node_, ray, intersection_record, num_intersection_tests_, num_intersections_))
                    primitive_intersect = true;
            }
        }
    }

#else

    if (node) {
        num_intersection_tests_++;

        if (node->aabb_.intersect(ray)) {
            num_intersections_++;

            if ((!node->left_node_) && (!node->right_node_)) { // is a leaf node
                IntersectionRecord tmp_intersection_record;

                for (std::size_t primitive_id = 0; primitive_id < node->primitive_ref_list_->size(); primitive_id++) {
                    num_intersection_tests_++;

                    if (primitives_[(*node->primitive_ref_list_)[primitive_id].id_]->intersect(ray, tmp_intersection_record)) {
                        num_intersections_++;

                        if ((tmp_intersection_record.t_ < intersection_record.t_) && (tmp_intersection_record.t_ > 0.0f)) {
                            intersection_record = tmp_intersection_record;
                            primitive_intersect = true;
                        }
                    }
                }
            } else {
                if (traverse(node->left_node_, ray, intersection_record, num_intersection_tests_, num_intersections_))
                    primitive_intersect = true;

                if (traverse(node->right_node_, ray, intersection_record, num_intersection_tests_, num_intersections_))
                    primitive_intersect = true;
            }
        }
    }

#endif // NO_LEAF_TRI_BOX

    return primitive_intersect;
}

void SBVH::dump( void ) const {
    std::queue<SBVHNode*> queue;
    SBVHNode* node = nullptr;
    int depth = 0;

    if ( root_node_ ) {
        queue.push( root_node_.get() );
        queue.push( nullptr );

        while( !queue.empty() ) {
            node = queue.front();
            queue.pop();

            if ( node ) {
                if ( ( !node->left_node_ ) && ( !node->right_node_ ) )
                    std::cout << "l";
                else
                    std::cout << "i";

                std::cout << " " << node->aabb_.min_.x << " " << node->aabb_.min_.y << " " << node->aabb_.min_.z;
                std::cout << " " << node->aabb_.max_.x << " " << node->aabb_.max_.y << " " << node->aabb_.max_.z;
                std::cout << "\n";

                if ( node->left_node_ )
                    queue.push( node->left_node_.get() );
                if ( node->right_node_ )
                    queue.push( node->right_node_.get() );
            } else
                if ( !queue.empty() ) {
                    queue.push( nullptr );
                    std::cout << "\n";
                    depth++;
                }
        }
    }

    /*
    std::queue< SBVHNode* > queue;
    SBVHNode *node = nullptr;
    int depth = 0;

    if ( root_node_ )
    {
        queue.push( root_node_ );
        queue.push( nullptr );

        while( !queue.empty() )
        {
            node = queue.front();
            queue.pop();

            if ( node )
            {
                if ( ( !node->left_ ) && ( !node->right_ ) )
                    std::cout << "l";
                else
                    std::cout << "i";

                std::cout << " " << node->aabb_.min_.x << " " << node->aabb_.min_.y << " " << node->aabb_.min_.z;
                std::cout << " " << node->aabb_.max_.x << " " << node->aabb_.max_.y << " " << node->aabb_.max_.z;
                std::cout << "\n";

                if ( node->left_ )
                    queue.push( node->left_ );
                if ( node->right_ )
                    queue.push( node->right_ );
            }
            else
                if ( !queue.empty() )
                {
                    queue.push( nullptr );
                    std::cout << "\n";
                    depth++;
                }
        }
    }
    //*/
}
