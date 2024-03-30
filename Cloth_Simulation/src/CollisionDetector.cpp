#include "CollisionDetector.h"




CollisionDetector::CollisionDetector(): offset(0.001f){}

void CollisionDetector::set( Eigen::VectorXf v, std::vector<float> t )
{
    vertices = v ;
    triangles = t ;
}

void CollisionDetector::detect_collision()
{
    for( int i = 0 ; i < vertices.size() ; i+=3 ){
        for( int j = 0 ; j < triangles.size() ; j+=24 ){
            glm::vec3 vertex = glm::vec3( vertices[i], vertices[i+1], vertices[i+2] ) ;
            glm::vec3 tri_v1 = glm::vec3( triangles[j], triangles[j+1], triangles[j+2] ) ;
            glm::vec3 tri_v2 = glm::vec3( triangles[j+8], triangles[j+9], triangles[j+10] ) ;
            glm::vec3 tri_v3 = glm::vec3( triangles[j+16], triangles[j+17], triangles[j+18]) ;
            Triangle triangle ;
            triangle.set( tri_v1, tri_v2, tri_v3 ) ;
            if( !(is_self_collision(vertex, triangle)) && (is_collision(vertex, triangle)) ){
                vertex = vertex + (offset * triangle.normal) ;
                for( int k = 0 ; k < 3 ; k++ )
                    vertices[i+k] = vertex[k] ;
            }
        }
    }
}

bool CollisionDetector::is_self_collision(glm::vec3 vertex, Triangle triangle)
{
    if( (vertex==triangle.v1) || (vertex==triangle.v2) || (vertex==triangle.v3) )
        return true ;
    return false ;
}

bool CollisionDetector::is_collision( glm::vec3 vertex, Triangle triangle )
{
    Triangle VAB, VCA, VBC ;
    VAB.set( vertex, triangle.v1, triangle.v2 ) ;
    VCA.set( vertex, triangle.v3, triangle.v1 ) ;
    VBC.set( vertex, triangle.v2, triangle.v3 ) ;
    float alpha = VBC.area / triangle.area ;
    float beta = VCA.area / triangle.area ;
    float gamma = VAB.area / triangle.area ;
    if( (alpha>=0) && (alpha<=1) && (beta>=0) && (beta<=1) && (gamma>=0) && (gamma<=1) )
        return true ;
    return false ;
}

Eigen::VectorXf CollisionDetector::get_vertices()
{
    return vertices ; 
}

CollisionDetector::~CollisionDetector(){}
