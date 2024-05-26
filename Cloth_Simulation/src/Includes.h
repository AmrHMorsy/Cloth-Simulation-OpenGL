#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <cmath>
#include <chrono>
#include <string>
#include <stdio.h>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseQR>
#include <Eigen/Dense>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
#include <OpenGL/OpenGL.h>
#pragma clang diagnostic pop
#include "../external/Include/irrKlang.h"
#include "../external/Include/progressbar.h"

using namespace std ;
using namespace irrklang;

#define NUM_PROGRESS_BARS 140
