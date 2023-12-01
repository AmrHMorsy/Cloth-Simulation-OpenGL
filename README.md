# Cloth Simulation

Developed in C++ and OpenGL, this simulation uses the Fast Fourier Transform method (FFT) with the assistance of OpenMP to simulate cloth. Additionally, it employs Physically-Based Rendering (PBR) and image-based lighting (IBL) techniques to enhance the realism of the scene visuals

We describe a scheme for time integration of mass-spring sys- tems that makes use of a solver based on block coordinate descent. This scheme provides a fast solution for classical linear (Hookean) springs. We express the widely used implicit Euler method as an energy minimization problem and introduce spring directions as auxiliary unknown variables.

![Screenshot 2023-12-01 at 16 37 19 2](https://github.com/AmrHMorsy/Cloth-Simulation/assets/56271967/9f6eccff-9659-4638-80f2-6604c01417ce)


## Features

### Graphics and Rendering:
- Powered by OpenGL, the project offers advanced graphics rendering.
- Utilized Physically-Based Rendering (PBR) techniques to achieve lifelike shading and materials, resulting in realistic reflections, refractions, and lighting effects on the water surface.
- Enhanced visual fidelity with Image-based Lighting, providing richer reflections and accurate illumination.
- Frustum culling ensures optimized performance, rendering only the elements within the player's perspective.

### Cloth Simulation:
- Employed Fast Fourier Transform (FFT) for the realistic depiction of ocean waves, providing a compelling representation of ocean dynamics.
- Integrated OpenCL to efficiently parallelize wave computations, bolstering the simulation's performance.
- Incorporated the Phillips Spectrum for detailed statistical modeling of wave energy distribution across various frequencies.
- Features adjustable ocean temperament settings, allowing transitions between serene waters and turbulent seas.
- Wake effects intricately modify the wave height field, further enhancing the simulation's realism.

### Environmental Effects:
- Introduced an HDR Skybox, presenting a dynamic, immersive sky that accentuates the visual experience.
- Atmospheric fog effects have been implemented, adding depth and a captivating ambiance to the scene.
- The Infinite Ocean feature ensures that the water landscape stretches endlessly, providing a consistent and immersive backdrop.

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
