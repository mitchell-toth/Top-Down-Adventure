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
#include <cmath>

namespace td {
    static sf::Clock CLOCK;
    static const int DEFAULT_TILE_SIZE = 8;

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
        static sf::RectangleShape rect(float x, float y, int width, int height, sf::Color color = sf::Color::White);
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
        sf::RectangleShape getRect(int tile_size = DEFAULT_TILE_SIZE) const;
        // Get sprite
        sf::RectangleShape getSprite(td::SpriteSheet& sprite_sheet, int tile_size = DEFAULT_TILE_SIZE) const;
        sf::Vector2i getPosition(int tile_size = DEFAULT_TILE_SIZE) const;
    };
    //------------------------------------------------------------------------------------------------------------------

    class Map {
    private:
        // Game map
        std::vector<std::vector<char>> map_raw;
        std::vector<std::vector<td::Tile>> map;

        // Special tile types mapper
        std::map<int, std::vector<char>> tile_types;

        // Tile configurations
        int tile_size{};
        int max_allowed_tile_size{};

        // Sprites
        td::SpriteSheet sprite_sheet{};

        // Player
        int player_start_row;
        int player_start_col;

        // Initialization
        void initVariables();
    public:
        // Constructor/destructor
        Map();
        explicit Map(const std::string& path);
        ~Map();

        // Special tile types
        enum TileTypes {
            WALL = 1,
            START = 2,
            DOOR = 3,
            KEY = 4
        };

        // Read in the map
        void readMap(const std::string& path);

        // Render
        void draw(sf::RenderTarget* target);

        // Getters
        int getTileSize() const;
        td::Tile getTile(int x, int y);
        std::vector<std::vector<td::Tile>> getMap();
        std::vector<char> getTileType(int type);
        td::Tile getPlayerStartTile();
        sf::Vector2i getMapSize(bool rows_cols = false);

        // Setters
        void setSpriteSheet(const td::SpriteSheet& sheet);
        void setTileSize(int size);
        void setTileType(int type, std::vector<char> type_id);

        // Collision
        static bool collides(td::Map& map, const std::vector<char>& type_ids, const sf::RectangleShape& rect);
    };
    //------------------------------------------------------------------------------------------------------------------

    class Player {
    private:
        // Position
        float x;
        float y;

        // Size
        int width;
        int height;

        // Color
        sf::Color color;

        // Map
        td::Map map;

        // Movement
        sf::Keyboard::Key up_key;
        sf::Keyboard::Key left_key;
        sf::Keyboard::Key down_key;
        sf::Keyboard::Key right_key;
        float speed;

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

        // Map
        void setMap(td::Map& m);

        // Render
        void draw(sf::RenderTarget* target) const;
        void setColor(sf::Color c);

        // Movement and collision
        void move();
        void setMovementKeys(sf::Keyboard::Key up, sf::Keyboard::Key left, sf::Keyboard::Key down,
                             sf::Keyboard::Key right);
        void setMoveSpeed(float move_speed);

        // Position
        void spawn();
        sf::Vector2f getPosition(bool center=false) const;

        // Size
        void setSize(int w, int h, bool center_in_tile = false);
    };
    //------------------------------------------------------------------------------------------------------------------

}

#endif //ENGINE_LIBRARY_HPP
