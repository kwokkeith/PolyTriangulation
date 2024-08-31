# Polygon Triangulation

An implementation of polygon triangulation which first partitions the polygon into monotone parts and triangulates the monotone polygons. A DCEL is used to store the polygon and its monotone partitioning and the triangulation. Made as a part of the computation geometry course at BITS Pilani, Hyderabad Campus.

To use the api:

- Use utils.cpp header for the Point class
- Include utils.hpp, DCEL.hpp, makeMonotone.hpp and monotoneTriangulation.hpp
- Use makeMonotone() function to partition the polygon into monotone polygons and then use monotoneTriangulation() function to triangulate the partitions polygon.
- Compile all the files in proper order.

Important points to take care of:

- While using this API provide the polygon in counter-clockwise order in the DCEL constructor.

To use the driver code (test.cpp)

- compile using g++ \*.cpp inside source
- run the program ./a.out and enter test number (1-5)
- tests can be added in /tests folder in file called inx.txt where x is a number, 1st line of test should be number of points and then x, y space separated, 1 in each line in counter-clockwise or clockwise order.
- might need to use bigger stack size (using ulimit -s unlimited in linux)
- output of edges of form v1, v2 is in outx.txt in tests folder
- plot.py plots the results (run plot.py and enter test number (1-5))
