/** @file */

#include "DCEL.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
using namespace std;

Face::Face(HalfEdge *half_edge) : half_edge(half_edge) {}

Face::Face() {}

Vertex::Vertex() {}

Edge::Edge() {}

Edge::Edge(Vertex *v1, Vertex *v2, HalfEdge *half_edge)
    : v1(v1), v2(v2), half_edge(half_edge) {}

HalfEdge::HalfEdge() {}

Vertex::Vertex(Point p) : point(p) {}

HalfEdge::HalfEdge(Vertex *origin) : origin(origin) {}

DCEL::DCEL(std::vector<Point> points) {
    Face outer = Face();
    Face inner = Face();
    faces.push_back(inner);
    faces.push_back(outer);
    // Populate Vertices
    for (Point p : points) {
        vertices.push_back(Vertex(p));
        vertices.back().index = vertices.size() - 1;
        edges.push_back(Edge());
        half_edges.push_back(HalfEdge());
        half_edges.push_back(HalfEdge());
    }
    int n = vertices.size();

    for (int i = 0; i < n; i++) {
        half_edges[i].origin = &vertices[i];
        half_edges[i + n].origin = &vertices[(i + 1) % n];
        half_edges[i].index = i;
        half_edges[i + n].index = i + n;
        half_edges[i].next = &half_edges[(i + 1) % n];
        half_edges[(i + 1) % n].prev = &half_edges[i];

        half_edges[(i + 1) % n + n].next = &half_edges[i + n];
        half_edges[i + n].prev = &half_edges[(i + 1) % n + n];

        edges[i].v1 = &vertices[i];
        edges[i].v2 = &vertices[(i + 1) % n];
        edges[i].half_edge = &half_edges[i];
        vertices[i].half_edge = &half_edges[i];
        half_edges[i].edge = &edges[i];
        half_edges[i + n].edge = &edges[i];

        half_edges[i].twin = &half_edges[i + n];
        half_edges[i + n].twin = &half_edges[i];
    }
    faces[0].half_edge = &half_edges[0];
    faces[1].half_edge = &half_edges[n];
}

/**
 * Gets the angle made by vector and x axis in counterclockwise direction
 * @param x component of vector along x
 * @param y component of vector along y
 * @return angle in range [0, 2*pi]
 */
long double getAngle(long double x, long double y) {
    if (y > 0) {
        return acos(x / (sqrt(x * x + y * y)));
    } else {
        return 2 * acos(-1) - acos(x / (sqrt(x * x + y * y)));
    }
}

DCEL::DCEL(std::vector<Point> points,
           std::vector<std::pair<int, int> > edgeList) {
    int n = points.size();
    int m = edgeList.size();
    for (Point point : points) {
        vertices.push_back(Vertex(point));
        vertices.back().index = vertices.size() - 1;
    }
    for (int i = 0; i < m; i++) {
        edges.push_back(Edge());
        half_edges.push_back(HalfEdge());
        half_edges.push_back(HalfEdge());
    }
    for (int i = 0; i < m; i++) {
        half_edges[i].twin = &half_edges[i + m];
        half_edges[i + m].twin = &half_edges[i];
        half_edges[i].index = i;
        half_edges[i + m].index = i + m;
    }
    vector<pair<int, int> > vout[n];
    for (int i = 0; i < m; i++) {
        vout[edgeList[i].first].push_back(std::make_pair(i, edgeList[i].second));
        vout[edgeList[i].second].push_back(std::make_pair(i, edgeList[i].first));
    }
    for (int i = 0; i < n; i++) {
        int center = i;
        auto comparator = [&](pair<int, int> e1, pair<int, int> e2) {
            return getAngle(points[e1.second].x - points[center].x,
                            points[e1.second].y - points[center].y) >
                   getAngle(points[e2.second].x - points[center].x,
                            points[e2.second].y - points[center].y);
        };
        std::sort(vout[i].begin(), vout[i].end(), comparator);
        for (int j = 0; j < vout[i].size(); j++) {
            int ei = vout[i][j].first;
            int nei = vout[i][(j + 1) % vout[i].size()].first;
            int from = vout[i][j].second;
            int to = vout[i][(j + 1) % vout[i].size()].second;
            int e1i, e2i;
            if (from < i) {
                e1i = ei + m;
            } else {
                e1i = ei;
            }
            if (i < to) {
                e2i = nei + m;
            } else {
                e2i = nei;
            }
            half_edges[e1i].next = &half_edges[e2i];
            half_edges[e2i].prev = &half_edges[e1i];
            half_edges[e2i].origin = &vertices[i];
            vertices[i].half_edge = &half_edges[e1i];
        }
    }
    vector<bool> done(2 * m, 0);
    for (int i = 0; i < 2 * m; i++) {
        if (done[i])
            continue;
        done[i] = 1;
        HalfEdge *head = &half_edges[i];
        HalfEdge *curr = head;
        int j = 0;
        curr = head->next;
        while (curr != head) {
            j++;
            done[curr->index] = 1;
            curr = curr->next;
        }
        Face face = Face();
        face.half_edge = &(half_edges[i]);
        faces.push_back(face);
    }
    for (int i = 0; i < m; i++) {
        edges[i].half_edge = &half_edges[i];
        edges[i].v1 = half_edges[i].origin;
        edges[i].v2 = half_edges[i + m].origin;
    }
}
