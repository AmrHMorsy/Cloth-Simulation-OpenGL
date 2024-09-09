#include "Camera.h"




class Renderer{
    
private:
    
    int fps, frameNumber ;
    Light * light ;
    Skybox * skybox ;
    Camera * camera ;
    Shadow * shadow ;
    Cloth * curtains ;
    GLFWwindow * window ;
    float fps_time, time ;
    Model * house, * desk, * chair, * carpet, * bookshelf, * books, * cornice, * paper ;

public:
    
    Renderer( GLFWwindow * window_, progressbar * bar) ;
    void set() ;
    void start_game_loop() ;
    void render_scene() ;
    void render_shadow() ;
    void handle_events() ;
    void update_view() ;
    void update_light() ; 
    void write_frame() ;
    void calculate_fps() ;
    void generate_error( std::string error_msg ) ;
    ~Renderer() ;

} ;
