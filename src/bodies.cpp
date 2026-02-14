#include "bodies.h"

#include <stdexcept>
#include <cmath>
#include <random>
#include <string>

Bodies::Bodies(std::string initType, int n, double map_size, double dt, double q, double v0)
    :x(n), y(n), z(n),
    vx(n), vy(n), vz(n),
    ax(n), ay(n), az(n),
    old_ax(n), old_ay(n), old_az(n),
    mass(n)
{
    if(initType == "cube") cubeInitializer(n, map_size, dt, q, v0);
    else if (initType == "plummer") plummerInitializer(n, map_size, dt, q, v0);
    else if (initType == "sphere") sphereInitializer(n, map_size, dt, q, v0);
    else if (initType == "disk") diskInitializer(n, 0.05*map_size,  map_size, dt, q, v0);
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

void Bodies::updateAcc2(int n, double dt)
{

    for(int i = 0; i < n; ++i){

        double xi = x[i];
        double yi = y[i];
        double zi = z[i];

        double mass_i = mass[i];

        for(int j = i+1; j < n; ++j ){
            double dx = x[j] - xi;
            double dy = y[j] - yi;
            double dz = z[j] - zi;

            double r2 = dx*dx + dy*dy + dz*dz + eps2;
            double inv_r = 1.0 / std::sqrt(r2);
            double inv_r3 = inv_r * inv_r * inv_r;

            double s_i = mass_i * inv_r3;
            double s_j = mass[j] * inv_r3;

            ax[i] += s_j * dx;
            ay[i] += s_j * dy;
            az[i] += s_j * dz;
            
            ax[j] -= s_i * dx;
            ay[j] -= s_i * dy;
            az[j] -= s_i * dz;
        }
    }
}

void Bodies::cubeInitializer(int n, double map_size, double dt, double q, double v0)
{
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> posDistrib(-map_size, map_size);
    std::uniform_real_distribution<double> velDistrib(-v0, v0);

    double vx_avg = 0; 
    double vy_avg = 0; 
    double vz_avg = 0; 

    double x_avg = 0, y_avg = 0, z_avg = 0;

    // double x_avg = 0;
    // double y_avg = 0;
    // double z_avg = 0; 

    for(int i = 0; i < n; i++){
        x[i] = posDistrib(gen);
        y[i] = posDistrib(gen);
        z[i] = posDistrib(gen);

        x_avg += x[i];
        y_avg += y[i];
        z_avg += z[i];

        mass[i] = 1.0;

        vx[i] = velDistrib(gen);
        vy[i] = velDistrib(gen);
        vz[i] = velDistrib(gen);

        vx_avg += mass[i] * vx[i];
        vy_avg += mass[i] * vy[i];
        vz_avg += mass[i] * vz[i];
    }

    // scaling velocities
    double K = 0;
    double U = 0;
    for(int i = 0; i < n; i++){
        K += 0.5 * mass[i] * (vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i]);
    }

    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            double dx = x[i] - x[j];
            double dy = y[i] - y[j];
            double dz = z[i] - z[j];
        
            double r2 = dx*dx + dy*dy + dz*dz + eps2;
            double inv_r = 1.0 / sqrt(r2);

            U -= mass[i] * mass[j] * inv_r;
        }
    }

    double target_k = q * std::abs(U);

    double scale = std::sqrt( target_k / K);

    for(int i = 0; i < n; i++){
        vx[i] *= scale;
        vy[i] *= scale;
        vz[i] *= scale;
    }

    vx_avg /= n;
    vy_avg /= n;
    vz_avg /= n;

    x_avg /= n;
    y_avg /= n;
    z_avg /= n;

    for(int i = 0; i < n; i++){
        vx[i] -= vx_avg;
        vy[i] -= vy_avg;
        vz[i] -= vz_avg;

        x[i] -= x_avg;
        y[i] -= y_avg;
        z[i] -= z_avg;
    }

    for(int i = 0; i < n; i++){
        updateAcc(i, dt);
    }
}

void Bodies::plummerInitializer(int n, double map_size, double dt, double q, double v0)
{
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> posDistrib(0, 1);
    std::uniform_real_distribution<double> velDistrib(-1, 1);

    double vx_avg = 0; 
    double vy_avg = 0; 
    double vz_avg = 0; 

    double x_avg = 0, y_avg = 0, z_avg = 0;

    for(int i = 0; i < n; i++){
        double X = posDistrib(gen);
        double a = 1.0;
        double r = a * pow( pow(X, -2.0/3.0) - 1, -0.5 ); // a = scale length

        // Random angles
        double theta = acos(1 - 2*posDistrib(gen));
        double phi = 2 * 3.14 * posDistrib(gen);

        x[i] = r*sin(theta)*cos(phi);
        y[i] = r*sin(theta)*sin(phi);
        z[i] = r*cos(theta);

        mass[i] = 1.0;

        vx[i] = velDistrib(gen);
        vy[i] = velDistrib(gen);
        vz[i] = velDistrib(gen);

        vx_avg += mass[i] * vx[i];
        vy_avg += mass[i] * vy[i];
        vz_avg += mass[i] * vz[i];
    }

    // scaling velocities
    double K = 0;
    double U = 0;
    for(int i = 0; i < n; i++){
        K += 0.5 * mass[i] * (vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i]);
    }

    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            double dx = x[i] - x[j];
            double dy = y[i] - y[j];
            double dz = z[i] - z[j];
        
            double r2 = dx*dx + dy*dy + dz*dz + eps2;
            double inv_r = 1.0 / sqrt(r2);

            U -= mass[i] * mass[j] * inv_r;
        }
    }

    double target_k = q * std::abs(U);

    double scale = std::sqrt( target_k / K);

    for(int i = 0; i < n; i++){
        vx[i] *= scale;
        vy[i] *= scale;
        vz[i] *= scale;
    }

    vx_avg /= n;
    vy_avg /= n;
    vz_avg /= n;

    for(int i = 0; i < n; i++){
        vx[i] -= vx_avg;
        vy[i] -= vy_avg;
        vz[i] -= vz_avg;
    }

    for(int i = 0; i < n; i++){
        updateAcc(i, dt);
    }

}

void Bodies::sphereInitializer(int n, double map_size, double dt, double q, double v0)
{
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> posDistrib(0, 1);
    std::uniform_real_distribution<double> velDistrib(-1, 1);

    double vx_avg = 0; 
    double vy_avg = 0; 
    double vz_avg = 0; 

    double x_avg = 0, y_avg = 0, z_avg = 0;

    for(int i = 0; i < n; i++){     
        // Random radius in [0, R] with proper weighting            
        double u = posDistrib(gen);       // random in [0,1)
        double r = map_size * std::cbrt(u);    // cube root gives uniform density in 3D
        double theta = acos(1 - 2*posDistrib(gen));
        double phi = 2 * 3.14 * posDistrib(gen);

        x[i] = r * sin(theta) * cos(phi);
        y[i] = r * sin(theta) * sin(phi);
        z[i] = r * cos(theta);

        mass[i] = 1.0;

        vx[i] = velDistrib(gen);
        vy[i] = velDistrib(gen);
        vz[i] = velDistrib(gen);

        vx_avg += mass[i] * vx[i];
        vy_avg += mass[i] * vy[i];
        vz_avg += mass[i] * vz[i];
    }

    // scaling velocities
    double K = 0;
    double U = 0;
    for(int i = 0; i < n; i++){
        K += 0.5 * mass[i] * (vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i]);
    }

    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            double dx = x[i] - x[j];
            double dy = y[i] - y[j];
            double dz = z[i] - z[j];
        
            double r2 = dx*dx + dy*dy + dz*dz + eps2;
            double inv_r = 1.0 / sqrt(r2);

            U -= mass[i] * mass[j] * inv_r;
        }
    }

    double target_k = q * std::abs(U);

    double scale = std::sqrt( target_k / K);

    for(int i = 0; i < n; i++){
        vx[i] *= scale;
        vy[i] *= scale;
        vz[i] *= scale;
    }

    vx_avg /= n;
    vy_avg /= n;
    vz_avg /= n;

    for(int i = 0; i < n; i++){
        vx[i] -= vx_avg;
        vy[i] -= vy_avg;
        vz[i] -= vz_avg;
    }

    updateAcc2(n, dt);
    // for(int i = 0; i < n; i++){
    //     updateAcc(i, dt);
    // }
}

void Bodies::diskInitializer(int n, double disk_height, double disk_radius, double dt, double q, double v0)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> uniform(0.0, 1.0);
    std::normal_distribution<double> gaussian(0.0, disk_height);
    std::normal_distribution<double> vz_dist(0, 0.05);

    mass.resize(n, 1.0);

    double total_mass = n;

    for(int i = 0; i < n; i++) {
        double u = uniform(gen);
        double r = -disk_radius * log(1.0 - u);

        double phi = 2.0 * 3.14 * uniform(gen);

        x[i] = r * cos(phi);
        y[i] = r * sin(phi);
        z[i] = gaussian(gen);

        double enclosed_mass = total_mass * (1.0 - exp(-r/disk_radius) * (1.0 + r/disk_radius));

        double v_circ = sqrt(enclosed_mass / sqrt(r*r + eps2) );

        vx[i] = -v_circ * sin(phi);
        vy[i] = v_circ * cos(phi);
        vz[i] = vz_dist(gen);
    }

    // Remove COM velocity

    double vx_avg = 0, vy_avg = 0, vz_avg = 0;
    for(int i = 0; i < n; i++){
        vx_avg += vx[i];
        vy_avg += vy[i];
        vz_avg += vz[i];
    }

    vx_avg /= n;
    vy_avg /= n;
    vz_avg /= n;

    for(int i = 0; i < n; i++){
        vx[i] -= vx_avg;
        vy[i] -= vy_avg;
        vz[i] -= vz_avg;
    }

    // Remove COM position

    double x_avg = 0, y_avg = 0, z_avg = 0;

    for(int i = 0; i < n; i++){
        x_avg += x[i];
        y_avg += y[i];
        z_avg += z[i];
    }

    x_avg/=n;
    y_avg/=n;
    z_avg/=n;
    
    for(int i = 0; i < n; i++){
        x[i]-=x_avg;
        y[i]-=y_avg;
        z[i]-=z_avg;      
    }

    // Virial scaling    
    // double K = 0;
    // double U = 0;

    // for(int i=0;i<n;i++)
    //     K += 0.5*(vx[i]*vx[i]+vy[i]*vy[i]+vz[i]*vz[i]);

    // for(int i=0;i<n;i++)
    // for(int j=i+1;j<n;j++)
    // {
    //     double dx=x[i]-x[j];
    //     double dy=y[i]-y[j];
    //     double dz=z[i]-z[j];

    //     double r = sqrt(dx*dx+dy*dy+dz*dz+eps2);

    //     U -= 1.0/r;
    // }

    // double target_K = q * fabs(U);
    // double scale = sqrt(target_K/K);

    // for(int i=0;i<n;i++)
    // {
    //     vx[i]*=scale;
    //     vy[i]*=scale;
    //     vz[i]*=scale;
    // }
    
    for(int i=0;i<n;i++) updateAcc(i, dt);
}
