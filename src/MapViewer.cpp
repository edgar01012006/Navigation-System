#include "MapViewer.hpp"

MapViewer::MapViewer(const NodeMap& node_lut, const AdjacencyList& adjacency_list) {
    minLon = 44.42;
    maxLon = 44.57;  
    minLat = 40.13;  
    maxLat = 40.23;

    window.create(sf::VideoMode(1200, 800), "Yerevan Map");
    view.reset(sf::FloatRect(0.f, 0.f, 1800.f, 1300.f));
    window.setView(view);
    
    for (const auto& [start, vec] : adjacency_list) {
        sf::Vector2f startPos = coordinatesToPixels(node_lut.at(start)); 

        for (const auto& edge : vec) {
            sf::Vector2f endPos = coordinatesToPixels(node_lut.at(edge.target_node_id));

            map_lines.append(sf::Vertex(startPos, sf::Color::Blue));
            map_lines.append(sf::Vertex(endPos, sf::Color::Blue));
        }
    }
}

sf::Vector2f MapViewer::coordinatesToPixels(const Coordinate& cord) {
    float x = (cord.lon - minLon) / (maxLon - minLon) * 1800.0f;
    float y = (maxLat - cord.lat) / (maxLat - minLat) * 1300.0f;

    return sf::Vector2f{x, y};
}

void MapViewer::drawMap() {      
    window.draw(map_lines);
} 

void MapViewer::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta > 0) {
                view.zoom(0.9f);
            } else if (event.mouseWheelScroll.delta < 0) {
                view.zoom(1.1f);
            }
            window.setView(view);
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            is_dragging = true;
            last_mouse_pos = sf::Mouse::getPosition(window);
            initial_press_pos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
        }

        if (event.type == sf::Event::MouseMoved && is_dragging) {
            sf::Vector2i current_mouse_pos = sf::Mouse::getPosition(window);
            sf::Vector2f delta = window.mapPixelToCoords(last_mouse_pos) - window.mapPixelToCoords(current_mouse_pos);
            
            view.move(delta);
            window.setView(view);
            last_mouse_pos = current_mouse_pos;
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i release_pos(event.mouseButton.x, event.mouseButton.y);
            float total_movement = std::sqrt(std::pow(release_pos.x - initial_press_pos.x, 2) + std::pow(release_pos.y - initial_press_pos.y, 2));

            if (total_movement < 8.0f) {
                if (release_pos.x >= 0 && release_pos.x < window.getSize().x && release_pos.y >= 0 && release_pos.y < window.getSize().y) {
                    sf::Texture window_texture;
                    window_texture.create(window.getSize().x, window.getSize().y);
                    window_texture.update(window);
                    sf::Image screenshot = window_texture.copyToImage();

                    bool hit_blue = false;
                    int radius = 4;

                    for (int dx = -radius; dx <= radius && !hit_blue; ++dx) {
                        for (int dy = -radius; dy <= radius; ++dy) {
                            int check_x = release_pos.x + dx;
                            int check_y = release_pos.y + dy;

                            if (check_x >= 0 && check_x < window.getSize().x && check_y >= 0 && check_y < window.getSize().y) {
                                if (screenshot.getPixel(check_x, check_y) == sf::Color::Blue) {
                                    hit_blue = true;
                                    break;
                                }
                            }
                        }
                    }

                    if (hit_blue) {
                        sf::Vector2f clickPos = window.mapPixelToCoords(release_pos);
                        if (locations.size() == 2) locations.clear();
                        locations.push_back(clickPos);
                    }
                }
            }

            is_dragging = false;
        }
    }
}

void MapViewer::drawNodes() {
    float dynamicRadius = view.getSize().x * 0.005f;

    sf::CircleShape circle(dynamicRadius);
    circle.setFillColor(sf::Color::White);
    circle.setOrigin(dynamicRadius, dynamicRadius);
    
    for (const auto& node : locations) {
        circle.setPosition(node);
        window.draw(circle);
    }
}

void MapViewer::run() {
    while (window.isOpen()) {
        processEvents();
        window.clear(sf::Color::Black);
        drawMap();
        drawNodes();
        window.display();
    }
}