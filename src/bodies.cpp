#include "bodies.h"

#include <stdexcept>
#include <cmath>
#include <random>

Bodies::Bodies(int n, int map_size, double dt, double v0)
    :x(n), y(n), z(n),
    vx(n), vy(n), vz(n),
    ax(n), ay(n), az(n),
    old_ax(n), old_ay(n), old_az(n),
    mass(n)
{
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> posDistrib(-map_size, map_size);
    std::uniform_real_distribution<double> velDistrib(-v0, v0);

    double vx_avg = 0; 
    double vy_avg = 0; 
    double vz_avg = 0; 

    for(int i = 0; i < n; i++){
        x[i] = posDistrib(gen);
        y[i] = posDistrib(gen);
        z[i] = posDistrib(gen);

        mass[i] = 1.0;

        vx[i] = velDistrib(gen);
        vy[i] = velDistrib(gen);
        vz[i] = velDistrib(gen);

        vx_avg += mass[i] * vx[i];
        vy_avg += mass[i] * vy[i];
        vz_avg += mass[i] * vz[i];
    }

    for(int i = 0; i < n; i++){
        vx[i] -= vx_avg;
        vy[i] -= vy_avg;
        vz[i] -= vz_avg;
    }

    for(int i = 0; i < n; i++){
        updateAcc(i, dt);
    }
}

void Bodies::updatePos(int n, double dt)
{
    if(n > x.size()-1) throw std::runtime_error("Indexing error in updatePos");
    // calculating pos(t+dt)
    x[n] += vx[n] * dt + 0.5 * ax[n] * dt * dt; 
    y[n] += vy[n] * dt + 0.5 * ay[n] * dt * dt;
    z[n] += vz[n] * dt + 0.5 * az[n] * dt * dt;


}

void Bodies::updateAcc(int n, double dt) // execute after updatePos has been executed for all bodies
{
    if(n > ax.size()-1) throw std::runtime_error("Indexing error in updateAcc ");

    double ax_new = 0;
    double ay_new = 0;
    double az_new = 0;

    for(int i = 0; i < x.size(); i++){
        if(i == n) continue;
        double dx = x[i] - x[n];
        double dy = y[i] - y[n];
        double dz = z[i] - z[n];

        double eps2 = 0.01;
        double r2 = dx*dx + dy*dy + dz*dz + eps2;
        double inv_r = 1.0 / sqrt(r2);
        double inv_r3 = inv_r * inv_r * inv_r;

        ax_new += mass[i] * dx * inv_r3;
        ay_new += mass[i] * dy * inv_r3;  
        az_new += mass[i] * dz * inv_r3; 
    }


    old_ax[n] = ax[n];
    old_ay[n] = ay[n];
    old_az[n] = az[n];

    ax[n] = ax_new;
    ay[n] = ay_new;
    az[n] = az_new;
}

void Bodies::updateVel(int n, double dt)
{
    if(n > vx.size()-1) throw std::runtime_error("Indexing error in updateVel");

    vx[n] += 0.5 *( old_ax[n] + ax[n] ) * dt;
    vy[n] += 0.5 *( old_ay[n] + ay[n] ) * dt;
    vz[n] += 0.5 *( old_az[n] + az[n] ) * dt;
}
