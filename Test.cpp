#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <unordered_map>
#include <boost/container_hash/hash.hpp>
#include <cassert>

#include "AStar.hpp"

// A complete graph with weighted edges
class Graph {
public:
    typedef std::pair<int,int> Edge;
    // Reads graph data from file
    void read_graph(std::ifstream& f) {
        f >> n; // number of vertices
        f >> p1 >> p2; // endpoints
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // Add edge
                f >> weight[{i,j}];
            }
        }
        // length of shortest path as previously calculated
        f >> shortest_dist;
    }
    // Returns the distance from v1 to v2
    float distance(int v1, int v2) {
        return weight[{v1,v2}];
    }
    // Saves the neighbours of vertex v into neighbourhood.
    // Actually the graphs are complete so neighbourhood is always {0,1,...,n-1}.
    void neighbours(std::vector<int>& neighbourhood, int v) {
        neighbourhood.clear();
        for (int i = 0; i < n; ++i) {
            neighbourhood.push_back(i);
        }
    }
    // Graph information as read from the file
    // start and goal vertices
    std::pair<int,int> endpoints() {
        return {p1,p2};
    }
    // Known shortest distance
    float get_shortest_dist() {
        return shortest_dist;
    }
private:
    int n;
    int p1, p2; // endpoints
    float shortest_dist;
    std::unordered_map<Edge, float, boost::hash<Edge>> weight;
};

// Wraps around Graph object's methods
auto distance_wrapper(Graph& g) {
    return [&](int v1, int v2) {
        return g.distance(v1, v2);
    };
}

auto neighbour_wrapper(Graph& g) {
    return [&](std::vector<int>& neighbourhood, int v) {
        g.neighbours(neighbourhood, v);
    };
}

// Compare to within roundoff error
template <class T>
bool close_enough(const T& a, const T& b) {
    return std::abs(a-b) < 1e-3;
}

int main() {
    // Read the test data
    std::ifstream f("test_data.txt");
    Graph g;

    while (!f.eof()) {
        g.read_graph(f);

        AStarSearch<int,float> a;
        a.set_heuristic();
        a.set_distance(distance_wrapper(g));
        a.set_neighbours(neighbour_wrapper(g));

        auto endp = g.endpoints();
        a.set_start(endp.first);
        a.set_goal(endp.second);
        a.solve();

        // For each graph, see whether the algorithm gives the same results
        assert(close_enough(a.get_length(), g.get_shortest_dist()));
    }
    f.close();
    return 0;
}