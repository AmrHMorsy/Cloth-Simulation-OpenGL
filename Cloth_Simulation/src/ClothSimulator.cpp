#include "ClothSimulator.h"






ClothSimulator::ClothSimulator( std::vector<float> _vertices, std::vector< unsigned int> _faces, std::vector<float> _cloth, std::vector<unsigned int> _pins): pins(_pins), faces(_faces), vertices(_vertices), cloth(_cloth)
{
    define_constants() ;
    initialize() ;
    construct_springs() ;
    construct_M() ;
    construct_L() ;
    construct_J() ;
    compute_system_matrix() ;
    compute_gravitational_force() ;
}

void ClothSimulator::define_constants()
{
    stiffness = 1.0f ;
    time_step = 0.008f ;
    wind_speed = 45.0f ;
    num_iterations = 15 ;
    damping_factor = 0.993 ;
    gravity_amplifier = 10.0f ;
    gravity_acceleration = 9.81 ;
    restoration_strength = 200.0f ;
    wind_strength_amplifier = 2.0f ;
    wind_direction = glm::vec3(0.0f, 0.0f, 1.0f) ;
    float n = sqrt(vertices.size()/3) ;
    mass = 0.5/n*n ;
    rest_length = 2.5f / (n-1) * 1.05f ;
}

void ClothSimulator::initialize()
{
    visited = new bool*[vertices.size()] ;
    for( int i = 0 ; i < vertices.size() ; i++ ){
        visited[i] = new bool[vertices.size()] ;
        for( int j = 0 ; j < vertices.size() ; j++ )
            visited[i][j] = false ;
    }
    previous_state.resize(vertices.size()) ;
    current_state.resize(vertices.size()) ;
    for( int i = 0 ; i < vertices.size() ; i++ )
        current_state[i] = vertices[i] ;
    previous_state = current_state ;
    external_force.resize(vertices.size()) ;
    restoration_force.resize(vertices.size()) ;
    normals.resize(vertices.size()) ;
}

void ClothSimulator::construct_springs()
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
        rest_lengths[i] = random(1,2) * rest_length ;
}

void ClothSimulator::construct_M()
{
    int i, j ;
    std::vector<Eigen::Triplet<float>> t ;
    M.resize( vertices.size() , vertices.size() ) ;
    t.reserve( vertices.size() ) ;
    for( i = 0 ; i < vertices.size()/3 ; i++ ){
        for( j = 0 ; j < 3 ; j++ )
            t.push_back( Eigen::Triplet<float>( (3*i)+j, (3*i)+j, mass) ) ;
    }
    M.setFromTriplets( t.begin(), t.end() ) ;
}

void ClothSimulator::construct_L()
{
    int s, i, j, m ;
    std::vector<Eigen::Triplet<float>> t ;
    L.resize( vertices.size(), vertices.size() ) ;
    t.reserve( vertices.size() ) ;
    for ( s = 0 ; s < springs.size() ; s++ ) {
        i = springs[s].first ;
        j = springs[s].second ;
        for ( m = 0 ; m < 3 ; m++ ){
            t.push_back( Eigen::Triplet<float>( (3*i)+m, (3*i)+m,  stiffness) ) ;
            t.push_back( Eigen::Triplet<float>( (3*i)+m, (3*j)+m, -stiffness) ) ;
            t.push_back( Eigen::Triplet<float>( (3*j)+m, (3*i)+m, -stiffness) ) ;
            t.push_back( Eigen::Triplet<float>( (3*j)+m, (3*j)+m, stiffness)  ) ;
        }
    }
    L.setFromTriplets(t.begin(), t.end()) ;
}

void ClothSimulator::construct_J()
{
    int s, i, j, m ;
    J.resize( vertices.size(), 3 * springs.size() ) ;
    std::vector<Eigen::Triplet<float>> t ;
    for ( s = 0 ; s < springs.size() ; s++ ){
        i = springs[s].first ;
        j = springs[s].second ;
        for ( m = 0; m < 3; m++) {
            t.push_back( Eigen::Triplet<float>( (3*i)+m, (3*s)+m,  stiffness) ) ;
            t.push_back( Eigen::Triplet<float>( (3*j)+m, (3*s)+m, -stiffness) ) ;
        }
    }
    J.setFromTriplets( t.begin(), t.end() ) ;
}

void ClothSimulator::compute_system_matrix()
{
    float h = time_step * time_step ;
    Eigen::SparseMatrix<float> A = M + h * L ;
    solver.compute(A) ;
}

void ClothSimulator::compute_gravitational_force()
{
    gravitational_force = - mass * gravity_acceleration * gravity_amplifier ;
}

void ClothSimulator::simulate()
{
    compute_inertial_term() ;
    previous_state = current_state ;
    for( int i = 0 ; i < num_iterations ; i++ ){
        local_step() ;
        global_step() ;
    }
    pin_vertices() ;
    deform_springs() ;
    update_normals() ;
}

void ClothSimulator::compute_inertial_term()
{
    inertial_term = M * (((damping_factor+1)*(current_state))-(damping_factor * previous_state)) ;
}

void ClothSimulator::local_step()
{
    int s, i, j, k ;
    Eigen::Vector3f d ;
    for( s = 0 ; s < springs.size() ; s++ ) {
        i = springs[s].first ;
        j = springs[s].second ;
        d = Eigen::Vector3f(
            current_state[ (3*i) + 0] - current_state[ (3*j) + 0],
            current_state[ (3*i) + 1] - current_state[ (3*j) + 1],
            current_state[ (3*i) + 2] - current_state[ (3*j) + 2]
        ) ;
        d.normalize() ;
        for( k = 0 ; k < 3 ; k++ )
            spring_directions[(3*s)+k] = rest_lengths[s] * d[k] ;
    }
}

void ClothSimulator::global_step()
{
    compute_external_forces() ;
    b = inertial_term + (time_step * time_step * J * spring_directions) + (time_step * time_step * external_force)  ;
    current_state = solver.solve(b) ;
}

void ClothSimulator::pin_vertices()
{
    int i, j ;
    for( i = 0 ; i < pins.size() ; i++ ){
        j = 3*pins[i] ;
        current_state[j] = previous_state[j] ;
        current_state[j+1] = previous_state[j+1] ;
        current_state[j+2] = previous_state[j+2] ;
    }
}

void ClothSimulator::deform_springs()
{
    float f1, f2, length, diff, rate ;
    for ( int k = 0 ; k < num_iterations ; k++ ) {
        for ( int s = 0 ; s < springs.size() ; s++ ) {
            Eigen::Vector3f v(
                current_state[3 * springs[s].first + 0] - current_state[3 * springs[s].second + 0],
                current_state[3 * springs[s].first + 1] - current_state[3 * springs[s].second + 1],
                current_state[3 * springs[s].first + 2] - current_state[3 * springs[s].second + 2]
            ) ;
            length = v.norm() ;
            diff = (length - (1 + 0.12f) * rest_length) / length ;
            rate = (length - rest_length) / rest_length ;
            if ( rate <= 0.12f )
                continue ;
            f1 = f2 = 0.5f ;
            for( int i = 0 ; i < pins.size() ; i++ ){
                if( springs[s].first == pins[i] ){
                    f1 = 0.0f ; f2 = 1.0f ;
                }
                if( springs[s].second == pins[i] ){
                    f1 = (f1 != 0.0f ? 1.0f : 0.0f) ;
                    f2 = 0.0f;
                }
            }
            for ( int j = 0 ; j < 3 ; j++ ) {
                current_state[3 * springs[s].first + j] -= v[j] * f1 * diff ;
                current_state[3 * springs[s].second + j] += v[j] * f2 * diff ;
            }
        }
    }
}

void ClothSimulator::update_normals()
{
    int f1, f2, f3 ;
    glm::vec3 normal ;
    
    initialize_normals() ;
    for ( int i = 0 ; i < faces.size() ; i+=3 ){
        f1 = (3*faces[i]) ; f2 = (3*faces[i+1]) ; f3 = (3*faces[i+2]) ;
        normal = compute_normal(f1, f2, f3) ;
        for( int j = 0 ; j < 3 ; j++ )
            normals[f1+j] += normal[j] ;
        for( int j = 0 ; j < 3 ; j++ )
            normals[f2+j] += normal[j] ;
        for( int j = 0 ; j < 3 ; j++ )
            normals[f3+j] += normal[j] ;
    }
    normalize_normals() ;
}

void ClothSimulator::initialize_normals()
{
    for( int i = 0 ; i < normals.size() ; i++ )
        normals[i] = 0.0f ;
}

glm::vec3 ClothSimulator::compute_normal( int f1, int f2, int f3 )
{
    glm::vec3 v1, v2, v3, normal ;
    
    v1 = glm::vec3(current_state[f1], current_state[f1+1], current_state[f1+2]) ;
    v2 = glm::vec3(current_state[f2], current_state[f2+1], current_state[f2+2]) ;
    v3 = glm::vec3(current_state[f3], current_state[f3+1], current_state[f3+2]) ;
    normal = glm::vec3(glm::cross( (v2-v1), (v3-v1) )) ;
    if (glm::length(normal) != 0)
        normal = glm::normalize(normal) ;
    return normal ;
}

void ClothSimulator::normalize_normals()
{
    glm::vec3 v1, v2, v3, normal ;
    
    for( int i = 0 ; i < normals.size() ; i+=3 ){
        normal = glm::vec3( normals[i], normals[i+1], normals[i+2] ) ;
        if (glm::length(normal) != 0)
            normal = glm::normalize(normal) ;
        for( int j = 0 ; j < 3 ; j++ )
            normals[i+j] = -normal[j] ;
    }
}

void ClothSimulator::compute_external_forces()
{
    compute_wind_force() ;
    compute_restoration_force() ;
    for ( int i = 0; i < external_force.size(); i += 3) {
        external_force[i] = wind_force.x + restoration_force[i] ;
        external_force[i+1] = gravitational_force + wind_force.y + restoration_force[i+1];
        external_force[i+2] = wind_force.z + restoration_force[i+2] ;
    }
}

void ClothSimulator::compute_wind_force()
{
    wind_strength = wind_speed * (0.5f + static_cast<float>(rand()) / RAND_MAX);
    if (rand() % 100 < 5)
        wind_strength *= wind_strength_amplifier ;
    wind_force = glm::vec3( (wind_strength * wind_direction.x), 0.0f, (wind_strength * wind_direction.z) ) ;
}

void ClothSimulator::compute_restoration_force()
{
    for ( int i = 0 ; i < restoration_force.size() ; i += 3) {
        restoration_force[i] = vertices[i] - current_state[i] ;
        restoration_force[i] *= restoration_strength ;
        restoration_force[i+1] = vertices[i+1] - current_state[i+1] ;
        restoration_force[i+1] *= restoration_strength ;
        restoration_force[i+2] = vertices[i+2] - current_state[i+2] ;
        restoration_force[i+2] *= restoration_strength ;
    }
}

std::vector<float> ClothSimulator::get_cloth()
{
    int i, j, k = 0 ;
    for( i = 0 ; i < faces.size() ; i++ ){
        for( j = 0 ; j < 3 ; j++ )
            cloth[k+j] = current_state[(3*faces[i])+j] ;
        for( j = 0 ; j < 3 ; j++ )
            cloth[k+5+j] = normals[(3*faces[i])+j] ;
        k+=8 ;
    }
    
    return cloth ;
}

int ClothSimulator::random( int min, int max )
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(rng);
}

ClothSimulator::~ClothSimulator()
{
    for( int i = 0 ; i < vertices.size() ; i++ )
        delete visited[i] ;
    delete visited ;
}
