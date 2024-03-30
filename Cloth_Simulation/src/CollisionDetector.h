#include "Skybox.h"




struct Triangle{
    float area ;
    glm::vec3 v1, v2, v3, normal, cross_product ;
    void set( glm::vec3 t1, glm::vec3 t2, glm::vec3 t3 ){
        v1 = t1 ; v2 = t2 ; v3 = t3 ;
        compute_normal() ;
        compute_area() ;
    }
    void compute_normal(){
        cross_product = glm::cross( (v2-v1), (v3-v1) ) ;
        normal = glm::normalize( cross_product ) ;
    }
    void compute_area(){
        area = 0.5 * glm::length( cross_product ) ;
    }
} ;


class CollisionDetector{
    
private:
    
    float offset ;
    Eigen::VectorXf vertices ;
    std::vector<float> triangles ;
    
public:
    
    CollisionDetector() ;
    void set( Eigen::VectorXf v, std::vector<float> t ) ;
    void detect_collision() ;
    bool is_self_collision(glm::vec3 vertex, Triangle triangle) ;
    bool is_collision( glm::vec3 vertex, Triangle triangle ) ;
    Eigen::VectorXf get_vertices() ;
    ~CollisionDetector() ;
    
} ;
