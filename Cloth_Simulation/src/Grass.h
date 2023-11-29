#include "Cloth.h"
#define NUMBER_SAND_PATCHES 1


class Grass{

private:

    Parser * parser ;
    Skybox * skybox ;
    Shader * shader ;
    float time, fov ;
    progressbar * bar ;
    std::vector<float> patch_indices ;
    std::vector<unsigned int> textures ;
    std::vector<std::string> file_paths ;
    int num_progress_bars, number_sand_patches ;
    glm::mat4 models[NUMBER_SAND_PATCHES], projection, view ;
    glm::vec3 light_color, camera, light_position, camera_front ;
    unsigned int vao, vbo, ebo, shader_program, number_of_indices, model_vbo, patch_indices_vbo ;

public:

    Grass( glm::mat4 view, glm::mat4 projection, glm::vec3 c, glm::vec3 front, float fv, progressbar * b ) ;
    void set( Skybox * skybox ) ;
    void generate_buffers() ;
    void blend_albedo() ;
    void shade( unsigned int shadow_shader_program ) ;
    void shade( Shadow * shadow ) ;
    void bind_vertices( std::vector<float> vertices ) ;
    void update_time() ;
    void bind_indices( std::vector<unsigned int> indices ) ;
    void set_uniform_variables() ;
    void bind_models() ;
    void set_models() ;
    float compute_index(int i, int j) ;
    bool is_in_view( glm::vec3 ocean_position ) ;
    void update_view( glm::vec3 camera, glm::mat4 v, glm::vec3 front ) ;
    void load_textures() ;
    void activate_textures() ;
    void generate_error( std::string msg ) ;
    void update_progress_bar( int num_bars ) ;
    ~Grass() ;
} ;
