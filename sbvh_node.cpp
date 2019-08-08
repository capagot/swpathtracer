#include "sbvh_node.h"

std::size_t SBVHNode::getNumReferences() const { return primitive_ref_list_->size(); }

PrimitiveRef& SBVHNode::getReferenceAt(unsigned long int id) const { return (*primitive_ref_list_)[id]; }

unsigned int SBVHNode::getActualPrimitiveId(unsigned long int id) const { return (*primitive_ref_list_)[id].id_; }
