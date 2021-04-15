#ifndef ENGINE_LIBRARY_HPP
#define ENGINE_LIBRARY_HPP

#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <sstream>

namespace td {
    void test();

    class Text {
    public:
        enum Align {
            LEFT = -1,
            CENTER = 0,
            RIGHT = 1
        };
        struct Config {
            sf::Text text{};
            int x{0};
            int y{0};
            int size{12};
            Align align{Align::LEFT};
            sf::Color color{sf::Color::White};
        };
        static void print(sf::RenderTarget* target, const std::string& s, Config config);
    };

    class Shapes {
    public:
        static sf::RectangleShape rect(float x, float y, float width, float height, sf::Color color = sf::Color::White);
        static sf::VertexArray line(float x1, float y1, float x2, float y2, sf::Color color = sf::Color::White);
    };
}

#endif //ENGINE_LIBRARY_HPP
