#include "MapGraph.hpp"

int main() {
    MapGraph graph;
    
    std::cout << "Parsing Yerevan map data..." << std::endl;
    if (!graph.loadFromOsm("data/yerevan.osm")) {
        return -1;
    }

    const auto& adjacency_list = graph.getAdjacencyList();
    std::cout << "Graph built successfully!" << std::endl;
    std::cout << "Total routable intersections/nodes: " << adjacency_list.size() << std::endl;

    MapViewer map(graph.getNodeCoordinates(), graph.getAdjacencyList());
    map.run();

    return 0;
}
