#include "MapGraph.hpp"

// Earth radius in meters for the Haversine formula
constexpr double EARTH_RADIUS_METERS = 6371000.0;

double MapGraph::calculateHaversine(const Coordinate& c1, const Coordinate& c2) {
    auto deg_to_rad = [](double degree) { return degree * std::numbers::pi / 180.0; };

    double dLat = deg_to_rad(c2.lat - c1.lat);
    double dLon = deg_to_rad(c2.lon - c1.lon);

    double lat1 = deg_to_rad(c1.lat);
    double lat2 = deg_to_rad(c2.lat);

    double a = std::sin(dLat / 2.0) * std::sin(dLat / 2.0) +
               std::sin(dLon / 2.0) * std::sin(dLon / 2.0) * std::cos(lat1) * std::cos(lat2);
    
    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return EARTH_RADIUS_METERS * c;
}

void MapGraph::clear() {
    m_node_lut.clear();
    m_adjacency_list.clear();
}

bool MapGraph::loadFromOsm(const std::string& filepath) {
    pugi::xml_document doc;
    
    pugi::xml_parse_result result = doc.load_file(filepath.c_str());
    if (!result) {
        std::cerr << "XML Error: " << result.description() << std::endl;
        return false;
    }

    pugi::xml_node osm_root = doc.child("osm");
    if (!osm_root) {
        std::cerr << "Error: Root <osm> tag not found." << std::endl;
        return false;
    }

    parseNodes(osm_root);

    parseWays(osm_root);

    return true;
}

void MapGraph::parseNodes(const pugi::xml_node& osm_root) {
    for (pugi::xml_node node : osm_root.children("node")) {
        int64_t id = node.attribute("id").as_llong(); // as_llong maps safely to int64_t
        double lat = node.attribute("lat").as_double();
        double lon = node.attribute("lon").as_double();

        m_node_lut[id] = Coordinate{lat, lon};
    }
}

void MapGraph::parseWays(const pugi::xml_node& osm_root) {
    for (pugi::xml_node way : osm_root.children("way")) {
        bool is_highway = false;
        bool is_oneway = false;

        for (pugi::xml_node tag : way.children("tag")) {
            std::string_view key = tag.attribute("k").value();
            std::string_view value = tag.attribute("v").value();

            if (key == "highway") {
                is_highway = true;
            } else if (key == "oneway" && value == "yes") {
                is_oneway = true;
            }
        }

        if (!is_highway) {
            continue;
        }

        std::vector<int64_t> way_nodes;
        for (pugi::xml_node nd : way.children("nd")) {
            way_nodes.push_back(nd.attribute("ref").as_llong());
        }

        if (way_nodes.size() < 2) {
            continue; // Need at least a starting point and ending point to make an edge
        }

        for (size_t i = 0; i < way_nodes.size() - 1; ++i) {
            int64_t node_A = way_nodes[i];
            int64_t node_B = way_nodes[i + 1];

            // Crucial Check: Ensure both nodes actually exist in the coordinate pool
            // Map extracts on city boundaries often contain "way segments" referencing missing outer nodes.
            if (m_node_lut.find(node_A) == m_node_lut.end() || m_node_lut.find(node_B) == m_node_lut.end()) {
                continue; 
            }

            double weight = calculateHaversine(m_node_lut[node_A], m_node_lut[node_B]);

            m_adjacency_list[node_A].push_back(Edge{node_B, weight});

            if (!is_oneway) {
                m_adjacency_list[node_B].push_back(Edge{node_A, weight});
            }
        }
    }
}