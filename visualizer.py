import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# -------- CONFIG --------
filename = "bin/snapshots.bin"
num_particles = 1000         # <-- change this
interval_ms = 30           # animation speed
# ------------------------

# Match C++ struct layout (40 bytes)
dtype = np.dtype([
    ("x", np.float64),
    ("y", np.float64),
    ("z", np.float64),
    ("timestep", np.float64),
    ("id", np.int32),
    ("_pad", np.int32),
])

data = np.fromfile(filename, dtype=dtype)

# Infer number of timesteps
num_timesteps = len(data) // num_particles
assert len(data) % num_particles == 0

# Reshape to [time, particle]
x = data["x"].reshape(num_timesteps, num_particles)
y = data["y"].reshape(num_timesteps, num_particles)

# -------- PLOT SETUP --------
fig, ax = plt.subplots()
scat = ax.scatter(x[0], y[0], s=2)

ax.set_title("N-body simulation")
ax.set_xlabel("x")
ax.set_ylabel("y")


# Set fixed limits (important for performance)
margin = 0.1
ax.set_xlim(x.min() - margin, x.max() + margin)
ax.set_ylim(y.min() - margin, y.max() + margin)

# -------- ANIMATION --------
def update(frame):
    scat.set_offsets(np.column_stack((x[frame], y[frame])))
    ax.set_title(f"t = {frame}")
    return scat,

ani = FuncAnimation(
    fig,
    update,
    frames=num_timesteps,
    interval=interval_ms,
    blit=True
)

plt.show()