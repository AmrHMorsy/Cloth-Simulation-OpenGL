#include "CollisionDetector.h"





class ClothSimulator{
    
private:
    
    int max_iterations ;
    bool ** visited, wind ;
    Eigen::SparseMatrix<float> L, M, J ;
    CollisionDetector * collision_detector ;
    std::vector<std::pair<int,int>> springs ;
    std::vector<unsigned int> faces, fixed_points ;
    Eigen::SimplicialLLT<Eigen::SparseMatrix<float>> solver ;
    std::vector<float> vertices, rendering_data, table, rest_lengths, normals, original_vertices ;
    float n, time_step, rest_length, damping_factor, mass, stiffness, perturbationStrength, wind_speed, wind_strength_amplifier, gravity_constant, gravity_amplifier, wind_strength, wind_directionX, wind_directionZ, restoration_strength ;
    Eigen::VectorXf inertial_term, b, previous_state, current_state, external_force, spring_directions ;
    
public:
    
    ClothSimulator( std::vector<float> v, std::vector< unsigned int> f, std::vector<float> data, std::vector<unsigned int> fp) ;
    void initialize() ;
    void construct_springs() ;
    void construct_M() ;
    void construct_L() ;
    void construct_J() ;
    void simulate() ;
    void local_step() ;
    void update_external_force() ;
    void compute_wind() ;
    void global_step() ;
    void fix_vertices() ;
    void deform_springs() ;
    void update_normals() ;
    void restore_vertices() ;
    void prepare_rendering_data() ;
    std::vector<float> get_rendering_data() ;
    int get_random_number(int min, int max) ;
    ~ClothSimulator() ;
    
} ;
