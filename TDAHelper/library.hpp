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
    // Forward declarations
    class Enemy;

    // Classes:
    //------------------------------------------------------------------------------------------------------------------

    class Util {
    public:
        template <typename V>
        static int find(std::vector<V> vector, V val);
        template<typename K, typename V>
        static bool keyInMap(std::map<K, V> map, K key);
        struct size {
            int width{0};
            int height{0};
        };
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

        static const int DEFAULT_TILE_SIZE = 8;

        // Config variables
        char sprite_id{};
        char type_id{};
        int row{};
        int col{};

        // Get tile rect
        sf::RectangleShape getRect(int tile_size = td::Tile::DEFAULT_TILE_SIZE) const;
        // Get sprite
        sf::RectangleShape getSprite(td::SpriteSheet& sprite_sheet, int tile_size = td::Tile::DEFAULT_TILE_SIZE) const;
        sf::Vector2i getPosition(int tile_size = td::Tile::DEFAULT_TILE_SIZE) const;
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

        // Enemies
        std::vector<td::Enemy> enemies;

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
            CHECKPOINT = 3,
            END = 4,
            DOOR = 5,
            KEY = 6
        };

        // Read in the map
        void readMap(const std::string& path);

        // Render
        void draw(sf::RenderTarget* target);
        void drawEnemies(sf::RenderTarget* target);

        // Getters
        int getTileSize() const;
        td::Tile getTile(float x, float y);
        std::vector<std::vector<td::Tile>> getMap();
        std::vector<char> getTileType(int type);
        td::Tile getPlayerStartTile();
        sf::Vector2i getMapSize(bool rows_cols = false);
        std::vector<td::Enemy> getEnemies();

        // Setters
        void setSpriteSheet(const td::SpriteSheet& sheet);
        void setTileSize(int size);
        void setTileType(int type, std::vector<char> type_id);

        // Enemies
        void addEnemy(const td::Enemy& enemy);

        // Collision
        static bool collides(td::Map& map, const std::vector<char>& type_ids, const sf::RectangleShape& rect);
        static std::vector<td::Tile> getCollisions(td::Map& map, const std::vector<char>& type_ids, const sf::RectangleShape& rect);
    };
    //------------------------------------------------------------------------------------------------------------------

    class Player {
    private:
        // Movement
        sf::Keyboard::Key up_key;
        sf::Keyboard::Key left_key;
        sf::Keyboard::Key down_key;
        sf::Keyboard::Key right_key;

        // Gameplay
        std::vector<char> inventory;
        td::Tile checkpoint;
    protected:
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
        float speed;

        // Appearance
        sf::Sprite sprite;

        // Gameplay
        int health;
        int max_health;
    public:
        // Constructor/destructor
        Player();
        ~Player();

        // Map
        virtual void setMap(td::Map& m);

        // Render
        void draw(sf::RenderTarget* target) const;
        void setColor(sf::Color c);

        // Movement
        void move(float elapsed);
        void setMovementKeys(sf::Keyboard::Key up, sf::Keyboard::Key left, sf::Keyboard::Key down,
                             sf::Keyboard::Key right);
        void setMoveSpeed(float move_speed);

        // Position
        void spawn();
        void respawn();
        sf::Vector2f getPosition(bool center=false) const;

        // Checkpoints
        bool onCheckpoint();
        void setCheckpoint();

        // End tiles
        bool onEnd();

        // Size
        td::Util::size getSize() const;
        void setSize(int w, int h, bool center_in_tile = false);

        // Enemy interaction
        bool isTouchingEnemy();
        std::vector<td::Enemy> getTouchingEnemies();

        // Health
        int getHealth() const;
        void setHealth(int h);
        void setMaxHealth(int mh);
        void loseHealth(int health_points);
        void gainHealth(int health_points);
        bool isDead() const;
    };
    //------------------------------------------------------------------------------------------------------------------

    class Enemy : public Player {
    private:
        int harm;
    public:
        // Constructor/destructor
        Enemy();
        ~Enemy();

        // Map
        void setMap(td::Map& m) override;

        // Position
        void setStartPosition(float start_x, float start_y);
        void setStartTile(int row, int col);

        // Harm
        int getHarm() const;
        void setHarm(int health_points);
    };

}

#endif //ENGINE_LIBRARY_HPP
