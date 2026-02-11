#ifndef SNAPSHOT_H
#define SNAPSHOT_H

struct Snapshot
{
    Snapshot(double timestep, int id, double x, double y, double z)
        : timestep(timestep), id(id), x(x), y(y), z(z) { }
        
    double x, y, z;
    double timestep;
    int id;
};


#endif /* SNAPSHOT_H */