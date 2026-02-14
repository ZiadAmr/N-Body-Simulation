import numpy as np
from vispy import app, scene
import imageio

# =========================
# CONFIG
# =========================
filename = "bin/snapshots.bin"
num_particles = 1000
frame_skip = 3
fps = 60
output_file = "galaxy_demo.mp4"
canvas_size = (912, 912)
# =========================

# =========================
# LOAD DATA
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

positions = np.stack((x, y, z), axis=-1).astype(np.float32)

# =========================
# VISPY SETUP
# =========================
canvas = scene.SceneCanvas(
    keys=None,
    bgcolor="black",
    size=canvas_size,
    show=False  # important for offscreen rendering
)

view = canvas.central_widget.add_view()

view.camera = scene.cameras.TurntableCamera(
    fov=60,
    azimuth=45,
    elevation=30,
    distance=10
)

view.camera.center = np.mean(positions[0], axis=0)

r = np.linalg.norm(positions[0] - view.camera.center, axis=1)
view.camera.distance = np.percentile(r, 95) * 2.5


# Compute center-of-mass
com = np.mean(positions[0], axis=0)

# Compute distances from COM
r = np.linalg.norm(positions[0] - com, axis=1)

# Use 95th percentile as camera distance
distance = np.percentile(r, 95) * 2.5  # factor to give padding
view.camera.distance = distance

# Optionally center camera on COM
view.camera.center = com

view.camera.set_range()

scatter = scene.visuals.Markers()
scatter.set_gl_state("translucent", 
                    depth_test=True,
                    blend_func=('src_alpha', 'one'))

scatter.set_data(
    positions[0],
    face_color=(1, 1, 1, 0.9),
    size=6
)

view.add(scatter)

# =========================
# MP4 EXPORT
# =========================
max_frames = num_timesteps // frame_skip

print("Starting MP4 export...")

writer = imageio.get_writer(
    output_file,
    fps=fps,
    codec="libx264",
    quality=10,          # 0–10 (10 = best quality)
    pixelformat="yuv420p"
)

for frame in range(max_frames):
    current = frame * frame_skip

    scatter.set_data(
        positions[current],
        face_color=(1, 1, 1, 0.9),
        size=3
    )

    canvas.update()
    canvas.app.process_events()

    img = canvas.render()
    writer.append_data(img)

    # print(f"Frame {frame+1}/{max_frames}")

writer.close()

print("Saved:", output_file)