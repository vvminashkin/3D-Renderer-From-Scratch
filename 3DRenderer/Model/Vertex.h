#pragma once
#include <Eigen/Dense>
namespace renderer{
   struct Vertex{

       Eigen::Vector3d coordinates;
       Eigen::Vector3d normal;
   };
}