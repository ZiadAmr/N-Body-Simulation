#ifndef BODIES_H
#define BODIES_H

#include <vector>
#include <string>

#include "tree.h"

class Bodies
{
    public:
        Bodies(std::string initType, std::vector<Node>& pool, int n, double map_size, double dt=0.001, double q=0.5, double v0=0.1);

        std::vector<double> x, y, z;
        std::vector<double> vx, vy, vz;
        std::vector<double> ax, ay, az;
        std::vector<double> old_ax, old_ay, old_az;
        std::vector<double> mass;

        void updatePos(int n, double dt);
        void updateAcc(int n, double dt);
        void updateVel(int n, double dt);

        void updateAcc2(int n, double dt);
        void updateAcc3(int n, double dt);

        void updateAccBH(int n, double dt, double map_size, std::vector<Node>& pool);

        double eps2 = 0.01;
    private:
        void cubeInitializer(int n, double map_size, double dt, double q, double v0, std::vector<Node>& pool);
        void plummerInitializer(int n, double map_size, double dt, double q, double v0, std::vector<Node>& pool);
        void sphereInitializer(int n, double map_size, double dt, double q, double v0, std::vector<Node>& pool);
        void diskInitializer(int n, double disk_height, double disk_radius, double dt, double q, double v0, std::vector<Node>& pool);

        double map_size;
};


#endif