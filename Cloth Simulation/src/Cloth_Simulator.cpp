#include "Cloth_Simulator.h"




Cloth_Simulator::Cloth_Simulator( std::vector<float> v, std::vector< unsigned int> f, std::vector<float> data, std::vector<unsigned int> fp, float wind_amplifier, float _wind_speed, float _gravity_constant): fixed_points(fp), n(sqrt(v.size()/3)), time_step(0.008f), max_iterations(15), stiffness(0.1f), mass(0.25/n*n), faces(f), vertices(v), rendering_data(data), damping_factor(0.993), rest_length(2.0f / (n-1) * 1.05f), perturbationStrength(0.001f), wind_strength_amplifier(wind_amplifier), wind_directionX(1.0f), wind_speed(_wind_speed), gravity_constant(_gravity_constant)
{
    initialize() ;
    construct_springs() ;
    construct_M() ;
    construct_L() ;
    construct_J() ;
    float h2 = (time_step*time_step) ;
    Eigen::SparseMatrix<float> A = M + h2 * L ;
    solver.compute(A) ;
}

void Cloth_Simulator::set( Collision_Detector * collision_detector )
{
    this->collision_detector = collision_detector ; 
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
    normals.resize(vertices.size()) ;
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
        rest_lengths[i] = get_random_number(1,2) * rest_length ;
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
    inertial_term = M * (((damping_factor+1)*(current_state))-(damping_factor * previous_state)) ;
    previous_state = current_state ;
    for( int i = 0 ; i < max_iterations ; i++ ){
        local_step() ;
        global_step() ;
        fix_vertices() ;
        perturb_vertices() ;
    }
    deform_springs() ;
    update_normals() ;
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

void Cloth_Simulator::update_external_force()
{
    compute_wind() ;
    for (int i = 0; i < external_force.size(); i += 3) {
        external_force[i] = wind_strength * wind_directionX ;
        external_force[i+1] = - gravity_amplifier * mass * 9.8f * gravity_constant ;
        external_force[i+2] = wind_strength * wind_directionZ;
    }
}

void Cloth_Simulator::compute_wind()
{
    gravity_amplifier = 1.0f ;
    wind_strength = wind_speed * (0.5f + static_cast<float>(rand()) / RAND_MAX);
    wind_directionZ = cos(static_cast<float>(time(NULL)) * 0.5f) ;
    if (rand() % 100 < 5) {
        wind_strength *= wind_strength_amplifier ;
        gravity_amplifier = 2.0f ;
    }
}

void Cloth_Simulator::global_step()
{
    update_external_force() ;
    b = inertial_term + (time_step * time_step * J * spring_directions ) + (time_step * time_step * external_force )  ;
    current_state = solver.solve(b) ;
}

void Cloth_Simulator::fix_vertices()
{
    int j ;
    for( int i = 0 ; i < fixed_points.size() ; i++ ){
        j = 3*fixed_points[i] ;
        current_state[j] = previous_state[j] ;
        current_state[j+1] = previous_state[j+1] ;
        current_state[j+2] = previous_state[j+2] ;
    }
}

void Cloth_Simulator::perturb_vertices()
{
    for (int i = 0; i < current_state.size(); i++)
        current_state[i] += perturbationStrength * (static_cast<float>(rand()) / RAND_MAX - 0.5f);
}

void Cloth_Simulator::deform_springs()
{
    float f1, f2, length, diff, rate ;
    for ( int k = 0 ; k < 15 ; k++ ) {
        for ( int s = 0 ; s < springs.size() ; s++ ) {
            Eigen::Vector3f v(
                current_state[3 * springs[s].first + 0] - current_state[3 * springs[s].second + 0],
                current_state[3 * springs[s].first + 1] - current_state[3 * springs[s].second + 1],
                current_state[3 * springs[s].first + 2] - current_state[3 * springs[s].second + 2]
            );
            length = v.norm() ;
            diff = (length - (1 + 0.12f) * rest_length) / length ;
            rate = (length - rest_length) / rest_length ;
            f1 = f2 = 0.5f ;
            for( int i = 0 ; i < fixed_points.size() ; i++ ){
                if( springs[s].first == fixed_points[i] ){
                    f1 = 0.0f ; f2 = 1.0f ;
                }
                if( springs[s].second == fixed_points[i] ){
                    f1 = (f1 != 0.0f ? 1.0f : 0.0f) ;
                    f2 = 0.0f;
                }
            }
            if ( rate <= 0.12f )
                continue ;
            for ( int j = 0 ; j < 3 ; j++ ) {
                current_state[3 * springs[s].first + j] -= v[j] * f1 * diff ;
                current_state[3 * springs[s].second + j] += v[j] * f2 * diff ;
            }
        }
    }
}

void Cloth_Simulator::update_normals()
{
    for( int i = 0 ; i < normals.size() ; i++ )
        normals[i] = 0.0f ;
    for ( int i = 0 ; i < faces.size() ; i+=3 ){
        glm::vec3 v1( current_state[(3*faces[i])], current_state[(3*faces[i])+1], current_state[(3*faces[i])+2] ) ;
        glm::vec3 v2( current_state[(3*faces[i+1])], current_state[(3*faces[i+1])+1], current_state[(3*faces[i+1])+2] ) ;
        glm::vec3 v3( current_state[(3*faces[i+2])], current_state[(3*faces[i+2])+1], current_state[(3*faces[i+2])+2] ) ;
        glm::vec3 normal = glm::cross( (v2-v1), (v3-v1) ) ;
        if (glm::length(normal) != 0)
            normal = glm::normalize(normal) ;
        for( int j = 0 ; j < 3 ; j++ )
            normals[(3*faces[i])+j] += normal[j] ;
        for( int j = 0 ; j < 3 ; j++ )
            normals[(3*faces[i+1])+j] += normal[j] ;
        for( int j = 0 ; j < 3 ; j++ )
            normals[(3*faces[i+2])+j] += normal[j] ;
    }
    for( int i = 0 ; i < normals.size() ; i+=3 ){
        glm::vec3 normal( normals[i], normals[i+1], normals[i+2] ) ;
        if (glm::length(normal) != 0)
            normal = glm::normalize(normal) ;
        for( int j = 0 ; j < 3 ; j++ )
            normals[i+j] = normal[j] ;
    }
}

std::vector<float> Cloth_Simulator::get_rendering_data()
{
    int k = 0 ;
    for( int i = 0 ; i < faces.size() ; i++ ){
        for( int j = 0 ; j < 3 ; j++ )
            rendering_data[k+j] = current_state[(3*faces[i])+j] ;
        for( int j = 0 ; j < 3 ; j++ )
            rendering_data[k+5+j] = normals[(3*faces[i])+j] ;
        k+=8 ;
    }
    return rendering_data ;
}

int Cloth_Simulator::get_random_number(int min, int max)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(rng);
}

Cloth_Simulator::~Cloth_Simulator(){}
