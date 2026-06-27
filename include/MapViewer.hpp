#ifndef MAP_VIEWER_HPP
#define MAP_VIEWER_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <cstdint>
#include <string_view>
#include <numbers> // C++20 standard library math constants
#include "pugixml.hpp"
#include "MapGraph.hpp"
#include <SFML/Graphics.hpp>

class MapViewer {
    private:
        sf::RenderWindow window;
        std::vector<sf::Vector2f> locations;
        sf::VertexArray map_lines{sf::Lines};
        
        sf::View view;
        bool is_dragging = false;
        sf::Vector2i last_mouse_pos;
        sf::Vector2i initial_press_pos;

        double minLon;
        double maxLon;
        double minLat;
        double maxLat;
       
        sf::Vector2f coordinatesToPixels(const Coordinate& cord);
        void drawMap();        
        void processEvents();
        void drawNodes();
    public:
        MapViewer(const NodeMap& node_lut, const AdjacencyList& adjacency_list);
        void run();
};

#endif