
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Qt5 COMPONENTS Core REQUIRED)
set_source_files_properties(tags PROPERTIES GENERATED true)
add_custom_command(OUTPUT tags
        COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(${PROJECT_NAME} main.cpp Model/AnyObject.cpp
        Model/Mesh.cpp
        Model/Vertex.cpp
        Model/Primitives.cpp
        Model/World.cpp
        Model/Camera.cpp
        Model/Renderer.cpp
        Model/Screen.cpp
        Model/Utils.cpp
        Model/Object.cpp
        tags
)
#set(${CMAKE_PREFIX_PATH} ~/)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)

target_link_libraries(${PROJECT_NAME} Qt5::Core)
# set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "main.out")
