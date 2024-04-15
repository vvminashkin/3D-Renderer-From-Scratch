#include "../../../OBJ-Loader/Source/OBJ_Loader.h"
#include "ObjLoader.h"
#include <iostream>

namespace kernel {
using RGB = renderer::RGB;
using Vector3d = Eigen::Vector3d;
using Matrix3d = Eigen::Matrix3d;
namespace {
RGB MakeRgb(const objl::Vector3 &vec) {
    RGB ans;
    ans.SetR(vec.X);
    ans.SetG(vec.Y);
    ans.SetB(vec.Z);
    return ans;
}
Vector3d MakeVector3d(const objl::Vector3 &vec) {
    Vector3d ans;
    ans.x() = vec.X;
    ans.y() = vec.Y;
    ans.z() = vec.Z;
    return ans;
}

}  // namespace
BasicObject ReadObject(const std::string &path) {
    BasicObject ans;
    objl::Loader loader;
    std::cout << loader.LoadFile(path) << std::endl;
    std::cout << path;
    for (int i = 0; i < loader.LoadedMeshes.size(); ++i) {
        const auto &mesh = loader.LoadedMeshes[i];
        RGB ambient, diffuse, specular;
        ambient = MakeRgb(mesh.MeshMaterial.Ka);
        std::cout << mesh.MeshMaterial.Kd.X << ' ' << mesh.MeshMaterial.Kd.Y << ' '
                  << mesh.MeshMaterial.Kd.Z << std::endl;
        diffuse = MakeRgb(mesh.MeshMaterial.Kd);
        specular = MakeRgb(mesh.MeshMaterial.Ks);
        ans.AddMesh(ambient, diffuse, specular);
        for (int j = 0; j < mesh.Indices.size(); j += 3) {
            Matrix3d coords;
            coords.row(0) = MakeVector3d(mesh.Vertices[mesh.Indices[j]].Position);
            coords.row(1) = MakeVector3d(mesh.Vertices[mesh.Indices[j + 1]].Position);
            coords.row(2) = MakeVector3d(mesh.Vertices[mesh.Indices[j + 2]].Position);
            // std::cout<<coords<<std::endl;
            ans.AddTriangle(coords, i);
        }
    }
    return ans;
}
}  // namespace kernel
