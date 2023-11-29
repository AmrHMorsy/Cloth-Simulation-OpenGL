#include "Skybox.h"





class Cloth_Simulator{
    
private:
    
    float n ;
    bool ** visited  ;
    float minX, minZ, maxX, maxZ ;
    std::vector< unsigned int> faces ;
    std::vector<std::pair<int,int>> springs ;
    std::vector<float> vertices, rendering_data, table ;
    int max_iterations ;
    Eigen::SparseMatrix<float> L, M, J ;
    float time_step, rest_length, damping_factor, mass, stiffness ;
    Eigen::SimplicialLLT<Eigen::SparseMatrix<float>> solver ;
    Eigen::VectorXf inertial_term, b, previous_state, current_state, external_force, spring_directions ;
    std::vector<float> rest_lengths ; 
    
public:
    
    Cloth_Simulator( std::vector<float> v, std::vector<unsigned int> f, std::vector<float> data) ;
    void initialize() ;
    int getRandomNumber(int min, int max) ; 
    void construct_springs() ;
    void construct_M() ;
    void construct_L() ;
    void construct_J() ;
    void construct_A() ;
    void local_step() ;
    void global_step() ;
    void simulate() ;
    void update_external_force() ;
    std::vector<float> get_rendering_data() ;
    void deform_springs() ;
    ~Cloth_Simulator() ;
    
} ;
