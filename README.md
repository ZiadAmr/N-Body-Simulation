# N-Body Simulation
A high-performance N-Body simulation implemented in C++, designed to model the dynamics of particles under mutual gravitational interaction. This project provides a flexible framework for simulating astrophysical systems, including star clusters, galaxies, or general particle systems.

## Features

- Gravitational Computation: Implements the classical N-Body problem with force calculation and velocity verlet integration

- Energy monitoring: tracks total and relative energy drift to ensure physical accuracy (using verifier.py)

- High precision: Designed to minimize numerical errors and energy drift during long simulations.

- Visualzation-ready output: Outputs particle positions and velocities for further plotting and analysis (can be used by galaxy-demo.py for video output and visualizer3D.py for matplotlib animation)

**Note: to run the visualizer.py, verifier.py or galaxy_demo.py you must manually edit the number of particles in the script.**

## Getting Started

1. Clone the repository:
```
    https://github.com/ZiadAmr/N-Body-Simulation.git
```
2. Create bin directory
3. cd into bin directory
4. Run the following instructions:
```
    cmake .. 
    cmake --build .
```

## How to run it

While inside `bin` directory run the `nbody.exe` with the following command line arguments:

```
    .\nbody.exe [-n numParticles] [-dt timestep] [-i iterations] [-l cube] [-ms mapsize] [-v0 init_vel] [-q virial_ratio]
```

The argument `-q` for virial ratio is a positive number is the ratio of the total kinetic energy to the magnitude of graviational potential energy. At 0.5 the generated system is stable, while larger values lead to expansion as cal be seen in `vidoes/expansion.mp4` and smaller values lead to collapse as can be seen in `videos/violent_collapse.mp4`.

The arguments for `-dt` `-l` -`ms` and `v0` must be positive numbers but they can be non-integers, while `-n` must be an integer

The arguments for -l are regarding the initilization of the simulation, these are the available options: 
- `cube` particles are randomly generated within a cube of size `2msx2ms`.
- `plummer`:  particles are initialized in a sphere that is denser in the center.
- `sphere`: particles are randomly and uniformly generated within a sphere of radius $r = ms \sqrt[3]{u}$ where u is a point in a uniform distribution $[0,1)$
- `disk`: (not correct yet) particles are initialized in a rotating, flattened disk with an exponential density profile. Velocities are set to produce stable rotation, approximating a realistic spiral galaxy.

## Showcase

Below is output from `galaxy_demo_pyvis.py` for 1 thousand particles over 2000 timesteps.

https://github.com/user-attachments/assets/6bb998d4-ef7c-43fd-bf92-3e9ef505f9fa

1 thousand particles, with random coordinates in a 5x5 cube, all objects have mass one and initial speed between -0.1 and 0.1. This system exhibits a violent collapse behaviour.

## Future Extensions

- Implement Barnes-Hut algorithm for improved performance on large particle counts
    - Morton ordering
- Add collision detection and merging for astrophysical realism 
- Multipole Acceptance Criterion optimization + node skipping.
- Implement initial condition modes:
    - Hernquist Profile
    - Isothermal Sphere
    - NFW Halo
    - Rotation Sphere
    - Binary Galaxy System
    - Exponential Disk Galaxy
    - Multi-component Galaxy
    - Jeans Equation Initialization
    - Cosmological Inittial Conditions

