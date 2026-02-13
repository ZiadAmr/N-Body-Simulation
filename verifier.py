import numpy as np
import matplotlib.pyplot as plt

# -------- CONFIG --------
filename = "bin/snapshots.bin"
num_particles = 10000     # <-- set this
G = 1.0
mass = 1.0               # per particle
softening2 = 0.01         # must match simulation
# ------------------------

# Snapshot layout (40 bytes)
dtype = np.dtype([
    ("x", np.float64),
    ("y", np.float64),
    ("z", np.float64),
    ("t", np.float64),
    ("id", np.int32),
    ("_pad", np.int32),
])

data = np.fromfile(filename, dtype=dtype)

num_timesteps = len(data) // num_particles
assert len(data) % num_particles == 0

# Reshape
x = data["x"].reshape(num_timesteps, num_particles)
y = data["y"].reshape(num_timesteps, num_particles)
z = data["z"].reshape(num_timesteps, num_particles)
t = data["t"].reshape(num_timesteps, num_particles)[:, 0]

# -------- ENERGY COMPUTATION --------

def kinetic_energy(vx, vy, vz):
    return 0.5 * mass * np.sum(vx*vx + vy*vy + vz*vz)

def potential_energy(x, y, z):
    U = 0.0
    for i in range(num_particles):
        dx = x[i+1:] - x[i]
        dy = y[i+1:] - y[i]
        dz = z[i+1:] - z[i]
        r = np.sqrt(dx*dx + dy*dy + dz*dz + softening2)
        U -= G * mass * mass * np.sum(1.0 / r)
    return U

def potential_energy_vectorized(x, y, z):
    r2 = (x[:, None] - x[None, :])**2 + \
         (y[:, None] - y[None, :])**2 + \
         (z[:, None] - z[None, :])**2 + softening2
    inv_r = np.where(np.triu(np.ones_like(r2), 1), 1.0 / np.sqrt(r2), 0.0)
    U = -G * mass * mass * np.sum(inv_r)
    return U

# Estimate velocities using finite differences
dt = t[1] - t[0]

vx = np.gradient(x, dt, axis=0)
vy = np.gradient(y, dt, axis=0)
vz = np.gradient(z, dt, axis=0)

E = np.zeros(num_timesteps)
K = np.zeros(num_timesteps)
U = np.zeros(num_timesteps)

for k in range(num_timesteps):
    K[k] = kinetic_energy(vx[k], vy[k], vz[k])
    U[k] = potential_energy_vectorized(x[k], y[k], z[k])
    E[k] = K[k] + U[k]

# -------- PLOT --------

plt.figure(figsize=(8, 4))
plt.plot(t, E, label="Total Energy")
plt.plot(t, K, "--", label="Kinetic")
plt.plot(t, U, "--", label="Potential")
plt.xlabel("Time")
plt.ylabel("Energy")
plt.legend()
plt.title("Energy Conservation Check")
plt.grid(True)
plt.tight_layout()
plt.show()

# Relative energy drift
rel_drift = (E - E[0]) / abs(E[0])
print("Max relative energy drift:", np.max(np.abs(rel_drift)))