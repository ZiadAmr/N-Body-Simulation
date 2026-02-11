import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d import Axes3D

# =========================
# CONFIG
# =========================
filename = "bin/snapshots.bin"
num_particles = 100
frame_skip = 3
fps = 30
trail_length = 10
# =========================

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

x = data["x"].reshape(num_timesteps, num_particles)
y = data["y"].reshape(num_timesteps, num_particles)
z = data["z"].reshape(num_timesteps, num_particles)
t = data["t"].reshape(num_timesteps, num_particles)[:, 0]

# =========================
# PLOT SETUP
# =========================
plt.style.use("dark_background")

fig = plt.figure(figsize=(8, 8))
ax = fig.add_subplot(111, projection='3d')

fig.patch.set_facecolor("black")
ax.set_facecolor("black")

# Remove grid & axes
ax.set_xticks([])
ax.set_yticks([])
ax.set_zticks([])
# ax.set_frame_on(False)

margin = 0.1

min = min(x.min(), y.min(), z.min())
max = max(x.max(), y.max(), z.max())

ax.set_xlim(min- margin, max + margin)
ax.set_ylim(min- margin, max + margin)
ax.set_zlim(min- margin, max + margin)

# Star scatter (small, bright)
scat = ax.scatter(x[0], y[0], z[0],
                  s=1,
                  c="white",
                  alpha=0.8)

# Trail container
trails = []

# =========================
# ANIMATION
# =========================
def update(frame):

    global trails

    frame = frame * frame_skip

    # Update stars
    scat._offsets3d = (x[frame], y[frame], z[frame])

    # Remove old trails
    for tr in trails:
        tr.remove()
    trails = []

    # Add motion trails
    if frame > trail_length:
        for i in range(num_particles):
            tr, = ax.plot(
                x[frame-trail_length:frame, i],
                y[frame-trail_length:frame, i],
                z[frame-trail_length:frame, i],
                color="white",
                alpha=0.05,
                linewidth=0.5
            )
            trails.append(tr)

    # Cinematic camera rotation
    # ax.view_init(elev=30, azim=0.2 * frame)

    return scat,

frames = num_timesteps // frame_skip

ani = FuncAnimation(
    fig,
    update,
    frames=frames,
    interval=1000//fps,
    blit=False
)

# Save as high-quality MP4 (recommended)
ani.save("galaxy_demo.gif", writer="pillow", fps=fps, dpi=200)

print("Saved galaxy_demo.mp4")