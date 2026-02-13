#include <iostream>
using namespace std;

#include "bodies.h"
#include "snapshot.h"
#include "SimulationConfig.h"

#include <format>
#include <iomanip>
#include <fstream>
#include <string>

/*
TODO:
    - Benchmarking suite
    - Profiling results
    - README explaining design tradeoffs
    - Comparison of different integrators
    - Energy drift analysis
*/

int main(int argc, char* argv[]) {

    SimulationConfig config; 

    if(!parseArguments(argc, argv, config)) {
        std::cerr << "Usage: " << argv[0] 
                  << " [-n numParticles] [-dt timestep] [-i iterations] [-l cube] [-ms mapsize] [-v0 init_vel]\n";
        return 1;

    }

    std::cout << "Simulation Configuration:\n"
            << "Particles: " << config.numParticles << "\n"
            << "Timestep: " << config.dt << "\n"
            << "Iterations: " << config.iterations << "\n"
            << "Layout: " << config.layout << "\n"
            << "Map Size: " << config.mapsize << "\n"
            << "Initial Velocity: " << config.v0 << "\n";

    double dt = config.dt;
    int max_iter = config.iterations;

    Bodies b(config.numParticles, config.mapsize, config.dt, config.v0);

    std::ofstream file ( "snapshots.bin" , ios::out | ios::binary ) ;

    if(!file) {

        cout << "Error opening file\n" ;
        return 1 ;

    }

    std::vector<Snapshot> snapshots;

    for(int iteration = 0; iteration < max_iter; iteration++){
        for(int i = 0; i < b.mass.size(); i++){
            b.updatePos(i, dt);
            b.updateAcc(i, dt);
            b.updateVel(i, dt);
            
            snapshots.emplace_back(iteration * dt, i, b.x[i], b.y[i], b.z[i]);
        }
    }

    std::cout << config.numParticles << " Particles over " << config.iterations << " iterations" << std::endl;

    file.write(reinterpret_cast<char*>(&snapshots[0]), sizeof(Snapshot) * snapshots.size() );
    file.close();

    return 0;
}