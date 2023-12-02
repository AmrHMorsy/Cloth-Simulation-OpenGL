# Cloth Simulation


Developed in C++ and OpenGL, this simulation employs a Mass-Spring System approach for realistic cloth dynamics, inspired by the paper ["Fast Simulation of Mass-Spring Systems"](http://graphics.berkeley.edu/papers/Liu-FSM-2013-11/Liu-FSM-2013-11.pdf) by Liu et al. Alongside this, the project leverages OpenMP for efficient parallelization and integrates advanced rendering techniques such as Physically-Based Rendering (PBR) and Image-Based Lighting (IBL) to enhance the visual realism of the simulated cloth.

![image](https://github.com/AmrHMorsy/Cloth-Simulation/assets/56271967/5bfad963-42dd-4bc8-9ae7-9afe49af8200)


## Features

### Graphics and Rendering:
- Powered by OpenGL, the project offers advanced graphics rendering capabilities.
- Utilized Physically-Based Rendering (PBR) techniques for lifelike shading and materials, resulting in realistic interactions of light and shadow on the cloth.
- Enhanced visual fidelity with Image-Based Lighting, providing accurate reflections and illumination.
- Shadow Mapping for dynamic, realistic shadows, enhancing the 3D appearance of the cloth.
- Fog Rendering adds atmospheric depth, contributing to the scene's realism.
- HDR Skyboxes create immersive and dynamic backgrounds, augmenting the overall visual experience.

### Cloth Simulation:
- Implemented a Mass-Spring System algorithm, inspired by the ["Fast Simulation of Mass-Spring Systems"](http://graphics.berkeley.edu/papers/Liu-FSM-2013-11/Liu-FSM-2013-11.pdf) paper, for fast and accurate simulation of cloth physics.
- Integrated OpenMP for efficient parallelization of cloth physics computations, significantly improving performance.
- Offers customizable parameters such as stiffness, damping, and mass, enabling diverse cloth behaviors and properties.
- Realistic collision detection and response ensure accurate interactions between the cloth and other objects.

### Environmental Effects:
- An HDR Skybox is used for a dynamic and immersive sky.
- Atmospheric fog effects are implemented, adding depth and a captivating ambiance to the scene.
- Real-time lighting effects, including adjustable light positions and intensities, enhance the realism of the cloth simulation.

## Installation

1. Clone the repository:
```
git clone https://github.com/AmrHMorsy/cloth_simulation-.git
```
2. Navigate to the project directory: 
```
cd Cloth_Simulation
```
3. Create a build directory: 
```
mkdir build
```
4. Navigate to the build directory: 
```
cd build
```
5. Generate makefiles using cmake: 
```
cmake ../
```
6. compile using makefile: 
```
make
```
7. Run the program: 
```
./cloth_simulation
```
