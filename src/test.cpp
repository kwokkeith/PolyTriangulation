#include "makeMonotone.hpp"
#include "monotoneTriangulation.hpp"
#include "utils.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <utility>
#include <fstream>


using namespace std;

const int MOD = 1e9 + 7;
const int MAX_N = 2e5 + 5;

int32_t main() {
    int testnumber = 2;
    cin >> testnumber;
    ifstream fin("./tests/in" + to_string(testnumber) + ".txt");
    ofstream fout("./tests/out" + to_string(testnumber) + ".txt");
    ofstream mout("./tests/mout" + to_string(testnumber) + ".txt");

    auto allstart = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::high_resolution_clock::now();
    int n;
    fin >> n;
    vector<Point> arr(n);
    int mxi = 0;
    long double mxy = -100000000;
    for (int i = 0; i < n; i++) {
        long double x, y;
        fin >> x >> y;
        arr[i] = Point(x, y);
        if (y > mxy) {
            mxy = y;
            mxi = i;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto execution_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    long double ms = execution_time.count();
    long double s = ms / 1000000;
    cout << "Time for input from File: " << s << endl;
    cout << "Number of Vertices in polygon: " << n << endl;
    start = std::chrono::high_resolution_clock::now();

    DCEL dcel = DCEL(arr);
    EventPoint ep;
    ep.index = mxi;
    ep.vertex = dcel.vertices[mxi];
    if (getVertexType(ep, dcel) == SPLIT) {
        reverse(arr.begin(), arr.end());
        dcel = DCEL(arr);
        ofstream rout("./tests/in" + to_string(testnumber) + ".txt");
        rout << n << "\n";
        for (int i = 0; i < n; i++) {
            rout << arr[i].x << " " << arr[i].y << "\n";
        }
    }
    end = std::chrono::high_resolution_clock::now();
    execution_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    ms = execution_time.count();
    s = ms / 1000000;
    cout << "Time to construct DCEL: " << s << endl;

    start = std::chrono::high_resolution_clock::now();
    DCEL ans = makeMonotone(dcel);
    std::cout << "MakeMonotone Done!" << std::endl;
    end = std::chrono::high_resolution_clock::now();
    execution_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    ms = execution_time.count();
    s = ms / 1000000;
    cout << "Time for MakeMonotone: " << s << endl;

    start = std::chrono::high_resolution_clock::now();
    DCEL tans = monotoneTriangulation(ans);
    std::cout << "Triangulation Done!" << std::endl;
    end = std::chrono::high_resolution_clock::now();
    execution_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    ms = execution_time.count();
    s = ms / 1000000;
    cout << "Time for Triangulation: " << s << endl;

    start = std::chrono::high_resolution_clock::now();

    fout << tans.edges.size() << "\n";
    for (auto it : tans.edges) {
        fout << it.v1->index << " " << it.v2->index << "\n";
    }

    mout << ans.edges.size() << "\n";
    for (auto it : ans.edges) {
        mout << it.v1->index << " " << it.v2->index << "\n";
    }

    end = std::chrono::high_resolution_clock::now();
    execution_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    ms = execution_time.count();
    s = ms / 1000000;

    cout << "Time for output to File: " << s << endl;
    auto allend = std::chrono::high_resolution_clock::now();
    execution_time = std::chrono::duration_cast<std::chrono::microseconds>(
        allend - allstart);
    ms = execution_time.count();
    s = ms / 1000000;
    cout << "Total Time for everything: " << s << endl;

    return 0;
}