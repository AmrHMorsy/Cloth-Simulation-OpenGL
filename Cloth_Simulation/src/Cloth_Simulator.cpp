#include "Cloth_Simulator.h"



//Cloth_Simulator::Cloth_Simulator( std::vector<float> v, std::vector< unsigned int> f, std::vector<float> data ): time_step(0.005f), max_iterations(15), stiffness(1.0f), mass(0.25/((v.size()/3)), faces(f), vertices(v), rendering_data(data), damping_factor(0.993), rest_length(2.0f / ((v.size()/3) - 1) * 1.05f)

//static const int n = 33; // must be odd, n * n = n_vertices | 61
//static const float w = 2.0f; // width | 2.0f
//static const float h = 0.008f; // time step, smaller for better results | 0.008f = 0.016f/2
//static const float r = w / (n - 1) * 1.05f; // spring rest legnth
//static const float k = 1.0f; // spring stiffness | 1.0f;
//static const float m = 0.25f / (n * n); // point mass | 0.25f
//static const float a = 0.993f; // damping, close to 1.0 | 0.993f
//static const float g = 9.8f * m ; // gravitational force | 9.8f

Cloth_Simulator::Cloth_Simulator( std::vector<float> v, std::vector< unsigned int> f, std::vector<float> data ): n(32.0f), time_step(0.005f), max_iterations(15), stiffness(1.0f), mass(0.25/n*n), faces(f), vertices(v), rendering_data(data), damping_factor(0.993), rest_length(2.0f / (n-1) * 1.05f)
{
    initialize() ;
    construct_springs() ;
    construct_M() ;
    construct_L() ;
    construct_J() ;
    float h2 = (time_step*time_step) ;
//    Eigen::SparseMatrix<float> A = M + ( (time_step*time_step) * L ) ;
    Eigen::SparseMatrix<float> A = M + h2 * L ;
    solver.compute(A) ;
}

void Cloth_Simulator::initialize()
{
    visited = new bool*[vertices.size()] ;
    for( int i = 0 ; i < vertices.size() ; i++ )
        visited[i] = new bool[vertices.size()] ;
    for( int i = 0 ; i < vertices.size() ; i++ ){
        for( int j = 0 ; j < vertices.size() ; j++ )
            visited[i][j] = false ;
    }
    previous_state.resize(vertices.size()) ;
    current_state.resize(vertices.size()) ;
    for( int i = 0 ; i < vertices.size() ; i++ )
        current_state[i] = vertices[i] ;
    previous_state = current_state ;
    external_force.resize(vertices.size()) ;
    for( int i = 0 ; i < external_force.size() ; i+=3 ){
        external_force[i] = 0.0f ;
        external_force[i+1] = ((- mass * 9.8))+0.0f  ;
        external_force[i+2] = 0.0f ;
    }
}

int Cloth_Simulator::getRandomNumber(int min, int max) 
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

    return dist(rng);
}

void Cloth_Simulator::construct_springs()
{
    for( int i = 0 ; i < faces.size() ; i+=3 ){
        if( !visited[faces[i]][faces[i+1]] ){
            springs.push_back( std::pair<unsigned int,unsigned int>(faces[i],faces[i+1]) ) ;
            visited[faces[i]][faces[i+1]] = true ;
        }
        if( !visited[faces[i+1]][faces[i+2]] ){
            springs.push_back( std::pair<unsigned int,unsigned int>(faces[i+1],faces[i+2]) ) ;
            visited[faces[i+1]][faces[i+2]] = true ;
        }
        if( !visited[faces[i]][faces[i+2]] ){
            springs.push_back( std::pair<unsigned int,unsigned int>(faces[i],faces[i+2]) ) ;
            visited[faces[i]][faces[i+2]] = true ;
        }
    }
    spring_directions.resize(3*springs.size()) ;
    rest_lengths.resize(springs.size()) ;
    for( int i = 0 ; i < rest_lengths.size() ; i++ )
        rest_lengths[i] = getRandomNumber(1,2) * rest_length ;
}

void Cloth_Simulator::construct_M()
{
    M.resize( vertices.size() , vertices.size() ) ;
    std::vector<Eigen::Triplet<float>> tripletList ;
    tripletList.reserve( vertices.size() ) ;
    for( int i = 0 ; i < vertices.size()/3 ; i++ ){
        for( int j = 0 ; j < 3 ; j++ )
            tripletList.push_back( Eigen::Triplet<float>( (3*i)+j, (3*i)+j, mass) ) ;
    }
    M.setFromTriplets( tripletList.begin(), tripletList.end() ) ;
}

void Cloth_Simulator::construct_L()
{
    L.resize( vertices.size(), vertices.size() ) ;
    std::vector<Eigen::Triplet<float>> tripletList ;
    tripletList.reserve( vertices.size() ) ;
    for ( int s = 0 ; s < springs.size() ; s++ ) {
        int i = springs[s].first ;
        int j = springs[s].second ;
        for ( int m = 0 ; m < 3 ; m++ ){
            tripletList.push_back( Eigen::Triplet<float>( (3*i)+m, (3*i)+m,  stiffness) ) ;
            tripletList.push_back( Eigen::Triplet<float>( (3*i)+m, (3*j)+m, -stiffness) ) ;
            tripletList.push_back( Eigen::Triplet<float>( (3*j)+m, (3*i)+m, -stiffness) ) ;
            tripletList.push_back( Eigen::Triplet<float>( (3*j)+m, (3*j)+m, stiffness)  ) ;
        }
    }
    L.setFromTriplets(tripletList.begin(), tripletList.end()) ;
}

void Cloth_Simulator::construct_J()
{
    J.resize( vertices.size(), 3 * springs.size() ) ;
    std::vector<Eigen::Triplet<float>> tripletList ;
    for ( int s = 0 ; s < springs.size() ; s++ ){
        int i = springs[s].first ;
        int j = springs[s].second ;
        for ( int m = 0; m < 3; m++) {
            tripletList.push_back(Eigen::Triplet<float>( (3*i)+m, (3*s)+m,  stiffness)) ;
            tripletList.push_back(Eigen::Triplet<float>( (3*j)+m, (3*s)+m, -stiffness)) ;
        }
    }
    J.setFromTriplets(tripletList.begin(), tripletList.end()) ;
}

void Cloth_Simulator::simulate()
{
    inertial_term = M * ( ((damping_factor+1) * (current_state)) - (damping_factor * previous_state) ) ;
    previous_state = current_state ;
    for( int i = 0 ; i < max_iterations ; i++ ){
        local_step() ;
        global_step() ;
    }
    deform_springs() ;
}

void Cloth_Simulator::local_step()
{
    for( int s = 0 ; s < springs.size() ; s++ ) {
        int i = springs[s].first ;
        int j = springs[s].second ;
        Eigen::Vector3f d(
            current_state[ (3*i) + 0] - current_state[ (3*j) + 0],
            current_state[ (3*i) + 1] - current_state[ (3*j) + 1],
            current_state[ (3*i) + 2] - current_state[ (3*j) + 2]
        ) ;
        d.normalize() ;
        for( int k = 0 ; k < 3 ; k++ )
            spring_directions[(3*s)+k] = rest_lengths[s] * d[k] ;
    }
}

//void Cloth_Simulator::updateWindForce() {
//    float windStrength = 5.0f * (0.5f + static_cast<float>(rand()) / RAND_MAX); // Random strength
//    float windDirectionX = sin(static_cast<float>(time(NULL)) * 0.5f); // Oscillating direction
//    float windDirectionZ = cos(static_cast<float>(time(NULL)) * 0.5f); // Oscillating direction
//
//    for (int i = 0; i < external_force.size(); i += 3) {
//        // Add dynamic wind force
//        external_force[i] += windStrength * windDirectionX;
//        external_force[i + 2] += windStrength * windDirectionZ;
//    }
//}

void Cloth_Simulator::update_external_force()
{
    float windStrength = 10.0f * (0.5f + static_cast<float>(rand()) / RAND_MAX); ;  // Base strength of the wind
    float windDirectionX = sin(static_cast<float>(time(NULL)) * 0.5f); // Oscillating direction
    float windDirectionZ = cos(static_cast<float>(time(NULL)) * 0.5f); // Oscillating direction

//    float windFrequency = 2.0f;  // Frequency of wind changes
    for (int i = 0; i < external_force.size(); i += 3) {
//        float posX = current_state[i];   // X-coordinate of the vertex
//        float timeFactor = sin(static_cast<float>(time(NULL)) * windFrequency);
//        float positionFactor = sin(posX * windFrequency);
        external_force[i] = 0.0f;
        external_force[i + 1] = (-mass * 9.8f);
        external_force[i + 2] = 0.0f;
        external_force[i] += windStrength * windDirectionX ;
        external_force[i + 2] += windStrength * windDirectionZ ;
    }
}

//void Cloth_Simulator::update_external_force()
//{
//    float windStrength = 10.0f;  // Base strength of the wind
//    float windFrequency = 2.0f;  // Frequency of wind changes
//
//    for (int i = 0; i < external_force.size(); i += 3) {
//        float posX = current_state[i];   // X-coordinate of the vertex
//        float timeFactor = sin(static_cast<float>(time(NULL)) * windFrequency);
//        float positionFactor = sin(posX * windFrequency);
//
//        // Reset external forces to only gravity
//        external_force[i] = 0.0f;
//        external_force[i + 1] = (-mass * 9.8f);
//        external_force[i + 2] = 0.0f;
//
//        // Add wind force
//        external_force[i] += windStrength * timeFactor * positionFactor;
//        external_force[i + 2] += windStrength * timeFactor * positionFactor;
//    }
//
////    external_force.resize(vertices.size()) ;
////    for( int i = 0 ; i < external_force.size() ; i+=3 ){
////        external_force[i] = 0.0f ;
////        external_force[i+1] = ((- mass * 9.8))+0.0f  ;
////        external_force[i+2] = 0.0f ;
////    }
//}

void Cloth_Simulator::global_step()
{
    update_external_force() ;
    b = inertial_term + (time_step * time_step * J * spring_directions ) + (time_step * time_step * external_force )  ;
    current_state = solver.solve(b) ;
    current_state[0] = previous_state[0] ;
    current_state[1] = previous_state[1] ;
    current_state[2] = previous_state[2] ;
    
    current_state[3069] = previous_state[3069] ;
    current_state[3070] = previous_state[3070] ;
    current_state[3071] = previous_state[3071] ;

    
    float perturbationStrength = 0.001f;

    for (int i = 0; i < current_state.size(); i++) {
        current_state[i] += perturbationStrength * (static_cast<float>(rand()) / RAND_MAX - 0.5f);
    }
//    for (int i = 0; i < current_state.size(); i += 3) {
//        if (current_state[i] >= minX && current_state[i] <= maxX &&
//            current_state[i+2] >= minZ && current_state[i+2] <= maxZ) {
//            if (current_state[i+1] < -1.2)
//                current_state[i+1] = -1.2 ;
//        }
//    }

//    current_state[3*(n-1)] = previous_state[3*(n-1)] ;
//    current_state[3*n] = previous_state[3*n] ;
//    current_state[3*(n+1)] = previous_state[3*(n+1)] ;
}

void Cloth_Simulator::deform_springs()
{
    const float tauc = 0.12f; // critical spring deformation | 0.12f
    const unsigned int deformIter = 15; // number of iterations | 15
    for ( int k = 0 ; k < 15 ; k++ ) {
        for ( int s = 0 ; s < springs.size() ; s++ ) {
            Eigen::Vector3f p12(
                current_state[3 * springs[s].first + 0] - current_state[3 * springs[s].second + 0],
                current_state[3 * springs[s].first + 1] - current_state[3 * springs[s].second + 1],
                current_state[3 * springs[s].first + 2] - current_state[3 * springs[s].second + 2]
            );
            float len = p12.norm();
            float rlen = rest_length ;
            float diff = (len - (1 + 0.12f) * rlen) / len;
            float rate = (len - rlen) / rlen;

            float f1, f2;
            f1 = f2 = 0.5f;

            if( springs[s].first == 0 ){
                f1 = 0.0f ;
                f2 = 1.0f ;
            }
            if (springs[s].second == 0 ) {
                f1 = (f1 != 0.0f ? 1.0f : 0.0f);
                f2 = 0.0f;
            }
            
            if( springs[s].first == 1023 ){
                f1 = 0.0f ;
                f2 = 1.0f ;
            }
            if (springs[s].second == 1023 ) {
                f1 = (f1 != 0.0f ? 1.0f : 0.0f);
                f2 = 0.0f;
            }

            // check deformation
            if (rate <= 0.12f) continue;

            for (int j = 0; j < 3; j++) {
                current_state[3 * springs[s].first + j] -= p12[j] * f1 * diff;
                current_state[3 * springs[s].second + j] += p12[j] * f2 * diff;
            }
        }
    }
}

std::vector<float> Cloth_Simulator::get_rendering_data()
{
    int k = 0 ;
    for( int i = 0 ; i < faces.size() ; i++ ){
        for( int j = 0 ; j < 3 ; j++ )
            rendering_data[k+j] = current_state[(3*faces[i])+j] ;
        k+=8 ;
    }
    return rendering_data ;
}

Cloth_Simulator::~Cloth_Simulator(){}
//    for (int s = 0; s < springs.size(); s++) {
//        int i = springs[s].first;
//        int j = springs[s].second;
//        Eigen::Vector3f d(
//            current_state[(3 * i) + 0] - current_state[(3 * j) + 0],
//            current_state[(3 * i) + 1] - current_state[(3 * j) + 1],
//            current_state[(3 * i) + 2] - current_state[(3 * j) + 2]
//        );
//        float current_length = d.norm();
//        float delta_length = current_length - rest_length;
//        d = d.normalized(); // Normalize
//        for (int k = 0; k < 3; k++) {
//            spring_directions[(3 * s) + k] = delta_length * d[k];
//        }
//    }
