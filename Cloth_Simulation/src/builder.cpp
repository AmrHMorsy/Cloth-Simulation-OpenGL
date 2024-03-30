#include "Builder.h"



Builder::Builder()
{
    initialize_GLFW() ;
    renderer = new Renderer(window, bar) ;
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0,0,scrWidth,scrHeight) ;
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS) ;
}

void Builder::initialize_GLFW()
{
    if( glfwInit() == GL_FALSE )
        generate_error( "ERROR::Failed to Initialize GLFW" ) ;
    
    set_window_attributes() ;
}

void Builder::set_window_attributes()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) ;
    
    create_window() ;
}

void Builder::create_window()
{
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);
    window = glfwCreateWindow(mode->width, mode->height, "Cloth Simulation", primary, NULL);
    glViewport(0,0,mode->width, mode->height) ;
    if( window == NULL )
        generate_error( "ERROR::Failed to Create Window" ) ;
    glfwMakeContextCurrent(window);
    glClearColor(0.00f, 0.00f, 0.00f, 1.0f );

    initialize_GLEW() ;
}

void Builder::initialize_GLEW()
{
    glewExperimental = true ;
    if( glewInit() != GLEW_OK )
        generate_error( "ERROR::Failed to Initialize GLEW" ) ;
    
    setup_progress_bar() ;
}

void Builder::setup_progress_bar()
{
    bar = new progressbar(NUM_PROGRESS_BARS) ;
    bar->set_done_char("\033[1;32m█\033[0m") ;
    bar->set_opening_bracket_char("[") ;
    bar->set_closing_bracket_char("]") ;
    std::cout << std::endl ;
    
    initialize_SDL() ;
}
 
void Builder::initialize_SDL()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        generate_error("Couldn't Initialize SDL") ;
}

void Builder::launch_game()
{
    renderer->set();
    renderer->start_game_loop() ;
}

void Builder::generate_error( std::string msg )
{
    std::cout << msg << std::endl ;
    exit(1) ;
}

Builder::~Builder()
{
    delete bar ;
    bar = NULL ;
    delete renderer ;
    renderer = NULL ;
    glfwDestroyWindow(window) ;
    glfwTerminate() ;
}
