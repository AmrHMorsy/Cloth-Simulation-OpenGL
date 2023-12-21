#include "Collision_Detector.h"



Collision_Detector::Collision_Detector( std::vector<float> _grass, std::vector<float> _cloth ): grass(_grass), cloth(_cloth){}

Eigen::VectorXf Collision_Detector::detect_collision( Eigen::VectorXf vertices )
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,2.2f,0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),glm::vec3(0.0f,1.0f,0.0f)) * glm::scale( glm::mat4(1.0f), glm::vec3(1.0f,1.0f,1.0f) ) ;
    for ( int i = 0 ; i < vertices.size(); i += 3) {
        glm::vec4 v(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f) ;
        v = model * v ;
        Eigen::Vector3f vertex(v.x, v.y, v.z) ;
        for (int j = 0; j < grass.size(); j += 24) {
            Eigen::Vector3f tri_v1(grass[j], grass[j+1], grass[j+2]) ;
            Eigen::Vector3f tri_v2(grass[j + 8], grass[j + 9], grass[j + 10]) ;
            Eigen::Vector3f tri_v3(grass[j + 16], grass[j + 17], grass[j + 18]) ;
            Eigen::Vector3f normal = (tri_v2 - tri_v1).cross(tri_v3 - tri_v1).normalized() ;
            float distance = normal.dot(vertex - tri_v1) ;
            if( (vertex.y()<=tri_v1.y()) || (vertex.y()<=tri_v2.y()) || (vertex.y()<=tri_v3.y()) ){
                vertex += normal * 0.1f ;
                for( int k = 0 ; k < 3 ; k++ )
                    vertices[i+k] = vertex[k] ;
            }
        }
    }
    return vertices ;
}

Collision_Detector::~Collision_Detector(){}
