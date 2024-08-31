/** @file */

#include "DCEL.hpp"
#include <queue>
#include <set>
#include <utility>
#include <vector>

/**
 * Takes the DCEL of a polygon and returns a DCEL of its monotone partitioning
 * @param dcel DCEL of the polygon (passed by reference)
 * @return DCEL of monotone partitioning of polygon
 */
DCEL makeMonotone(DCEL &dcel);

/**
 * A wrapper structure which represents an event point of the sweep line
 */
struct EventPoint {
    Vertex vertex;
    int index;
};

/**
 * A function that returns the type of an event point, i.e. Split, End, etc.
 * @param event_point the event point
 * @param dcel DCEL of the polygon to which the vertex belongs
 *
 * @return A value of enum type VertexType which denotes the type of the vertex
 */
VertexType getVertexType(EventPoint &event_point, DCEL &dcel);
