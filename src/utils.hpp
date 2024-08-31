/** @file */

#pragma once

/**
 * A Class for a Point in 2-D Space
 */
class Point {
  public:
    /// The x coordinate of the Point
    long double x;
    /// The y coordinate of the Point
    long double y;

    /**
     * Constructor. Initializes the coordinates to the passed parameters
     *
     * @param x The x coordinate
     * @param y The y coordinate
     */
    Point(long double x, long double y);

    /**
     * Default Constructor
     */
    Point();

    /**
     * Checks if this point is below the point passed as parameter.
     * @param p The point for which the check is to be made.
     * @return true if this point is below p.
     */
    bool isBelow(Point p);
};

/**
 * Check if 3 points are in counter-clockwise order
 *
 * @param A 1st Point
 * @param B 2nd Point
 * @param C 3rd Point
 * @return true if the 3 points are in counter-clockwise order
 *
 * @relatesalso Point
 */
bool counterClockwise(Point p1, Point p2, Point p3);
