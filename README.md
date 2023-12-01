# Cloth Simulation

Developed in C++ and OpenGL, this simulation uses a Mass-Spring System approach with the assistance of OpenMP to simulate cloth dynamics. Additionally, it employs Physically-Based Rendering (PBR) and Image-Based Lighting (IBL) techniques to enhance the realism of the cloth visuals.

![Screenshot 2023-12-01 at 16 37 19 2](https://github.com/AmrHMorsy/Cloth-Simulation/assets/56271967/9f6eccff-9659-4638-80f2-6604c01417ce)


## Features

### Graphics and Rendering:
- Powered by OpenGL, the project offers advanced graphics rendering capabilities.
- Utilized Physically-Based Rendering (PBR) techniques for lifelike shading and materials, resulting in realistic interactions of light and shadow on the cloth.
- Enhanced visual fidelity with Image-Based Lighting, providing accurate reflections and illumination.
- Shadow Mapping for dynamic, realistic shadows, enhancing the 3D appearance of the cloth.
- Fog Rendering adds atmospheric depth, contributing to the scene's realism.
- HDR Skyboxes create immersive and dynamic backgrounds, augmenting the overall visual experience.

### Cloth Simulation:
- Implements a Mass-Spring System algorithm, inspired by the "Fast Simulation of Mass-Spring Systems" paper, for authentic cloth physics.
- Integrated OpenMP for efficient parallelization of cloth physics computations, significantly improving performance.
- Offers customizable parameters such as stiffness, damping, and mass, enabling diverse cloth behaviors and properties.
- Realistic collision detection and response ensure accurate interactions between the cloth and other objects.

### Environmental Effects:
- Similar to the ocean simulation, an HDR Skybox is used for a dynamic and immersive sky.
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
