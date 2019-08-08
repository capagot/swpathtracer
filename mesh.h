#ifndef MESH_H
#define MESH_H

struct Mesh {
    Mesh(const std::string& filename, long unsigned int material_id) : filename_(filename), material_id_(material_id) {}

    std::string filename_;
    long unsigned int material_id_;
};

#endif  // MESH_H
