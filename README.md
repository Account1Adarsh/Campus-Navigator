# Campus Navigator

This C++ project implements a multi-floor campus navigation system using Dijkstra's algorithm. It supports:
- 3D node coordinates `(x, y, z)`
- Horizontal "walk" edges and vertical "stairs" edges
- Dynamic turn-by-turn instructions (left/right/straight, upstairs/downstairs)

## Build & Run

```bash
mkdir build && cd build
cmake ..
make
./navigator