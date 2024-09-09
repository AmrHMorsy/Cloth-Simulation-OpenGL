cmake_minimum_required(VERSION 3.8)
project(Cloth_Simulation)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang++)
set(CMAKE_PREFIX_PATH /encs/opt/local)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -O3")


find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Eigen3 REQUIRED)


include_directories(${CMAKE_SOURCE_DIR}/external/Include)
include_directories(src/)

aux_source_directory(src SOURCE)

add_executable(Cloth_Simulation src/Main.cpp ${SOURCE})

target_link_libraries(Cloth_Simulation 
                      OpenGL::GL 
                      GLEW::glew 
                      glfw 
		      Eigen3::Eigen)

target_include_directories(Cloth_Simulation PRIVATE 
			   ${EIGEN3_INCLUDE_DIR})

