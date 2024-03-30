#include "Renderer.h"
#define PROGRESS_BARS_STEP 10




Renderer::Renderer( GLFWwindow * window_, progressbar * bar): fps(0), fps_time(0), window(window_)
{
    light = new Light(window) ;
    camera = new Camera() ;
    shadow = new Shadow( window, light->get_projection(), light->get_view() ) ;
    house = new Model( camera->get_view(), camera->get_projection(), camera->get_camera_position(), bar, {"../assets/House/Albedo.png", "../assets/House/AO.jpg", "../assets/House/Normal.png", "../assets/House/Roughness.png", "../assets/House/Displacement.png", "../assets/House/Metallic.png", "../assets/House/Opacity.png"}, "../assets/House/House2.obj", light->get_projection(), light->get_view() ) ;
    curtains = new Cloth( camera->get_view(), camera->get_projection(), camera->get_camera_position(), bar, light->get_projection(), light->get_view() ) ;
    chair = new Model( camera->get_view(), camera->get_projection(), camera->get_camera_position(), bar, {"../assets/Chair/Albedo.png", "../assets/Chair/AO.png", "../assets/Chair/Normal.png", "../assets/Chair/Roughness.png", "../assets/Chair/Displacement.png", "../assets/Chair/Metallic.png", "../assets/Chair/Opacity.png"}, "../assets/Chair/chair.obj", light->get_projection(), light->get_view() ) ;
    desk = new Model(camera->get_view(),camera->get_projection(),camera->get_camera_position(),bar,{"../assets/Desk/Albedo.png", "../assets/Desk/AO.png", "../assets/Desk/Normal.png", "../assets/Desk/Roughness.png", "../assets/Desk/Displacement.png", "../assets/Desk/Metallic.png", "../assets/Desk/Opacity.png"}, "../assets/Desk/Desk.obj", light->get_projection(),light->get_view());
    carpet = new Model(camera->get_view(),camera->get_projection(),camera->get_camera_position(),bar,{"../assets/Carpet/Albedo.png", "../assets/Carpet/AO.png", "../assets/Carpet/Normal.png", "../assets/Carpet/Roughness.png", "../assets/Carpet/Displacement.png", "../assets/Carpet/Metallic.png", "../assets/Carpet/Opacity.png"}, "../assets/Carpet/Carpet.obj", light->get_projection(),light->get_view());
    bookshelf = new Model(camera->get_view(),camera->get_projection(),camera->get_camera_position(),bar,{"../assets/bookshelf/Albedo.png", "../assets/bookshelf/AO.png", "../assets/bookshelf/Normal.png", "../assets/bookshelf/Roughness.png", "../assets/bookshelf/Displacement.png", "../assets/bookshelf/Metallic.png", "../assets/bookshelf/Opacity.png"}, "../assets/bookshelf/bookshelf.obj", light->get_projection(),light->get_view()) ;
    books = new Model(camera->get_view(),camera->get_projection(),camera->get_camera_position(),bar,{"../assets/Books/Albedo.png", "../assets/Books/AO.png", "../assets/Books/Normal.png", "../assets/Books/Roughness.png", "../assets/Books/Displacement.png", "../assets/Books/Metallic.png", "../assets/Books/Opacity.png"}, "../assets/Books/Books.obj", light->get_projection(),light->get_view()) ;
    cornice = new Model(camera->get_view(),camera->get_projection(),camera->get_camera_position(),bar,{"../assets/cornice/Albedo.png", "../assets/cornice/AO.png", "../assets/cornice/Normal.png", "../assets/cornice/Roughness.png", "../assets/cornice/Displacement.png", "../assets/cornice/Metallic.png", "../assets/cornice/Opacity.png"}, "../assets/cornice/cornice.obj", light->get_projection(),light->get_view()) ;
    skybox = new Skybox( camera->get_view(), camera->get_projection(), camera->get_camera_position(), bar) ;
}

void Renderer::set()
{
    house->set(skybox) ;
    curtains->set(skybox) ;
    desk->set(skybox) ;
    chair->set(skybox) ;
    bookshelf->set(skybox) ;
    carpet->set(skybox) ;
    books->set(skybox) ;
    cornice->set(skybox) ;
}

void Renderer::start_game_loop()
{
    time = glfwGetTime() ;
    update_view() ;
    while( !glfwWindowShouldClose(window) ){
        render_shadow() ;
        render_scene() ;
        glfwSwapBuffers(window) ;
        handle_events() ;
        calculate_fps() ;
    }
}

void Renderer::render_shadow()
{
    shadow->render() ;
    house->shade(shadow->get_shader_program()) ;
//    curtains->shade(shadow->get_shader_program()) ;
    desk->shade(shadow->get_shader_program()) ;
    chair->shade(shadow->get_shader_program()) ;
//    bookshelf->shade(shadow->get_shader_program()) ;
    carpet->shade(shadow->get_shader_program()) ;
    books->shade(shadow->get_shader_program()) ;
    cornice->shade(shadow->get_shader_program()) ;
    shadow->reset() ;
}

void Renderer::render_scene()
{
    skybox->activate() ;
    skybox->render() ;
    house->shade(shadow) ;
    desk->shade(shadow) ;
    chair->shade(shadow) ;
//    bookshelf->shade(shadow) ;
    carpet->shade(shadow) ;
//    curtains->shade(shadow) ;
    books->shade(shadow) ;
    cornice->shade(shadow) ;
}

void Renderer::handle_events()
{
    glfwPollEvents() ;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        camera->look_around(false,false,true,false) ;
        update_view() ;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        camera->look_around(false,false,false,true) ;
        update_view() ;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        camera->look_around(true,false,false,false) ;
        update_view() ;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        camera->look_around(false,true,false,false) ;
        update_view() ;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera->move() ;
        update_view() ;
    }
}

void Renderer::update_view()
{
    skybox->update_view( camera->get_view(), camera->get_camera_position() ) ;
    house->update_view( camera->get_camera_position(), camera->get_view() ) ;
//    curtains->update_view( camera->get_camera_position(), camera->get_view());
    desk->update_view( camera->get_camera_position(), camera->get_view());
    chair->update_view( camera->get_camera_position(), camera->get_view());
//    bookshelf->update_view( camera->get_camera_position(), camera->get_view());
    carpet->update_view( camera->get_camera_position(), camera->get_view());
    books->update_view( camera->get_camera_position(), camera->get_view());
    cornice->update_view( camera->get_camera_position(), camera->get_view());
}

void Renderer::calculate_fps()
{
    fps++ ;
    if( glfwGetTime() - static_cast<float>(fps_time) >= 1.0 ){
        std::cout << "FPS: \033[1;31m" << fps << "\033[0m" << '\r' << std::flush ;
        fps = 0 ;
        fps_time += 1.0 ;
    }
}

void Renderer::generate_error( std::string error_msg )
{
    std::cout << "\033[1;31m" << error_msg << "\033[0m" << std::endl;
    exit(1) ;
}

Renderer::~Renderer()
{
    delete cornice ;
    cornice = NULL ;
    delete books ;
    books = NULL ; 
    delete carpet ;
    carpet = NULL ;
    delete bookshelf ;
    bookshelf = NULL ;
    delete desk ;
    desk = NULL ;
    delete chair ;
    chair = NULL ;
    delete curtains ;
    curtains = NULL ;
    delete house ;
    house = NULL ;
    delete light ;
    light = NULL ;
    delete camera ;
    camera = NULL ;
    delete skybox ;
    skybox = NULL ;
}
