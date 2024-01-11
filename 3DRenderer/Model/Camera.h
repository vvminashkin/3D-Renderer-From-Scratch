#pragma once
#include <Eigen/Dense>
namespace renderer {
    class Camera {
    public:

    private:
        void InitPerspective();
        double r_,l_,t_,b_;//not sure what are these
        Eigen::Matrix4d perspective_matrix_ = Eigen::Matrix4d::Zero(); 
        double near_plane_distance_ = 1;
        //double far_plane_distance_ = 100; idk if needed

    };
}