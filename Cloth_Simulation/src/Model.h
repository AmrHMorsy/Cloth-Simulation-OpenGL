#include "Cloth.h"





class Model{

private:

    float time ;
    Parser * parser ;
    Skybox * skybox ;
    Shader * shader ;
    progressbar * bar ;
    int num_progress_bars ;
    std::vector<unsigned int> textures ;
    std::vector<std::string> file_paths ;
    glm::vec3 light_color, camera, light_position, camera_front ;
    glm::mat4 model, projection, view, lightprojection, light_view ;
    unsigned int vao, vbo, ebo, shader_program, number_of_indices, texture_indices_vbo ;

public:

    Model( glm::mat4 v, glm::mat4 proj, glm::vec3 c, progressbar * b, std::vector<std::string> texture_paths, std::string file_path, glm::mat4 lightproj, glm::mat4 lightview ) ;
    void set( Skybox * skybox ) ;
    void generate_buffers() ;
    void shade( unsigned int shadow_shader_program ) ;
    void shade( Shadow * shadow ) ;
    void bind_vertices( std::vector<float> vertices ) ;
    void update_time() ;
    void bind_indices( std::vector<unsigned int> indices ) ;
    void set_uniform_variables() ;
    void update_view( glm::vec3 camera, glm::mat4 v ) ;
    void update_light( glm::vec3 position, glm::mat4 proj, glm::mat4 view, glm::vec3 color ) ;
    void load_textures() ;
    void update_progress_bar( int num_bars ) ;
    void activate_textures() ;
    void generate_error( std::string msg ) ;
    ~Model() ;

} ;
