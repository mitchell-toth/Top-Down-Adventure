#ifndef ENGINE_LIBRARY_HPP
#define ENGINE_LIBRARY_HPP

#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <sstream>
#include <iostream>
#include <fstream>

namespace td {

    class Text {
    public:
        enum Align {
            LEFT = -1,
            CENTER = 0,
            RIGHT = 1
        };
        struct Config {
            sf::Font font{};
            int x{0};
            int y{0};
            int size{12};
            Align align{Align::LEFT};
            sf::Color color{sf::Color::White};
        };
        static void print(sf::RenderTarget* target, const std::string& s, const Config& config);
    };

    class Shapes {
    public:
        static sf::RectangleShape rect(int x, int y, int width, int height, sf::Color color = sf::Color::White);
        static sf::VertexArray line(int x1, int y1, int x2, int y2, sf::Color color = sf::Color::White);
    };

    class Tile {
    public:
        // Constructor/destructor
        Tile();
        Tile(char sprite, char type, int row, int col);
        ~Tile();

        // Config variables
        char sprite{};
        char type{};
        int row{};
        int col{};
    };

    class Map {
    private:
        char EMPTY = '`';
        char WALL = 'w';
        char KEY = '*';

        // void setMapChars(key, value)
        // Change the default mapping so that the user can make EMPTY = ' ', for instance.

        // Game map
        std::vector<std::vector<char>> map_raw;
        std::vector<std::vector<td::Tile>> map;

        // Tile configurations
        int tile_size{};
        int max_allowed_tile_size{};

        void initVariables();
    public:
        // Constructor/destructor
        Map();
        explicit Map(const std::string& path);
        ~Map();

        void readMap(const std::string& path);
        void printMap();
        void renderMap(sf::RenderTarget* target);
        void setTileSize(int size);
    };
}

#endif //ENGINE_LIBRARY_HPP
