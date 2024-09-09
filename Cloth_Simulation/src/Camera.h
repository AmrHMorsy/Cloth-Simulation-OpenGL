#include "Light.h"




class Camera{
    
private:
    
    int pov ;
    glm::vec3 camera_position, front, up, right, direction, camera_offset ;
    glm::mat4 projection, view, model, rotation, lookaround_rotation ;
    float a, angle, fov, lookaround_speed, hfov, near_plane, far_plane, projection_width, projection_height, z_offset ;
    
public:
    
    Camera() ;
    void look_around( bool rightwards, bool leftwards, bool upwards, bool downwards ) ;
    void move() ;
    void set_projection() ;
    void set_view() ;
    float get_fov() ;
    glm::vec3 get_camera_front() ;
    glm::vec3 get_camera_position() ;
    glm::mat4 get_view() ;
    glm::mat4 get_projection() ;
    ~Camera() ;
    
} ;
