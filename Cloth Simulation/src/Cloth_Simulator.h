 #include "Collision_Detector.h"





class Cloth_Simulator{
    
private:
    
    bool ** visited, wind ;
    int max_iterations ;
    Collision_Detector * collision_detector ; 
    Eigen::SparseMatrix<float> L, M, J ;
    std::vector<std::pair<int,int>> springs ;
    std::vector<unsigned int> faces, fixed_points ;
    Eigen::SimplicialLLT<Eigen::SparseMatrix<float>> solver ;
    std::vector<float> vertices, rendering_data, table, rest_lengths, normals ;
    float n, time_step, rest_length, damping_factor, mass, stiffness, perturbationStrength, wind_speed, wind_strength_amplifier, gravity_constant, gravity_amplifier, wind_strength, wind_directionX, wind_directionZ ;
    Eigen::VectorXf inertial_term, b, previous_state, current_state, external_force, spring_directions ;
    
public:
    
    Cloth_Simulator( std::vector<float> v, std::vector< unsigned int> f, std::vector<float> data, std::vector<unsigned int> fp, float wind_amplifier, float _wind_speed, float _gravity_constant );
    void set( Collision_Detector * collision_detector ) ;
    void initialize() ;
    int get_random_number(int min, int max) ;
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
    void perturb_vertices() ;
    void deform_springs() ;
    void update_normals() ; 
    std::vector<float> get_rendering_data() ;
    ~Cloth_Simulator() ;
    
} ;
