#include "../../../OBJ-Loader/Source/OBJ_Loader.h"
#include "ObjLoader.h"
#include <iostream>
#include <filesystem>

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
void ReadAllFromDirectory(std::string path, renderer::World *world) {
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        std::string file_path = entry.path().string();
        if (file_path.substr(file_path.size() - 4, file_path.size()) == ".obj") {
            world->AddObject(ReadObject(file_path));
        }
    }
}
BasicObject ReadObject(const std::string &path) {
    BasicObject ans;
    objl::Loader loader;
    std::cout << loader.LoadFile(path) << std::endl;
    std::cout << path;
    for (int i = 0; i < loader.LoadedMeshes.size(); ++i) {
        const auto &mesh = loader.LoadedMeshes[i];
        RGB ambient, diffuse, specular;
        ambient = MakeRgb(mesh.MeshMaterial.Ka);
        std::cout << mesh.MeshMaterial.Ka.X << ' ' << mesh.MeshMaterial.Ka.Y << ' '
                  << mesh.MeshMaterial.Ka.Z << std::endl;
        diffuse = MakeRgb(mesh.MeshMaterial.Kd);
        specular = MakeRgb(mesh.MeshMaterial.Ks);
        ans.AddMesh(ambient, diffuse, specular);
        for (int j = 0; j < mesh.Indices.size() - mesh.Indices.size() % 3; j += 3) {
            Matrix3d coords;
            coords.row(0) = MakeVector3d(mesh.Vertices[mesh.Indices[j]].Position);
            coords.row(1) = MakeVector3d(mesh.Vertices[mesh.Indices[j + 1]].Position);
            coords.row(2) = MakeVector3d(mesh.Vertices[mesh.Indices[j + 2]].Position);

            Matrix3d normals;
            normals.row(0) = MakeVector3d(mesh.Vertices[mesh.Indices[j]].Normal);
            normals.row(1) = MakeVector3d(mesh.Vertices[mesh.Indices[j + 1]].Normal);
            normals.row(2) = MakeVector3d(mesh.Vertices[mesh.Indices[j + 2]].Normal);
            ans.AddTriangle(coords, normals, i);
        }
        ans.GetMesh(i).SetNormalFunction(
            [](const renderer::Triangle &tr, const Vector3d &b_coords) -> Vector3d {
                Vector3d ans = Vector3d::Zero();
                for (int k = 0; k < tr.GetVerticies().size(); ++k) {
                    ans += tr.GetVerticies()[k].normal.GetCoordinates() * b_coords[k];
                }
                return ans.normalized();
            });
    }
    return ans;
}
}  // namespace kernel
