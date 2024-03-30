#include "Cloth.h"
#include "../external/Include/stb_image.h"
#define EPSILON 1.8





Cloth::Cloth( glm::mat4 v, glm::mat4 proj, glm::vec3 c, progressbar * b, glm::mat4 lightproj, glm::mat4 lightview ): num_progress_bars(10), light_position({7.19999, 2.9, -10.3}), light_color({10.0f, 10.0f, 10.0f}), view(v), projection(proj), camera(c), bar(b), lightprojection(lightproj), light_view(lightview), file_paths({"../assets/Curtains/Albedo.jpg", "../assets/Curtains/AO.jpg", "../assets/Curtains/Normal.jpg", "../assets/Curtains/Roughness.png", "../assets/Curtains/Displacement.png", "../assets/Curtains/Metallic.jpg", "../assets/Curtains/Opacity.png"})
{
    parser = new Parser( bar ) ;
    parser->parse( "../assets/Curtains/Curtains.obj" ) ;
    shader = new Shader( "../external/Shaders/Cloth/Object_vs.vs", "../external/Shaders/Cloth/Object_fs.fs") ;
    generate_buffers() ;
    load_textures() ;
    shader_program = shader->get_shader_program() ;
    glUseProgram(shader_program) ;
    model = glm::scale( glm::mat4(1.0f), glm::vec3(2.0f,2.0f,2.0f) ) ;
    bind_vertices( parser->get_vertices() ) ;
    bind_indices( parser->get_vertices_faces() ) ;
    set_uniform_variables() ;
    compute_fixed_points() ;
    cloth_simulator = new ClothSimulator( parser->get_pure_vertices(), parser->get_faces(), parser->get_vertices(), fixed_points ) ;
}

void Cloth::set( Skybox * skybox )
{
    this->skybox = skybox ;
}

void Cloth::generate_buffers()
{
    textures.resize(file_paths.size()) ;
    glGenTextures(file_paths.size(),textures.data()) ;
    glGenVertexArrays(1, &vao) ;
    glGenBuffers(1, &vbo) ;
    glGenBuffers(1, &ebo) ;
    glGenBuffers(1, &texture_indices_vbo);
    update_progress_bar(10) ;
}

void Cloth::shade( unsigned int shadow_shader_program )
{
    glBindVertexArray(vao) ;
    glUniformMatrix4fv(glGetUniformLocation(shadow_shader_program, "model"), 1, GL_FALSE, &model[0][0]) ;
    glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0 ) ;
    glBindVertexArray(0) ;
}

void Cloth::shade( Shadow * shadow )
{
    glUseProgram(shader_program) ;
    update_time() ;
    cloth_simulator->simulate() ;
    bind_vertices(cloth_simulator->get_rendering_data()) ;
    activate_textures() ;
    shadow->activate_texture() ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model[0][0]) ;
    glBindVertexArray(vao) ;
    glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0) ;
    glBindVertexArray(0) ;
}

void Cloth::bind_vertices( std::vector<float> vertices )
{
    glBindVertexArray(vao) ;
    glBindBuffer(GL_ARRAY_BUFFER, vbo) ;
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW) ;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)0) ;
    glEnableVertexAttribArray(0) ;
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (8*sizeof(float)), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0) ;
    update_progress_bar(10) ;
}

void Cloth::update_time()
{
    float deltaTime = glfwGetTime() - time ;
    time += deltaTime ;
}

void Cloth::bind_indices( std::vector<unsigned int> indices )
{
    glBindVertexArray(vao) ;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo) ;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), indices.data(), GL_DYNAMIC_DRAW) ;
    glBindVertexArray(0) ;
    number_of_indices = indices.size() ;
    update_progress_bar(8) ;
}

void Cloth::set_uniform_variables()
{
    glUniform1i(glGetUniformLocation(shader_program, "albedo_map"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "ao_map"), 1);
    glUniform1i(glGetUniformLocation(shader_program, "normal_map"), 2);
    glUniform1i(glGetUniformLocation(shader_program, "roughness_map"), 3);
    glUniform1i(glGetUniformLocation(shader_program, "displacement_map"), 4);
    glUniform1i(glGetUniformLocation(shader_program, "metallic_map"), 8) ;
    glUniform1i(glGetUniformLocation(shader_program, "opacity_map"), 9) ;
    glUniform1i(glGetUniformLocation(shader_program, "shadow_map"), 10) ;
    glUniform1i(glGetUniformLocation(shader_program, "irradiance_map"), 5) ;
    glUniform1i(glGetUniformLocation(shader_program, "prefilter_map"), 6) ;
    glUniform1i(glGetUniformLocation(shader_program, "brdf_integration_map"), 7) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, &projection[0][0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model[0][0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "light_color"), 1, &light_color[0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "light_position"), 1, &light_position[0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "light_view"), 1, GL_FALSE, &light_view[0][0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "light_projection"), 1, GL_FALSE, &lightprojection[0][0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "camera"), 1, &camera[0]) ;
    update_progress_bar(6) ;
}

void Cloth::update_view( glm::vec3 camera, glm::mat4 v )
{
    this->camera = camera ;
    glUseProgram(shader_program) ;
    glUniform3fv(glGetUniformLocation(shader_program, "camera"), 1, &camera[0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, &v[0][0]) ;
}

void Cloth::update_light( glm::vec3 position, glm::mat4 proj, glm::mat4 view, glm::vec3 color )
{
    glUseProgram(shader_program) ;
    light_position = position ;
    lightprojection = proj ;
    light_view = view ;
    light_color += color ;
    glUniform3fv(glGetUniformLocation(shader_program, "light_position"), 1, &position[0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "light_view"), 1, GL_FALSE, &view[0][0]) ;
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "light_projection"), 1, GL_FALSE, &proj[0][0]) ;
    glUniform3fv(glGetUniformLocation(shader_program, "light_color"), 1, &light_color[0]) ;
}

void Cloth::load_textures()
{
    GLenum format ;
    int w, h, nc ;
    unsigned char * data ;
    for( int i = 0 ; i < file_paths.size() ; i++ ){
        glBindTexture(GL_TEXTURE_2D, textures[i]) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) ;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) ;
        stbi_set_flip_vertically_on_load(true) ;
        data = stbi_load(file_paths[i].c_str(), &w, &h, &nc, 0);
        format = (nc==1)? GL_RED : (nc==3)? GL_RGB : (nc==4) ? GL_RGBA : GL_RGB ;
        if (data){
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data) ;
            glGenerateMipmap(GL_TEXTURE_2D) ;
        }
        else
            generate_error("Failure To Load Texture") ;
        stbi_image_free(data);
    }
    update_progress_bar(6) ;
}

void Cloth::update_progress_bar( int num_bars )
{
    if( num_bars > num_progress_bars )
        return ;
    for( int i = 0 ; i < num_bars  ; i++ )
        bar->update() ;
    num_progress_bars -= num_bars ;
}

void Cloth::activate_textures()
{
    for( int i = 0 ; i < textures.size()-2 ; i++ ){
        glActiveTexture(GL_TEXTURE0+i) ;
        glBindTexture(GL_TEXTURE_2D, textures[i]) ;
    }
    skybox->activate_irradiance() ;
    glActiveTexture(GL_TEXTURE8) ;
    glBindTexture(GL_TEXTURE_2D, textures[textures.size()-2]) ;
    glActiveTexture(GL_TEXTURE9) ;
    glBindTexture(GL_TEXTURE_2D, textures[textures.size()-1]) ;
}

void Cloth::compute_fixed_points()
{
    int j = 0 ;
    std::vector<float> pins = parser->get_vertices("../assets/Curtains/pins.obj") ;
    std::vector<float> vertices = parser->get_pure_vertices() ;
    for( int i = 0 ; i < vertices.size() ; i+=3 ){
        for( int j = 0 ; j < pins.size() ; j+=3 ){
            if( (vertices[i]==pins[j]) && (vertices[i+1]==pins[j+1]) && (vertices[i+2]==pins[j+2]) )
                fixed_points.push_back(i/3) ;
        }
    }
}

void Cloth::generate_error( std::string msg )
{
    std::cout << msg << std::endl ;
    exit(1) ;
}

Cloth::~Cloth()
{
    delete cloth_simulator ;
    cloth_simulator = NULL ;
    delete shader ;
    shader = NULL ;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(textures.size(), textures.data());
}
