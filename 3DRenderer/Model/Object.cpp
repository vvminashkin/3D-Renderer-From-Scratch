#include "Object.h"
namespace renderer {
void renderer::BasicObject::AddTriangle(const BasicObject::Matrix3d &coordinates, size_t index) {
    meshes_[index].AddTriangle(coordinates);
}
void renderer::BasicObject::AddTriangle(const BasicObject::Matrix3d &coordinates,
                                        const Matrix3d &normals, size_t index) {
    meshes_[index].AddTriangle(coordinates, normals);
}
renderer::BasicObject::BasicObject() {
}
renderer::Mesh &renderer::BasicObject::GetMesh(size_t index) {
    return meshes_[index];
}
Iterable<BasicObject::MeshConstIterator> BasicObject::GetMesh() const {
    return Iterable<MeshConstIterator>(meshes_.begin(), meshes_.end());
}

void renderer::BasicObject::AddMesh(RGB ambient, RGB diffuse, RGB specular) {
    meshes_.emplace_back(
        [ambient](const Triangle &, const Vector3d &b_coordinates) { return ambient; },

        [diffuse](const Triangle &, const Vector3d &b_coordinates) { return diffuse; },
        [specular](const Triangle &, const Vector3d &b_coordinates) { return specular; });
}

Iterable<BasicObject::MeshConstIterator> renderer::Sphere::GetMesh() const {
    return Iterable<MeshConstIterator>(meshes_.begin(), meshes_.end());
}
renderer::Sphere::Sphere(RGB ambient, RGB diffuse, RGB specular, double radius,
                         size_t max_triangle_count) {
    std::list<Matrix3d> triangles = MakeTetrahydron();
    // subdivide
    AddMesh(ambient, diffuse, specular);
    while (triangles.size() * 4 <= max_triangle_count) {
        auto it = triangles.end();
        --it;
        while (true) {
            SubDivideAndExtrude(*it, &triangles);
            auto it1 = it;

            if (it == triangles.begin()) {
                triangles.erase(it1);
                break;
            }
            --it;
            triangles.erase(it1);
        }
    }
    for (const auto &triangle : triangles) {
        meshes_[0].AddTriangle(triangle * radius, triangle);
    }
    meshes_[0].SetNormalFunction(
        [](const renderer::Triangle &tr, const Vector3d &b_coords) -> Vector3d {
            Vector3d ans = Vector3d::Zero();
            for (int k = 0; k < tr.GetVerticies().size(); ++k) {
                ans += tr.GetVerticies()[k].normal.GetCoordinates() * b_coords[k];
            }
            return ans.normalized();
        });
}

void renderer::Sphere::AddMesh(RGB ambient, RGB diffuse, RGB specular) {
    meshes_.emplace_back(
        [ambient](const Triangle &, const Vector3d &b_coordinates) { return ambient; },

        [diffuse](const Triangle &, const Vector3d &b_coordinates) { return diffuse; },

        [specular](const Triangle &, const Vector3d &b_coordinates) { return specular; });
}
std::list<Sphere::Matrix3d> Sphere::MakeTetrahydron() {
    std::list<Matrix3d> triangles;
    Matrix3d triangle;
    double phi = (1.0f + sqrt(5.0f)) * 0.5f;  // golden ratio
    double a = 1.0f;
    double b = 1.0f / phi;

    // add vertices
    Vector3d v1(0, b, -a);
    Vector3d v2(b, a, 0);
    Vector3d v3(-b, a, 0);
    Vector3d v4(0, b, a);
    Vector3d v5(0, -b, a);
    Vector3d v6(-a, 0, b);
    Vector3d v7(0, -b, -a);
    Vector3d v8(a, 0, -b);
    Vector3d v9(a, 0, b);
    Vector3d v10(-a, 0, -b);
    Vector3d v11(b, -a, 0);
    Vector3d v12(-b, -a, 0);

    // add triangles
    triangles.push_back(MakeMatrix(v3, v2, v1));
    triangles.push_back(MakeMatrix(v2, v3, v4));
    triangles.push_back(MakeMatrix(v6, v5, v4));
    triangles.push_back(MakeMatrix(v5, v9, v4));
    triangles.push_back(MakeMatrix(v8, v7, v1));
    triangles.push_back(MakeMatrix(v7, v10, v1));
    triangles.push_back(MakeMatrix(v12, v11, v5));
    triangles.push_back(MakeMatrix(v11, v12, v7));
    triangles.push_back(MakeMatrix(v10, v6, v3));
    triangles.push_back(MakeMatrix(v6, v10, v12));
    triangles.push_back(MakeMatrix(v9, v8, v2));
    triangles.push_back(MakeMatrix(v8, v9, v11));
    triangles.push_back(MakeMatrix(v3, v6, v4));
    triangles.push_back(MakeMatrix(v9, v2, v4));
    triangles.push_back(MakeMatrix(v10, v3, v1));
    triangles.push_back(MakeMatrix(v2, v8, v1));
    triangles.push_back(MakeMatrix(v12, v10, v7));
    triangles.push_back(MakeMatrix(v8, v11, v7));
    triangles.push_back(MakeMatrix(v6, v12, v5));
    triangles.push_back(MakeMatrix(v11, v9, v5));
    for (auto &triangle : triangles) {
        Vector3d normal =
            (triangle.row(0) - triangle.row(1)).cross(triangle.row(0) - triangle.row(2));
        normal = -normal;
        if (normal.dot(triangle.row(0)) < 0) {
            Vector3d r1 = triangle.row(0);
            Vector3d r2 = triangle.row(1);
            triangle.row(0) = r2;
            triangle.row(1) = r1;
        }
        for (size_t i = 0; i < 3; ++i) {
            triangle.row(i).normalize();
        }
    }

    return triangles;
}
void Sphere::SubDivideAndExtrude(Matrix3d triangle, std::list<Matrix3d> *triangles) {
    Vector3d middle01 = Vector3d{0.5, 0.5, 0}.transpose() * triangle;
    Vector3d middle12 = Vector3d{0, 0.5, 0.5}.transpose() * triangle;
    Vector3d middle02 = Vector3d{0.5, 0., 0.5}.transpose() * triangle;
    middle01.normalize();
    middle02.normalize();
    middle12.normalize();
    Matrix3d res_triangle;
    res_triangle.row(0) = triangle.row(0);
    res_triangle.row(1) = middle01;
    res_triangle.row(2) = middle02;
    triangles->push_back(res_triangle);

    res_triangle.row(0) = middle01;
    res_triangle.row(1) = triangle.row(1);
    res_triangle.row(2) = middle12;
    triangles->push_back(res_triangle);

    res_triangle.row(0) = middle01;
    res_triangle.row(1) = middle12;
    res_triangle.row(2) = middle02;
    triangles->push_back(res_triangle);

    res_triangle.row(0) = middle02;
    res_triangle.row(1) = middle12;
    res_triangle.row(2) = triangle.row(2);
    triangles->push_back(res_triangle);
}
Sphere::Matrix3d Sphere::MakeMatrix(Vector3d a, Vector3d b, Vector3d c) {
    Matrix3d ans;
    ans.row(0) = a;
    ans.row(1) = b;
    ans.row(2) = c;
    return ans;
}
}  // namespace renderer
