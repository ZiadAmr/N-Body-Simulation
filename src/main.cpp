#include <iostream>
using namespace std;

#include "bodies.h"
#include "snapshot.h"
#include "SimulationConfig.h"

#include <format>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>

/*
TODO:
    - Benchmarking suite
    - Profiling results
    - README explaining design tradeoffs
    - Comparison of different integrators
    - Energy drift analysis
    - add theta (opening angle) to the command line
    - Initializers:
        - Hernquist Profile
        - Isothermal Sphere
        - NFW Halo
        - Rotating Sphere
        - Binary galaxy system
        - Exponential Disk Galaxy
        - Multi-component Galaxy
        - Jeans Equation Initialization
        - Cosmological Initial Conditions
*/

void run_simulation(SimulationConfig config, std::vector<Snapshot>& snapshots){
    double dt = config.dt;
    int max_iter = config.iterations;

    Bodies b(config.layout, config.numParticles, config.mapsize, config.dt, config.q, config.v0);

    for(int iteration = 0; iteration < max_iter; iteration++){

        for(int i = 0; i < config.numParticles; i++){
            b.updatePos(i, dt);
        }


        std::swap(b.old_ax, b.ax);
        std::swap(b.old_ay, b.ay);
        std::swap(b.old_az, b.az);

        std::fill(b.ax.begin(), b.ax.end(), 0.0);
        std::fill(b.ay.begin(), b.ay.end(), 0.0);
        std::fill(b.az.begin(), b.az.end(), 0.0);

        b.updateAcc2(config.numParticles, dt);

        for(int i = 0; i < config.numParticles; i++){
            b.updateVel(i, dt);
        }

        for(int i = 0; i < config.numParticles; i++){
            snapshots.emplace_back(iteration * dt, i, b.x[i], b.y[i], b.z[i]);
        }
    }
}


int main(int argc, char* argv[]) {

    SimulationConfig config; 

    if(!parseArguments(argc, argv, config)) {
        std::cerr << "Usage: " << argv[0] 
                  << " [-n numParticles] [-dt timestep] [-i iterations] [-l cube] [-ms mapsize] [-v0 init_vel] [--benchmark]\n";
        return 1;

    }

    std::cout << "Simulation Configuration:\n"
            << "Particles: " << config.numParticles << "\n"
            << "Timestep: " << config.dt << "\n"
            << "Iterations: " << config.iterations << "\n"
            << "Layout: " << config.layout << "\n"
            << "Map Size: " << config.mapsize << "\n"
            << "Initial Velocity: " << config.v0 << "\n"
            << "Benchmarking: " << config.benchmark << "\n";

    std::vector<Snapshot> snapshots;

    if(!config.benchmark) {

        std::ofstream file ( "snapshots.bin" , ios::out | ios::binary ) ;

        if(!file) {

            cout << "Error opening file\n" ;
            return 1 ;

        }

        run_simulation(config, snapshots);

        std::cout << config.numParticles << " Particles over " << config.iterations << " iterations" << std::endl;
            
        file.write(reinterpret_cast<char*>(&snapshots[0]), sizeof(Snapshot) * snapshots.size() );
        file.close();
    }
    else{

    }


    return 0;
}