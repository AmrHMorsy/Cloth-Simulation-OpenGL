cmake_minimum_required(VERSION 3.8)
project(Cloth_Simulation)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_PREFIX_PATH /encs/opt/local)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(SDL2_MIXER_INCLUDE_DIRS "/usr/local/include/SDL2")
set(SDL2_MIXER_LIBRARIES "/usr/local/lib/libSDL2_mixer.dylib")

# Find the required packages
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenCL REQUIRED)
list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake/clFFT") # Add the path of clFFT's CMake files
find_package(clFFT REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/external/Include)
include_directories(src/)

aux_source_directory(src SOURCE)

add_executable(Cloth_Simulation src/Main.cpp ${SOURCE})

# Link GLUT libraries and include GLUT directories
target_link_libraries(Cloth_Simulation 
                      OpenCL::OpenCL 
                      OpenGL::GL 
                      GLEW::glew 
                      glfw 
                      ${GLUT_LIBRARIES} # Add GLUT libraries
                      ${ASSIMP_LIBRARIES}
                      /usr/local/lib/libclFFT.dylib
                      ${SDL2_MIXER_LIBRARIES}
                      ${SDL2_LIBRARIES}
                      Eigen3::Eigen)

target_include_directories(Cloth_Simulation PRIVATE 
                           ${FREETYPE_INCLUDE_DIRS} 
                           ${OPENCL_INCLUDE_DIR} 
                           ${clFFT_INCLUDE_DIRS}
                           ${ASSIMP_INCLUDE_DIRS}
                           ${SDL2_INCLUDE_DIRS} 
                           ${SDL2_MIXER_INCLUDE_DIRS}
                           ${EIGEN3_INCLUDE_DIR}
                           ${GLUT_INCLUDE_DIRS}) # Add GLUT include directories
