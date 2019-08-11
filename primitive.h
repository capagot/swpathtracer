#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <memory>
#include <vector>

#include "aabb.h"
#include "intersection_record.h"
#include "ray.h"

class Primitive {
   public:
    Primitive(long unsigned int material_id);
    virtual ~Primitive(void);
    virtual bool intersect(const Ray& ray, IntersectionRecord& intersection_record) = 0;
    virtual bool clipAgainstAABB(const AABB& aabb, std::vector<glm::vec3>& new_vertex_list, AABB& new_aabb,
                                 glm::vec3& centroid) = 0;
    virtual AABB getAABB() const = 0;
    virtual glm::vec3 getCentroid() const = 0;
    inline long unsigned int getMaterialID() const {
        return material_id_;
    }

   private:
    long unsigned int material_id_;
};

#endif  // PRIMITIVE_H
