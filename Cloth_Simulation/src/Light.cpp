#include "Light.h"





Light::Light( GLFWwindow * window ): light_position(glm::vec3(0.484394f, 7.5f, 0.416144f)), fov(50.0f), light_near_plane(1.0f), light_far_plane(100000.0f), light_up(glm::vec3(0.0f, 0.0f, -1.0f)), light_direction(glm::vec3(0.0f,-1.0f,0.0f))
{
    glfwGetFramebufferSize(window, &width, &height);
    set_view() ;
    set_projection() ;
}

void Light::set_projection()
{
    projection = glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH /(float)WINDOW_HEIGHT, light_near_plane, light_far_plane) ;
}

void Light::update(glm::vec3 new_position)
{
    light_direction = glm::normalize(new_position - light_position);
    light_position = new_position + glm::vec3(0.0f, 10.0f, 0.0f);
    set_view();
}

void Light::set_view()
{
    view = glm::lookAt(light_position, light_position + light_direction, light_up) ;
}

glm::mat4 Light::get_projection()
{
    return projection ;
}

glm::mat4 Light::get_view()
{
    return view ;
}

Light::~Light(){}
