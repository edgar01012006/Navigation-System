#ifndef MAP_GRAPH_HPP
#define MAP_GRAPH_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <cstdint>
#include <string_view>
#include <numbers> // C++20 standard library math constants
#include "pugixml.hpp"

// Represents a geographic coordinate point
struct Coordinate {
    double lat;
    double lon;
};

// Represents a directed edge in the graph
struct Edge {
    int64_t target_node_id;
    double distance_meters;
};

// Aliases for readability
using NodeMap = std::unordered_map<int64_t, Coordinate>;
using AdjacencyList = std::unordered_map<int64_t, std::vector<Edge>>;

class MapGraph {
public:
    MapGraph() = default;

    bool loadFromOsm(const std::string& filepath);

    const AdjacencyList& getAdjacencyList() const { return m_adjacency_list; }
    const NodeMap& getNodeCoordinates() const { return m_node_lut; }

    void clear();

private:
    NodeMap m_node_lut;
    AdjacencyList m_adjacency_list;

    void parseNodes(const pugi::xml_node& osm_root);
    void parseWays(const pugi::xml_node& osm_root);

    // Utility math function
    static double calculateHaversine(const Coordinate& c1, const Coordinate& c2);
};

#endif // MAP_GRAPH_HPP