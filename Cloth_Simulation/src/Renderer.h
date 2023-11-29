#include "Camera.h"



class Renderer{
    
private:
    
    bool done ; 
    int frames ;
    Roof * roof ;
    Light * light ;
    Grass * grass ;
    Cloth * cloth ;
    Skybox* skybox ;
    Camera * camera ;
    Shadow * shadow ; 
    GLFWwindow * window ;
    Pole * pole ; 
    Wall * wall ; 
    Tree * tree ; 
    float FPS_time, time ;

public:
    
    Renderer( GLFWwindow * _window, progressbar * bar ) ;
    void set( cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue ) ;
    void start_game_loop() ;
    void render_shadow() ;
    void render_scene() ;
    void play_music() ;
    void play_sound_effect() ;
    void handle_events() ;
    void update_view() ;
    void calculate_FPS() ;
    void generate_error(std::string msg) ;
    ~Renderer() ;
    
} ;
