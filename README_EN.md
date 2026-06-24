# 📐 2D Parametric Curve Generator

> **Computer Graphics Assignment — UENF 2025/2026**
> Interactive C + OpenGL application for generating, visualizing, and manipulating closed parametric curves, with efficient mouse picking via an AABB tree structure.

> 🇧🇷 A Portuguese version of this README is available in [Readme.md](./Readme.md).

---

## 🎯 Goal

Build an interactive 2D graphics application where users can define control points by clicking and visualize closed parametric curves in real time, with geometric transformations and precise mouse picking.

---

## ✅ Implemented Features

### 📊 Supported Curves
- [x] **Hermite**
- [x] **Bézier**
- [x] **B-Spline** (with adaptive subdivision)
- [x] **Catmull-Rom**

### 🖱️ Interaction
- [x] Control point creation via mouse click
- [x] Visualizable closed control polygon
- [x] Vertex selection and drag (mouse picking)
- [x] Curve type switching
- [x] Interactive side menu

### 🔄 Geometric Transformations
- [x] Translation
- [x] Rotation around a point
- [x] Scale
- [ ] Mirroring / Reflection
- [ ] Shear

### ⚡ Efficient Picking (B-Spline)
- [x] Hierarchical **AABB Tree** built per segment
- [x] Pruning by mouse-to-box distance (`best_dist` propagated)
- [x] Closest-child-first traversal
- [x] Adaptive `t` refinement via binary subdivision at the leaf
- [x] Debug visualization of AABBs (by index and by hierarchy level)

---

## 🏗️ Project Structure

```
comp_grafica_trabalho_1/
├── src/
│   ├── curvas/
│   │   └── bspline.c         # Evaluation, adaptive subdivision, picking
│   ├── opengl.c              # Main loop, events, rendering
│   └── ...
├── includes/
│   ├── curvas/
│   │   └── bspline.h
│   ├── tipos.h               # ponto, Pontos, AABB, AABBTREE, ResultadoPicking
│   └── ...
├── MakeFile.sh               # Build & run script
└── shell.nix                 # Nix environment (optional)
```

---

## 🧠 Technical Highlights

### Adaptive Subdivision
The curve is generated recursively. Given two endpoints `A` and `B` on interval `[t0, t1]`, the midpoint `M` is computed. If `M` lies close enough to segment `AB` (within tolerance), the point is accepted. Otherwise the interval is split into `[t0, tmid]` and `[tmid, t1]`. This ensures point density proportional to curvature.

### AABB Tree for Picking
Each B-Spline segment has its own AABB computed from the **actual curve points** (not the control points), producing tight bounding boxes. A hierarchical binary tree groups segments, enabling efficient pruning: picking descends the tree by prioritizing the child whose box is closest to the mouse, using `best_dist` to discard branches that cannot improve the current best result.

### `t` Refinement at the Leaf
When a candidate leaf is reached, `subdivide_picking_bspline` performs binary subdivision on `[0, 1]` within the segment — computing the AABB of each half and descending toward the mouse — until `t1 - t0 < 0.001`, at which point it returns the `t` and the actual distance to the curve point.

---

## 💻 Technologies

| Item | Detail |
|------|--------|
| Language | C (C99) |
| Graphics | OpenGL + GLUT |
| Compiler | GCC |
| Platform | Linux (also buildable on Windows with MinGW) |
| Optional env | Nix (`shell.nix`) |

---

## ⚡ How to Run

```bash
# Clone the repository
git clone https://github.com/Zadoque/comp_grafica_trabalho_1
cd comp_grafica_trabalho_1

# Build and run
chmod +x MakeFile.sh
./MakeFile.sh
```

> **Dependencies:** `gcc`, `libGL`, `libGLU`, `freeglut` (or `libglut`).
> With Nix: `nix-shell` sets up the full environment automatically.

---

## 🎮 Controls

| Action | Command |
|--------|----------|
| Add control point | Left click |
| Select / drag vertex | Left click on point |
| Switch curve type | Side menu |
| Apply transformation | Side menu |
| Quit | `ESC` |

---

## 🔧 Planned Improvements

- [ ] **AABBTREE memory arena** — eliminate cache misses by allocating all tree nodes in a single contiguous array (`2n - 1` nodes for `n` segments), replacing per-node `malloc` with a single allocation
- [ ] **Pre-compute `box_left` / `box_right`** — store the AABBs for both halves of each segment directly inside the `AABB` struct, removing the redundant `criar_box_picking` calls during picking
- [ ] Mirroring and shear transformations
- [ ] Support for multiple simultaneous curves

---

## 👨‍💻 Author

**Zadoque Carneiro** — Computer Science student, UENF  
[github.com/Zadoque](https://github.com/Zadoque)
