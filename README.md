# N-Body Simulation
A high-performance N-Body simulation implemented in C++, designed to model the dynamics of particles under mutual gravitational interaction. This project provides a flexible framework for simulating astrophysical systems, including star clusters, galaxies, or general particle systems.

## Features

- Gravitational Computation: Implements the classical N-Body problem with force calculation and velocity verlet integration

- Energy monitoring: tracks total and relative energy drift to ensure physical accuracy (using verifier.py)

- High precision: Designed to minimize numerical errors and energy drift during long simulations.

- Visualzation-ready output: Outputs particle positions and velocities for further plotting and analysis (can be used by galaxy-demo.py for video output and visualizer3D.py for matplotlib animation)

## Getting Started

1. Clone the repository:
```
    https://github.com/ZiadAmr/N-Body-Simulation.git
```
2. Create bin directory
3. cd into bin directory
4. Run the following instructions:
```
    cmake -G "MinGW Makefiles" .. 
    cmake --build .
```

## How to run it

While inside `bin` directory run the `nbody.exe` with the following command line arguments:

```
    .\nbody.exe [-n numParticles] [-dt timestep] [-i iterations] [-l cube] [-ms mapsize] [-v0 init_vel]
```

## Future Extensions

- Implement Barnes-Hut algorithm for improved performance on large particle counts
- Add collision detection and merging for astrophysical realism 