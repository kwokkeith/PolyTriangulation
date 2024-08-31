/** @file */

#include "utils.hpp"

Point::Point(long double x, long double y) : y(y), x(x) {}

Point::Point() : y(0), x(0) {}

bool Point::isBelow(Point p) { return (y == p.y) ? (x > p.x) : (y < p.y); }

bool counterClockwise(Point p1, Point p2, Point p3) {
    return ((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y)) > 0;
}
