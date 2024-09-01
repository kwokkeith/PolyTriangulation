/** @file */

#include "monotoneTriangulation.hpp"
#include <algorithm>
#include <iostream>
#include <stack>
#include <fstream>

#define TRIANGLE_EXPORT_FILE "../tests/triangles.txt"

DCEL monotoneTriangulation(DCEL &dcel) {
    int n = dcel.faces.size();
    std::vector<std::array<Point, 3>> triangles;  // Store the triangles

    std::vector<std::pair<int, int> > diagonals;
    for (int i = 1; i < n; i++) {
        faceTriangulation(dcel.faces[i], dcel, diagonals);
    }
    std::vector<Point> new_points;
    std::vector<std::pair<int, int> > new_edges;
    for (Vertex v : dcel.vertices) {
        new_points.push_back(v.point);
    }
    for (auto it : dcel.edges) {
        new_edges.push_back(std::make_pair(it.v1->index, it.v2->index));
    }
    for (auto it : diagonals) {
        new_edges.push_back(it);
    }
    DCEL new_dcel = DCEL(new_points, new_edges);


    // TODO: Convert DCEL to triangle
    // Convert DCEL to triangles using edges
    
    // face[0] is outer boundary
    for (size_t i = 1; i < new_dcel.faces.size(); ++i) {  
        Face &face = new_dcel.faces[i];

        HalfEdge *start = face.half_edge;
        HalfEdge *curr = start;

        std::array<Point, 3> triangle;
        int j = 0;

        // Traverse the half-edges of the face to collect vertices in the correct order
        do {
            triangle[j] = new_dcel.vertices[curr->origin->index].point;
            j++;
            curr = curr->next;
        } while (curr != start && j < 3);

        // Ensure we have exactly 3 vertices and check for degenerate triangles
        if (j == 3) {
            if (!(triangle[0].x == triangle[1].x && triangle[0].y == triangle[1].y) &&
                !(triangle[1].x == triangle[2].x && triangle[1].y == triangle[2].y) &&
                !(triangle[0].x == triangle[2].x && triangle[0].y == triangle[2].y)) {
                triangles.push_back(triangle);
            } else {
                std::cerr << "Skipping degenerate triangle: (" 
                          << triangle[0].x << "," << triangle[0].y << "), ("
                          << triangle[1].x << "," << triangle[1].y << "), ("
                          << triangle[2].x << "," << triangle[2].y << ")\n";
            }
        } else {
            std::cerr << "Error: Face does not form a valid triangle.\n";
        }
    }

   // Export the triangle vertices into a file
    std::ofstream out_triangle(TRIANGLE_EXPORT_FILE);
    if (out_triangle.is_open()) {
        for (const auto &triangle : triangles) {
            out_triangle << triangle[0].x << "," << triangle[0].y 
                         << " " << triangle[1].x << "," << triangle[1].y 
                         << " " << triangle[2].x << "," << triangle[2].y << std::endl;
        }
        out_triangle.close();  // Ensure file is closed after writing
    } else {
        std::cerr << "Error: Unable to open file " << TRIANGLE_EXPORT_FILE << std::endl;
    }

    return new_dcel;
}

void faceTriangulation(Face &face, DCEL &dcel,
                       std::vector<std::pair<int, int> > &diagonals) {
    std::vector<std::pair<Vertex *, int> > vertices;
    HalfEdge *head = face.half_edge;
    HalfEdge *curr = head;
    vertices.push_back(std::make_pair(curr->origin, -1));
    curr = head->next;
    while (curr != head) {
        vertices.push_back(std::make_pair(curr->origin, -1));
        curr = curr->next;
    }
    int index_in_dcel[vertices.size()];
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].second = i;
        index_in_dcel[i] = vertices[i].first->index;
    }
    auto comparator = [](std::pair<Vertex *, int> v1,
                         std::pair<Vertex *, int> v2) {
        return (v1.first->point.y == v2.first->point.y)
                   ? (v1.first->point.x < v2.first->point.x)
                   : (v1.first->point.y > v2.first->point.y);
    };
    std::sort(vertices.begin(), vertices.end(), comparator);
    std::stack<std::pair<Vertex *, int> > st;
    st.push(vertices[0]);
    st.push(vertices[1]);
    for (int j = 2; j < vertices.size() - 1; j++) {
        int stopi = index_in_dcel[st.top().second];
        int stoppi = index_in_dcel[(st.top().second - 1 + vertices.size()) %
                                   vertices.size()];
        int ji = index_in_dcel[vertices[j].second];
        int jpi = index_in_dcel[(vertices[j].second - 1 + vertices.size()) %
                                vertices.size()];
        if (dcel.vertices[ji].point.isBelow(dcel.vertices[jpi].point) !=
            dcel.vertices[stopi].point.isBelow(dcel.vertices[stoppi].point)) {
            while (!st.empty()) {
                auto tp = st.top();
                st.pop();
                if (st.empty()) {
                    break;
                }
                diagonals.push_back(std::make_pair(ji, index_in_dcel[tp.second]));
            }
            st.push(vertices[j - 1]);
            st.push(vertices[j]);
        } else {
            std::pair<Vertex *, int> lpop = st.top();
            st.pop();
            while (!st.empty()) {
                if (dcel.vertices[ji].point.isBelow(dcel.vertices[jpi].point)) {
                    if (!counterClockwise(vertices[j].first->point,
                                          lpop.first->point,
                                          st.top().first->point)) {
                        diagonals.push_back(
                            std::make_pair(ji, index_in_dcel[st.top().second]));
                        lpop = st.top();
                        st.pop();
                    } else {
                        break;
                    }
                } else {
                    if (counterClockwise(vertices[j].first->point,
                                         lpop.first->point,
                                         st.top().first->point)) {
                        diagonals.push_back(
                            std::make_pair(ji, index_in_dcel[st.top().second]));
                        lpop = st.top();
                        st.pop();
                    } else {
                        break;
                    }
                }
            }
            st.push(lpop);
            st.push(vertices[j]);
        }
    }
    while (!st.empty()) {
        diagonals.push_back(
            std::make_pair(vertices.back().first->index, st.top().first->index));
        st.pop();
    }
}
