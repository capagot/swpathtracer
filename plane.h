#ifndef PLANE_H_
#define PLANE_H_

// TODO: incorporate plane as a primitive. How to do that for a BVH?
struct Plane {
   public:
    Plane(const glm::vec3 &normal, const glm::vec3 &point) : normal_(normal),
                                                             point_(point) {};
    glm::vec3 normal_;
    glm::vec3 point_;
};

#endif /* PLANE_H_ */
