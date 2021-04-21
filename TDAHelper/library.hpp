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

    class Util {
    public:
        template <typename V>
        static int find(std::vector<V> vector, V val);
        template<typename K, typename V>
        static bool keyInMap(std::map<K, V> map, K key);
    };
    //------------------------------------------------------------------------------------------------------------------

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
    //------------------------------------------------------------------------------------------------------------------

    class Shapes {
    public:
        static sf::RectangleShape rect(int x, int y, int width, int height, sf::Color color = sf::Color::White);
        static sf::VertexArray line(int x1, int y1, int x2, int y2, sf::Color color = sf::Color::White);
    };

    //------------------------------------------------------------------------------------------------------------------

    class SpriteSheet {
    public:
        // Constructor/destructor
        SpriteSheet();
        ~SpriteSheet();

        std::map<char, sf::Sprite> mapping;
        void addSprite(char id, sf::Color color);
    };
    //------------------------------------------------------------------------------------------------------------------

    class Tile {
    public:
        // Constructor/destructor
        Tile();
        Tile(char sprite_id, char type_id, int row, int col);
        ~Tile();

        // Config variables
        char sprite_id{};
        char type_id{};
        int row{};
        int col{};

        // Get tile rect
        sf::RectangleShape getRect(int tile_size) const;
        // Get sprite
        sf::RectangleShape getSprite(td::SpriteSheet& sprite_sheet, int tile_size) const;
        std::vector<int> getPosition(int tile_size) const;
    };
    //------------------------------------------------------------------------------------------------------------------

    class Map {
    private:
        // Game map
        std::vector<std::vector<char>> map_raw;
        std::vector<std::vector<td::Tile>> map;

        // Tile configurations
        int tile_size{};
        int max_allowed_tile_size{};

        // Sprites
        td::SpriteSheet sprite_sheet{};

        // Initialization
        void initVariables();
    public:
        // Constructor/destructor
        Map();
        explicit Map(const std::string& path);
        ~Map();

        void readMap(const std::string& path);
        void setSpriteSheet(const td::SpriteSheet& sheet);
        void printMap();
        void draw(sf::RenderTarget* target);
        int getTileSize() const;
        void setTileSize(int size);
        td::Tile getTile(int x, int y);
        std::vector<std::vector<td::Tile>> getMap();
    };
    //------------------------------------------------------------------------------------------------------------------

    class Player {
    private:
        sf::Clock clock;
        // Position
        float x;
        float y;

        // Movement keys
        sf::Keyboard::Key up_key;
        sf::Keyboard::Key down_key;
        sf::Keyboard::Key left_key;
        sf::Keyboard::Key right_key;

        // Appearance
        sf::Sprite sprite;

        // Gameplay
        std::vector<char> inventory;
        int health;
        int max_health;
    public:
        // Constructor/destructor
        Player();
        ~Player();

        // Render
        void draw(sf::RenderTarget* target) const;

        // Movement and collision
        void move(td::Map& map);
        static bool collides(td::Map& map, char type_id, int x, int y);

        // Position
        void setPosition(td::Map& map, int row, int col);
    };
}

#endif //ENGINE_LIBRARY_HPP
