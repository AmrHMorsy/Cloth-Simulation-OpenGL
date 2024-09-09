#include "CollisionDetector.h"





class ClothSimulator{
    
private:
    
    bool ** visited ;
    int num_iterations ;
    float gravitational_force ;
    Eigen::SparseMatrix<float> L, M, J ;
    glm::vec3 wind_force, wind_direction ;
    std::vector<unsigned int> faces, pins ;
    std::vector<std::pair<int,int>> springs ;
    Eigen::SimplicialLLT<Eigen::SparseMatrix<float>> solver ;
    std::vector<float> vertices, cloth, rest_lengths, normals ;
    float time_step, rest_length, damping_factor, mass, stiffness, wind_speed, wind_strength_amplifier, gravity_amplifier, wind_strength, restoration_strength, gravity_acceleration ;
    Eigen::VectorXf inertial_term, b, previous_state, current_state, external_force, spring_directions, restoration_force ; 
    
public:
    
    ClothSimulator( std::vector<float> _vertices, std::vector< unsigned int> _faces, std::vector<float> _cloth, std::vector<unsigned int> _pins) ;
    void define_constants() ;
    void initialize() ;
    void construct_springs() ;
    void construct_M() ;
    void construct_L() ;
    void construct_J() ;
    void compute_system_matrix() ;
    void compute_gravitational_force() ;
    void simulate() ;
    void compute_inertial_term() ;
    void local_step() ;
    void global_step() ;
    void pin_vertices() ;
    void deform_springs() ;
    void update_normals() ;
    void initialize_normals() ;
    glm::vec3 compute_normal( int f1, int f2, int f3 ) ;
    void normalize_normals() ;
    void compute_external_forces() ;
    void compute_wind_force() ;
    void compute_restoration_force() ;
    std::vector<float> get_cloth() ;
    int random( int min, int max ) ;
    ~ClothSimulator() ;
    
} ;
