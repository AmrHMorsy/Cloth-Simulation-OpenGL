#include "Model.h"




class Light{
    
private:
    
    int width, height ;
    glm::mat4 projection, view ;
    float fov, light_near_plane, light_far_plane ;
    glm::vec3 light_up, light_position, light_direction ;

public:
    
    Light( GLFWwindow * window ) ;
    void set_projection() ;
    void set_view() ;
    void move( glm::vec3 direction ) ;
    glm::mat4 get_projection() ;
    glm::vec3 get_position() ;
    glm::mat4 get_view() ;
    ~Light() ;
    
} ;
