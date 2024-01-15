
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
add_executable(${PROJECT_NAME} main.cpp Model/Objects.cpp
        Model/Mesh.cpp
        Model/Vertex.cpp
        Model/Primitives.cpp
        Model/World.cpp
        Model/Camera.cpp
        Model/Renderer.cpp
        Model/Screen.cpp
        Model/Utils.cpp
)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)

# set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "main.out")
