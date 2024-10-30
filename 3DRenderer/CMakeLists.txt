
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
#find_package(SFML )
#set_source_files_properties(tags PROPERTIES GENERATED true)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)
add_executable(${PROJECT_NAME} main.cpp 
        Model/Mesh.cpp
        Model/Vertex.cpp
        Model/Primitives.cpp
        Model/World.cpp
        Model/Camera.cpp
        Model/Renderer.cpp
        Model/Screen.cpp
        Model/Utils.cpp
        Model/Object.cpp
        Model/GraphicEngine.cpp
        View/MainScreen.cpp
        Controller/GEngineController.cpp
        Application.cpp
        Model/Light/AmbientLight.cpp
        Model/Light/PointLight.cpp
        Model/ObjLoader/ObjLoader.cpp
        #tags
)
target_link_libraries(${PROJECT_NAME} sfml-graphics )

#set(${CMAKE_PREFIX_PATH} ~/)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)

#target_link_libraries(${PROJECT_NAME} Qt5::Core)

# set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "main.out")
