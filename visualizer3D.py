import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d import Axes3D

# =========================
# CONFIGURATION
# =========================
filename = "bin/snapshots.bin"
num_particles = 1000      # <-- SET THIS
interval_ms = 15         # animation speed
marker_size = 2
# =========================

# Define binary layout (40 bytes per snapshot)
dtype = np.dtype([
    ("x", np.float64),
    ("y", np.float64),
    ("z", np.float64),
    ("t", np.float64),
    ("id", np.int32),
    ("_pad", np.int32),   # padding to match 40 bytes
])

# Load file
data = np.fromfile(filename, dtype=dtype)

if len(data) % num_particles != 0:
    raise ValueError("File size is not divisible by num_particles. Check value.")

num_timesteps = len(data) // num_particles

print(f"Loaded {num_timesteps} timesteps with {num_particles} particles each.")

# Reshape into [time, particle]
x = data["x"].reshape(num_timesteps, num_particles)
y = data["y"].reshape(num_timesteps, num_particles)
z = data["z"].reshape(num_timesteps, num_particles)
t = data["t"].reshape(num_timesteps, num_particles)[:, 0]

# =========================
# SET UP 3D PLOT
# =========================
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

scat = ax.scatter(x[0], y[0], z[0], s=marker_size)

# Set fixed limits (IMPORTANT for performance)
margin = 0.1

min = min(x.min(), y.min(), z.min())
max = max(x.max(), y.max(), z.max())

ax.set_xlim(min- margin, max + margin)
ax.set_ylim(min- margin, max + margin)
ax.set_zlim(min- margin, max + margin)

ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")

title = ax.set_title(f"t = {t[0]:.3f}")

# =========================
# ANIMATION FUNCTION
# =========================
def update(frame):
    scat._offsets3d = (x[frame], y[frame], z[frame])
    title.set_text(f"t = {t[frame]:.3f}")
    return scat, title

ani = FuncAnimation(
    fig,
    update,
    frames=num_timesteps,
    interval=interval_ms,
    blit=False  # 3D doesn't support blitting well
)

# # Save as GIF
# ani.save("nbody_3d.gif", writer="pillow", fps=30)

# print("Saved nbody_3d.gif")

plt.tight_layout()
plt.show()