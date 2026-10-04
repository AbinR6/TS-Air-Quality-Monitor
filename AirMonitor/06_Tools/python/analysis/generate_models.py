import os

def create_box_stl(filename, name, x0, y0, z0, dx, dy, dz):
    """Generates an ASCII STL representation of a rectangular prism."""
    # 8 vertices
    v = [
        (x0, y0, z0),          # 0
        (x0 + dx, y0, z0),     # 1
        (x0 + dx, y0 + dy, z0),# 2
        (x0, y0 + dy, z0),     # 3
        (x0, y0, z0 + dz),     # 4
        (x0 + dx, y0, z0 + dz),# 5
        (x0 + dx, y0 + dy, z0 + dz), # 6
        (x0, y0 + dy, z0 + dz) # 7
    ]
    # 12 triangles (2 per face)
    facets = [
        # Bottom (-Z)
        (0, 2, 1, (0, 0, -1)), (0, 3, 2, (0, 0, -1)),
        # Top (+Z)
        (4, 5, 6, (0, 0, 1)), (4, 6, 7, (0, 0, 1)),
        # Front (-Y)
        (0, 1, 5, (0, -1, 0)), (0, 5, 4, (0, -1, 0)),
        # Back (+Y)
        (2, 3, 7, (0, 1, 0)), (2, 7, 6, (0, 1, 0)),
        # Left (-X)
        (0, 4, 7, (-1, 0, 0)), (0, 7, 3, (-1, 0, 0)),
        # Right (+X)
        (1, 2, 6, (1, 0, 0)), (1, 6, 5, (1, 0, 0))
    ]
    
    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"solid {name}\n")
        for i1, i2, i3, norm in facets:
            f.write(f"  facet normal {norm[0]:.1f} {norm[1]:.1f} {norm[2]:.1f}\n")
            f.write("    outer loop\n")
            f.write(f"      vertex {v[i1][0]:.3f} {v[i1][1]:.3f} {v[i1][2]:.3f}\n")
            f.write(f"      vertex {v[i2][0]:.3f} {v[i2][1]:.3f} {v[i2][2]:.3f}\n")
            f.write(f"      vertex {v[i3][0]:.3f} {v[i3][1]:.3f} {v[i3][2]:.3f}\n")
            f.write("    endloop\n")
            f.write("  endfacet\n")
        f.write(f"endsolid {name}\n")

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
target_dir = os.path.join(BASE_DIR, "08_Models")
os.makedirs(target_dir, exist_ok=True)

# ESP32-WROVER-B: 18.0 mm x 31.4 mm x 3.3 mm
create_box_stl(os.path.join(target_dir, "esp32_wrover_b.stl"), "ESP32_WROVER-B", 0, 0, 0, 18.0, 31.4, 3.3)

# PM Sensor (Plantower style): 50.0 mm x 38.0 mm x 21.0 mm
create_box_stl(os.path.join(target_dir, "pm_sensor_module.stl"), "PM_SENSOR_MODULE", 0, 0, 0, 50.0, 38.0, 21.0)

# Main PCB: 64.0 mm x 64.0 mm x 1.6 mm
create_box_stl(os.path.join(target_dir, "air_monitor_pcb.stl"), "AIR_MONITOR_PCB", 0, 0, 0, 64.0, 64.0, 1.6)

# Display Glass Subassembly: 48.0 mm x 48.0 mm x 2.2 mm
create_box_stl(os.path.join(target_dir, "display_subassembly.stl"), "DISPLAY_SUBASSEMBLY", 0, 0, 0, 48.0, 48.0, 2.2)

# Copy to 02_Hardware/3d
hw_3d = os.path.join(BASE_DIR, "02_Hardware", "3d")
os.makedirs(hw_3d, exist_ok=True)
import shutil
for fname in ["esp32_wrover_b.stl", "pm_sensor_module.stl", "air_monitor_pcb.stl", "display_subassembly.stl"]:
    shutil.copy2(os.path.join(target_dir, fname), os.path.join(hw_3d, fname))

print("Generated 3D STL models successfully.")
