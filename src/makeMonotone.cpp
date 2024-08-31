/** @file */

#include "makeMonotone.hpp"
#include <iostream>
#include <queue>
#include <set>
#include <utility>
#include <vector>

/**
 * A comparator class with a functor to compare 2 vertices for priority in the
 * event_points priority queue
 */
class CompareVertices {
  public:
    bool operator()(EventPoint first, EventPoint second) {
        return (first.vertex.point.y == second.vertex.point.y)
                   ? (first.vertex.point.x > second.vertex.point.x)
                   : first.vertex.point.y < second.vertex.point.y;
    }
};

/**
 * A comparator class with a functor to compare HalfEdges from left to right
 */
class CompareEdges {
  public:
    bool operator()(std::pair<Edge *, int> first,
                    std::pair<Edge *, int> second) {
        if (second.first->v1->point.y == second.first->v2->point.y) {
            if (first.first->v1->point.y == first.first->v2->point.y) {
                if (first.first->v1->point.y < second.first->v1->point.y)
                    return true;
                else
                    return false;
            }
            if (counterClockwise(first.first->v1->point, first.first->v2->point,
                                 second.first->v1->point))
                return true;
            else
                return false;
        } else if (first.first->v1->point.y == first.first->v2->point.y) {
            if (counterClockwise(second.first->v1->point,
                                 second.first->v2->point,
                                 first.first->v1->point))
                return false;
            else
                return true;
        } else if (first.first->v1->point.y < second.first->v1->point.y) {
            if (counterClockwise(second.first->v1->point,
                                 second.first->v2->point,
                                 first.first->v1->point))
                return false;
            else
                return true;
        } else {
            if (counterClockwise(first.first->v1->point, first.first->v2->point,
                                 second.first->v1->point))
                return true;
            else
                return false;
        }
    }
};

int n;

VertexType getVertexType(EventPoint &event_point, DCEL &dcel) {
    int n = dcel.vertices.size();
    Vertex curr_vertex = event_point.vertex;
    Vertex prev_vertex = dcel.vertices[(event_point.index - 1 + n) % n];
    Vertex next_vertex = dcel.vertices[(event_point.index + 1) % n];
    VertexType vertex_type;
    if (prev_vertex.point.isBelow(curr_vertex.point) &&
        next_vertex.point.isBelow(curr_vertex.point)) {
        if (counterClockwise(prev_vertex.point, curr_vertex.point,
                             next_vertex.point)) {
            vertex_type = START;
        } else {
            vertex_type = SPLIT;
        }
    } else if (curr_vertex.point.isBelow(prev_vertex.point) &&
               curr_vertex.point.isBelow(next_vertex.point)) {
        if (counterClockwise(prev_vertex.point, curr_vertex.point,
                             next_vertex.point)) {
            vertex_type = END;
        } else {
            vertex_type = MERGE;
        }
    } else {
        vertex_type = REGULAR;
    }
    return vertex_type;
}

/**
 * A function to handle START Vertices of polygon
 * @param event_point the event point
 * @param dcel DCEL of the polygon to which the vertex belongs
 * @param binary_search_tree A BST containing the edges.
 * @param helper A vector which stores the helper of edges using their indices.
 */

void handleStartVertex(
    EventPoint &event_point, DCEL &dcel,
    std::set<std::pair<Edge *, int>, CompareEdges> &binary_search_tree,
    std::vector<int> &helper) {
    binary_search_tree.insert(
        std::make_pair(&dcel.edges[event_point.index], event_point.index));
    helper[event_point.index] = event_point.index;
}

/**
 * A function to handle END Vertices of polygon
 * @param event_point the event point
 * @param dcel DCEL of the polygon to which the vertex belongs
 * @param binary_search_tree A BST containing the edges.
 * @param helper A vector which stores the helper of edges using their indices.
 * @param diagonals A vector of diagonals to which new diagonals are added.
 */
void handleEndVertex(
    EventPoint &event_point, DCEL &dcel,
    std::set<std::pair<Edge *, int>, CompareEdges> &binary_search_tree,
    std::vector<int> &helper, std::vector<std::pair<int, int> > &diagonals) {
    if (helper[(event_point.index - 1 + n) % n] > -1) {
        EventPoint helper_e_i_1;
        helper_e_i_1.vertex =
            dcel.vertices[helper[(event_point.index - 1 + n) % n]];
        helper_e_i_1.index = helper[(event_point.index - 1 + n) % n];
        if (getVertexType(helper_e_i_1, dcel) == MERGE) {
            diagonals.push_back(
                std::make_pair(event_point.index, helper_e_i_1.index));
        }
    }
    binary_search_tree.erase(
        std::make_pair((&dcel.edges[(event_point.index - 1 + n) % n]),
                       (event_point.index - 1 + n) % n));
}

/**
 * A function to handle SPLIT Vertices of polygon
 * @param event_point the event point
 * @param dcel DCEL of the polygon to which the vertex belongs
 * @param binary_search_tree A BST containing the edges.
 * @param helper A vector which stores the helper of edges using their indices.
 * @param diagonals A vector of diagonals to which new diagonals are added.
 */

void handleSplitVertex(
    EventPoint &event_point, DCEL &dcel,
    std::set<std::pair<Edge *, int>, CompareEdges> &binary_search_tree,
    std::vector<int> &helper, std::vector<std::pair<int, int> > &diagonals) {

    Point p1 = Point(event_point.vertex.point.x, event_point.vertex.point.y);
    Point p2 = Point(p1.x + 0.0000001, p1.y);
    Vertex v1 = Vertex(p1);
    Vertex v2 = Vertex(p2);
    Edge temp = Edge(&v1, &v2, nullptr);
    auto ejp = (binary_search_tree.lower_bound(
        std::make_pair(&temp, event_point.index)));
    ejp--;
    diagonals.push_back(
        std::make_pair(helper[(*ejp).second], event_point.index));

    binary_search_tree.insert(
        std::make_pair(&dcel.edges[event_point.index], event_point.index));
    helper[event_point.index] = event_point.index;
    helper[(*ejp).second] = event_point.index;
}

/**
 * A function to handle MERGE Vertices of polygon
 * @param event_point the event point
 * @param dcel DCEL of the polygon to which the vertex belongs
 * @param binary_search_tree A BST containing the edges.
 * @param helper A vector which stores the helper of edges using their indices.
 * @param diagonals A vector of diagonals to which new diagonals are added.
 */

void handleMergeVertex(
    EventPoint &event_point, DCEL &dcel,
    std::set<std::pair<Edge *, int>, CompareEdges> &binary_search_tree,
    std::vector<int> &helper, std::vector<std::pair<int, int> > &diagonals) {
    if (helper[(event_point.index - 1 + n) % n] > -1) {
        EventPoint helper_e_i_1;
        helper_e_i_1.vertex =
            dcel.vertices[helper[(event_point.index - 1 + n) % n]];
        helper_e_i_1.index = helper[(event_point.index - 1 + n) % n];
        if (getVertexType(helper_e_i_1, dcel) == MERGE) {
            diagonals.push_back(
                std::make_pair(event_point.index, helper_e_i_1.index));
        }
    }
    binary_search_tree.erase(
        std::make_pair((&dcel.edges[(event_point.index - 1 + n) % n]),
                       (event_point.index - 1 + n) % n));

    Point p1 = Point(event_point.vertex.point.x, event_point.vertex.point.y);
    Point p2 = Point(p1.x + 0.0000001, p1.y);
    Vertex v1 = Vertex(p1);
    Vertex v2 = Vertex(p2);
    Edge temp = Edge(&v1, &v2, nullptr);
    auto ejp = (binary_search_tree.lower_bound(
        std::make_pair(&temp, event_point.index)));
    ejp--;
    EventPoint helper_e_j;
    helper_e_j.vertex = dcel.vertices[helper[(*ejp).second]];
    helper_e_j.index = helper[(*ejp).second];
    if (getVertexType(helper_e_j, dcel) == MERGE) {
        diagonals.push_back(
            std::make_pair(event_point.index, helper_e_j.index));
    }
    helper[(*ejp).second] = event_point.index;
}

/**
 * A function to handle REGULAR Vertices of polygon
 * @param event_point the event point
 * @param dcel DCEL of the polygon to which the vertex belongs
 * @param binary_search_tree A BST containing the edges.
 * @param helper A vector which stores the helper of edges using their indices.
 * @param diagonals A vector of diagonals to which new diagonals are added.
 */

void handleRegularVertex(
    EventPoint &event_point, DCEL &dcel,
    std::set<std::pair<Edge *, int>, CompareEdges> &binary_search_tree,
    std::vector<int> &helper, std::vector<std::pair<int, int> > &diagonals) {
    int n = dcel.vertices.size();
    Vertex prev = dcel.vertices[(event_point.index - 1 + n) % n];
    Vertex next = dcel.vertices[(event_point.index + 1) % n];
    if (event_point.vertex.point.isBelow(prev.point) &&
        next.point.isBelow(event_point.vertex.point)) {
        if (helper[(event_point.index - 1 + n) % n] > -1) {
            EventPoint helper_e_i_1;
            helper_e_i_1.vertex =
                dcel.vertices[helper[(event_point.index - 1 + n) % n]];
            helper_e_i_1.index = helper[(event_point.index - 1 + n) % n];
            if (getVertexType(helper_e_i_1, dcel) == MERGE) {
                diagonals.push_back(
                    std::make_pair(event_point.index, helper_e_i_1.index));
            }
        }

        binary_search_tree.erase(
            std::make_pair((&dcel.edges[(event_point.index - 1 + n) % n]),
                           (event_point.index - 1 + n) % n));

        binary_search_tree.insert(
            std::make_pair(&dcel.edges[event_point.index], event_point.index));
        helper[event_point.index] = event_point.index;
    } else {

        Point p1 =
            Point(event_point.vertex.point.x, event_point.vertex.point.y);
        Point p2 = Point(p1.x + 0.0000001, p1.y);
        Vertex v1 = Vertex(p1);
        Vertex v2 = Vertex(p2);
        Edge temp = Edge(&v1, &v2, nullptr);
        auto ejp = (binary_search_tree.lower_bound(
            std::make_pair(&temp, event_point.index)));
        ejp--;
        EventPoint helper_e_j;
        helper_e_j.vertex = dcel.vertices[helper[(*ejp).second]];
        helper_e_j.index = helper[(*ejp).second];
        if (getVertexType(helper_e_j, dcel) == MERGE) {
            diagonals.push_back(
                std::make_pair(event_point.index, helper_e_j.index));
        }
        helper[(*ejp).second] = event_point.index;
    }
}

/**
 * A function to handle Vertices of polygon
 * @param event_point the event point
 * @param dcel DCEL of the polygon to which the vertex belongs
 * @param binary_search_tree A BST containing the edges.
 * @param helper A vector which stores the helper of edges using their indices.
 * @param diagonals A vector of diagonals to which new diagonals are added.
 */
void handleVertex(
    EventPoint &event_point, DCEL &dcel,
    std::set<std::pair<Edge *, int>, CompareEdges> &binary_search_tree,
    std::vector<int> &helper, std::vector<std::pair<int, int> > &diagonals) {
    VertexType vertex_type = getVertexType(event_point, dcel);
    switch (vertex_type) {
    case START:
        handleStartVertex(event_point, dcel, binary_search_tree, helper);
        break;
    case END:
        handleEndVertex(event_point, dcel, binary_search_tree, helper,
                        diagonals);
        break;
    case SPLIT:
        handleSplitVertex(event_point, dcel, binary_search_tree, helper,
                          diagonals);
        break;
    case MERGE:
        handleMergeVertex(event_point, dcel, binary_search_tree, helper,
                          diagonals);
        break;
    case REGULAR:
        handleRegularVertex(event_point, dcel, binary_search_tree, helper,
                            diagonals);
    }
}

DCEL makeMonotone(DCEL &dcel) {
    n = dcel.vertices.size();
    std::priority_queue<EventPoint, std::vector<EventPoint>, CompareVertices>
        event_points;
    for (int i = 0; i < n; i++) {
        event_points.push({dcel.vertices[i], i});
    }
    std::set<std::pair<Edge *, int>, CompareEdges> binary_search_tree;
    std::vector<int> helper(n, -1);
    std::vector<std::pair<int, int> > diagonals;
    int i = 1;
    while (!event_points.empty()) {
        i++;
        EventPoint event_point = event_points.top();
        event_points.pop();
        handleVertex(event_point, dcel, binary_search_tree, helper, diagonals);
    }
    std::vector<Point> points;
    for (Vertex v : dcel.vertices) {
        points.push_back(v.point);
    }
    std::vector<std::pair<int, int> > edges;
    for (int i = 0; i < n; i++) {
        edges.push_back(std::make_pair(i, (i + 1) % n));
    }
    for (std::pair<int, int> p : diagonals) {
        edges.push_back(p);
    }
    // DCEL new_dcel = DCEL();
    DCEL new_dcel = DCEL(points, edges);
    return new_dcel;
}
