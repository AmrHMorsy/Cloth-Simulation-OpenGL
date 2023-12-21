#include "Skybox.h"




class Collision_Detector{
    
private:
    
    std::vector<float> grass, cloth ;
    
public:
    
    Collision_Detector( std::vector<float> _grass, std::vector<float> _cloth ) ;
    bool is_colliding(Eigen::Vector3f vertex, Eigen::Vector3f tri_v1, Eigen::Vector3f tri_v2, Eigen::Vector3f tri_v3 ) ;
    Eigen::VectorXf detect_collision( Eigen::VectorXf vertices ) ;
    ~Collision_Detector() ;
    
} ;
