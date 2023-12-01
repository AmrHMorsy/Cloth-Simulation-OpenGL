#include "Renderer.h"
#define PROGRESS_BARS_STEP 10




Renderer::Renderer( GLFWwindow * _window, progressbar * bar): frames(0), FPS_time(0), window(_window)
{
    light = new Light(window) ;
    camera = new Camera() ;
    shadow = new Shadow( window, light->get_projection(), light->get_view() ) ;
    grass = new Grass( camera->get_view_matrix(), camera->get_projection_matrix(), camera->get_camera_position(), camera->get_camera_front(), camera->get_fov(),bar, light->get_projection(), light->get_view()) ;
    cloth = new Cloth( camera->get_view_matrix(), camera->get_projection_matrix(), camera->get_camera_position(), camera->get_camera_front(), camera->get_fov(),bar, light->get_projection(), light->get_view() ) ;
    bag = new Bag( camera->get_view_matrix(), camera->get_projection_matrix(), camera->get_camera_position(), camera->get_camera_front(), camera->get_fov(),bar) ;
    campfire = new Campfire( camera->get_view_matrix(), camera->get_projection_matrix(), camera->get_camera_position(), camera->get_camera_front(), camera->get_fov(),bar) ;
    van = new Van( camera->get_view_matrix(), camera->get_projection_matrix(), camera->get_camera_position(), camera->get_camera_front(), camera->get_fov(),bar) ;
    skybox = new Skybox( camera->get_view_matrix(), camera->get_projection_matrix(), bar) ;
    pole = new Pole( camera->get_view_matrix(), camera->get_projection_matrix(), camera->get_camera_position(), camera->get_camera_front(), camera->get_fov(),bar) ;
}

void Renderer::set( cl_context context, cl_kernel FA, cl_kernel IFT, cl_command_queue queue )
{
    grass->set(skybox) ;
    cloth->set(skybox) ;
    bag->set(skybox) ;
    pole->set(skybox) ;
    campfire->set(skybox) ;
    van->set(skybox) ;
}

void Renderer::start_game_loop()
{
    time = glfwGetTime() ;
    play_music() ;
    update_view() ;
    while( !glfwWindowShouldClose(window) ){
        render_shadow() ;
        render_scene() ; 
        glfwSwapBuffers(window);
        glfwPollEvents() ;
        handle_events() ;
        calculate_FPS() ;
    }
}

void Renderer::render_shadow()
{
    shadow->render() ;
    grass->shade(shadow->get_shader_program()) ;
    cloth->shade(shadow->get_shader_program()) ;
    bag->shade(shadow->get_shader_program()) ;
    campfire->shade(shadow->get_shader_program()) ;
    pole->shade(shadow->get_shader_program()) ;
    van->shade(shadow->get_shader_program()) ;
    shadow->reset() ;
}

void Renderer::render_scene()
{
    skybox->activate() ;
    skybox->shade() ;
    grass->shade(shadow) ;
    cloth->shade(shadow) ;
    pole->shade(shadow) ;
    bag->shade(shadow) ;
    campfire->shade(shadow) ;
    van->shade(shadow) ;
}

void Renderer::play_music()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
        generate_error("Failed to open audio") ;
    Mix_Music * music = Mix_LoadMUS("../assets/music/Wind.mp3");
    if (!music)
        generate_error("Failed to load music") ;
    Mix_VolumeMusic(MIX_MAX_VOLUME / 3) ;
    Mix_PlayMusic(music, -1) ;
    std::cout << std::endl << std::endl ;
}

static void submerge( GLFWwindow* window, double x, double y )
{
    Camera * c = static_cast<Camera*>(glfwGetWindowUserPointer(window)) ;
    c->submerge(static_cast<float>(y)) ;
}

void Renderer::handle_events()
{
    glfwSetWindowUserPointer(window, camera) ;
    glfwSetScrollCallback(window, submerge) ;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera->look_around(false,false,true,false) ;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera->look_around(false,false,false,true) ;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera->look_around(true,false,false,false) ;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera->look_around(false,true,false,false) ;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->move( glm::vec3(0.0f, 0.0f, -0.1f) ) ;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->move( glm::vec3(0.0f, 0.0f, 0.1f) ) ;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->move( glm::vec3(0.1f, 0.0f, 0.0f) ) ;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->move( glm::vec3(-0.1f, 0.0f, 0.0f) ) ;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        cloth->start_simulation() ;
    update_view() ;
}

void Renderer::update_view()
{
    grass->update_view( camera->get_camera_position(), camera->get_view_matrix(), camera->get_camera_front()) ;
    cloth->update_view( camera->get_camera_position(), camera->get_view_matrix(), camera->get_camera_front()) ;
    bag->update_view( camera->get_camera_position(), camera->get_view_matrix(), camera->get_camera_front()) ;
    campfire->update_view( camera->get_camera_position(), camera->get_view_matrix(), camera->get_camera_front()) ;
    van->update_view( camera->get_camera_position(), camera->get_view_matrix(), camera->get_camera_front()) ;
    pole->update_view( camera->get_camera_position(), camera->get_view_matrix(), camera->get_camera_front()) ;
    skybox->update_view( camera->get_view_matrix(), camera->get_camera_position()) ;
}

void Renderer::calculate_FPS()
{
    frames++ ;
    if( glfwGetTime() - static_cast<float>(FPS_time) >= 1.0 ){
        std::cout << "FPS: \033[1;31m" << frames << "\033[0m" << '\r' << std::flush ;
        frames = 0 ;
        FPS_time += 1.0 ;
    }
}

void Renderer::generate_error( std::string msg )
{
    std::cout << msg << std::endl ;
    exit(1) ;
}

Renderer::~Renderer()
{
    delete light ;
    light = NULL ;
    delete camera ;
    camera = NULL ;
    delete shadow ;
    shadow = NULL ;
    delete grass ;
    grass = NULL ;
    delete cloth ;
    cloth = NULL ;
    delete bag ;
    bag = NULL ;
    delete campfire ;
    campfire = NULL ;
    delete van ;
    van = NULL ;
    delete skybox ;
    skybox = NULL ;
    delete pole ;
    pole = NULL ;
}
