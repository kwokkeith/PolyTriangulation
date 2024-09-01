# Polygon Triangulation

An implementation of polygon triangulation which first partitions the polygon into monotone parts and triangulates the monotone polygons. A DCEL is used to store the polygon and its monotone partitioning and the triangulation. Adopted from https://github.com/arcane810/polygon-triangulation.git and adapted to suit the need for a robotics application where a polygon requires fast triangulation O(NlogN)
