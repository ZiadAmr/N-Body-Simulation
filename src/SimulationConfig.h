#ifndef SIMULATIONCONFIG_H
#define SIMULATIONCONFIG_H

#include <string>
#include <unordered_set>
#include <iostream>

struct SimulationConfig
{
    int numParticles = 1000;
    double dt = 0.001;
    int iterations = 1000;
    double mapsize = 1.0;
    double v0 = 0.1;
    std::string layout = "cube";
};

bool parseArguments(int argc, char* argv[], SimulationConfig& config) {
    const std::unordered_set<std::string> allowedLayouts = {"cube"};

    for(int i = 1; i < argc; ++i){
        std::string arg = argv[i];

        if (arg == "-n"){
            if(i + 1 < argc) {
                config.numParticles = std::stoi(argv[++i]);
                if(config.numParticles <= 0){
                    std::cerr << "Error: Number of particles must be positive" << std::endl;
                    return false;
                }
            } else {
                std::cerr << "Error -n requires a value" << std::endl;
                return false;
            }
        } else if (arg == "-dt") {
            if(i + 1 < argc){
                config.dt = std::stod(argv[++i]);
                if(config.dt <= 0){
                    std::cerr << "Error: timestep must be positive" << std::endl;
                }
            } else {
                std::cerr << "Error: -dt requires a value" << std::endl;
                return false;
            }
        } else if (arg == "-i") {
            if(i + 1 < argc) {
                config.iterations = std::stoi(argv[++i]);
                if (config.iterations <= 0) {
                    std::cerr << "Error: Iterations must be positive.\n";
                    return false;
                }                
            } else {
                std::cerr << "Error: -i requires a value.\n";
                return false;
            }
        } else if (arg == "-l") {
            if (i + 1 < argc) {
                std::string layout = argv[++i];
                if (allowedLayouts.find(layout) != allowedLayouts.end()) {
                    config.layout = layout;
                } else {
                    std::cerr << "Error: Invalid layout '" << layout << "'. Allowed layouts are: ";
                    for (const auto& l : allowedLayouts) std::cerr << l << " ";
                    std::cerr << "\n";
                    return false;
                }
            } else {
                std::cerr << "Error: -l requires a value.\n";
                return false;
            }
        } else if (arg == "-ms") {
            if (i + 1 < argc) {
                double ms = std::stod(argv[++i]);
                if (ms <= 0) {
                    std::cerr << "Error: mapsize must be positive" << std::endl;
                }
            } else {
                std::cerr << "Error: -ms requires a value.\n";
                return false;
            }
        } else if (arg == "-v0") {
            if (i + 1 < argc) {
                double ms = std::stod(argv[++i]);
                if (ms <= 0) {
                    std::cerr << "Error: initial velocity must be positive" << std::endl;
                }
            } else {
                std::cerr << "Error: -v0 requires a value.\n";
                return false;
            }           
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            return false;
        }
    }

    return true;
}
#endif /* SIMULATIONCONFIG_H */