#ifndef BODIES_H
#define BODIES_H

#include <vector>

class Bodies
{
    public:
        Bodies(int n, int map_size, double dt=0.001, double q=0.5, double v0=0.1);

        std::vector<double> x, y, z;
        std::vector<double> vx, vy, vz;
        std::vector<double> ax, ay, az;
        std::vector<double> old_ax, old_ay, old_az;
        std::vector<double> mass;

        void updatePos(int n, double dt);
        void updateAcc(int n, double dt);
        void updateVel(int n, double dt);

    private:
        double eps2 = 0.01;

};


#endif