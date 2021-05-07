#ifndef ENGINE_LIBRARY_HPP
#define ENGINE_LIBRARY_HPP

/**
 * @file library.h
 * @brief The TDAHelper library declaration file containing the td namespace.
 * @authors Mitchell Toth, Jacob Haimes
 * @date May 2021
 */

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

/**
 * @namespace td
 * @brief The namespace in which all TDAHelper functionality and types are scoped.
 */
namespace td {
    // Forward declarations
    class Enemy;
    class Item;

    // Classes:
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @class Util
     * @brief A static collection of helpful utility methods and types.
     */
    class Util {
    public:
        template <typename V>
        static int find(std::vector<V> vector, V val);
        template<typename K, typename V>
        static bool keyInMap(std::map<K, V> map, K key);
        /**
         * @struct size
         * @brief Helpful struct for storing a pair of width and height values.
         */
        struct size {
            int width{0};
            int height{0};
        };
        static float dist(float x1, float y1, float x2, float y2);
    };
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @class Text
     * @brief Useful types and a print utility to conveniently display text to the screen.
     */
    class Text {
    public:
        /**
         * @enum Align
         * @brief Text alignment, choice of LEFT, CENTER, or RIGHT.
         */
        enum Align {
            LEFT = -1,
            CENTER = 0,
            RIGHT = 1
        };
        /**
         * @struct Config
         * @brief Text configuration, including font, position, size, alignment, and color.
         * Default values: x=0, y=0, size=12, align=td::Text::Align::LEFT, color=sf::Color::WHITE
         */
        struct Config {
            sf::Font font{};
            int x{0};
            int y{0};
            int size{12};
            Align align{Align::LEFT};
            sf::Color color{sf::Color::White};
        };
        static void print(sf::RenderTarget* target, const std::string& s, const Config& config, bool relativeToView=true);
    };
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @class Music
     * @brief Utility for creating and playing instances of SFML music. Great for larger sound files.
     * The underlying SFML music object is made public to allow for greatest use and flexibility.
     */
    class Music {
    public:
        // Constructor/destructor
        Music();
        explicit Music(const std::string& path, bool loop=true, float volume=100.f, float pitch=1.f);
        ~Music();

        sf::Music music;

        void play();
        void stop();
    };
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @class Sound
     * @brief Utility for creating and playing instances of SFML sound effects. Better for smaller sound files.
     * The underlying SFML sound object is made public to allow for greatest use and flexibility.
     */
    class Sound {
    public:
        // Constructor/destructor
        Sound();
        explicit Sound(const std::string& path, float volume=100.f, float pitch=1.f);
        ~Sound();

        sf::SoundBuffer buffer;
        sf::Sound sound;

        void play();
        void stop();
    };
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @class Shapes
     * @brief Utility for creating common shape objects.
     */
    class Shapes {
    public:
        static sf::RectangleShape rect(float x, float y, int width, int height, sf::Color color = sf::Color::White);
        static sf::CircleShape circ(float x, float y, float radius, sf::Color color = sf::Color::White);
        static sf::VertexArray line(int x1, int y1, int x2, int y2, sf::Color color = sf::Color::White);
    };
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @class ClickableMenu
     * @brief Utility for quick creation of clickable button menus, capable of a 2D grid button layout.
     */
    class ClickableMenu {
    private:
        // Window
        sf::RenderWindow* target{};

        // Options
        std::vector<std::vector<std::string>> menuItems;

        // Text
        td::Text::Config textConfig;

        // Position
        float x{};
        float y{};

        // Styling
        std::vector<int> padding;
        std::vector<std::vector<sf::Color>> optionColors;

        // Absolute button width and height, optional
        int buttonWidth{};
        int buttonHeight{};

        // Highlighting rectangles
        std::vector<std::vector<sf::RectangleShape>> menuItemRects;

        // Colors
        sf::Color onHoverColor;
        sf::Color outlineColor;
        int outlineThickness{};
    public:
        //Constructor/destructor
        ClickableMenu();
        ClickableMenu(sf::RenderWindow* target, float start_x, float start_y, const std::vector<int>& padding,
                      std::vector<std::vector<std::string>> menuItems,
                      const td::Text::Config& textConfig);
        ~ClickableMenu();

        void initVariables();
        void createOnHoverRectangles();

        // Getters
        std::vector<std::vector<std::string>> getMenuItems();
        sf::Vector2f getPosition() const;

        // Setters
        void setRenderWindow(sf::RenderWindow* rw);
        void setPosition(float start_x, float start_y);
        void setPadding(const std::vector<int>& p);
        void setMenuItems(const std::vector<std::vector<std::string>>& items);
        void setTextConfig(const td::Text::Config& config);
        void setButtonSize(int width, int height);
        void setOptionColors(const std::vector<std::vector<sf::Color>>& colors);

        // Colors
        void setOnHoverColor(sf::Color color);
        void setOutline(sf::Color color, int thickness);

        // Render
        void drawMenu();
        void onMouseOver();

        // Selection
        std::string onMouseClick();
    };
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @class SpriteSheet
     * @brief Defines a mapping between rectangle shapes and textures.
     */
    class SpriteSheet {
    public:
        // Constructor/destructor
        SpriteSheet();
        ~SpriteSheet();

        std::map<char, sf::RectangleShape> mapping;
        void addSprite(char id, sf::Color color);
        void addTexture(char id, const std::string& file);
    };
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @class Tile
     * @brief An individual tile within a tile grid.
     * Stores information about sprite_id (what sprite is on the tile)
     * and type_id (the tile's type and how the tile should behave).
     */
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

    /**
     * @class Map
     * @brief A tile grid map composed of Tile objects.
     * Stores key game information such as enemies and items.
     * Also handles crucial functions like player-tile collision detection.
     */
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
        int player_start_row{};
        int player_start_col{};

        // Enemies
        std::vector<td::Enemy*> enemies;

        // Items
        std::vector<td::Item*> items;

        // Initialization
        void initVariables();
    public:
        // Constructor/destructor
        Map();
        explicit Map(const std::string& path);
        ~Map();

        /**
         * @enum TileTypes
         * @brief Special tile types.
         */
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
        void drawItems(sf::RenderTarget* target);

        // Getters
        int getTileSize() const;
        td::Tile getTile(float x, float y);
        std::vector<std::vector<td::Tile>> getMap();
        std::vector<char> getTileType(int type);
        td::Tile getPlayerStartTile();
        sf::Vector2i getMapSize(bool rows_cols = false);
        std::vector<td::Enemy*>* getEnemies();
        std::vector<td::Item*>* getItems();

        // Setters
        void setSpriteSheet(const td::SpriteSheet& sheet);
        void setTileSize(int size);
        void setTileType(int type, std::vector<char> type_id);

        // Enemies
        void addEnemy(td::Enemy* enemy);
        void moveEnemies(float elapsed);
        void resetEnemies();

        // Items
        void addItem(td::Item* item);
        void resetItems();


        // Collision
        static bool collides(td::Map& map, const std::vector<char>& type_ids, const sf::RectangleShape& rect);
        static std::vector<td::Tile> getCollisions(td::Map& map, const std::vector<char>& type_ids, const sf::RectangleShape& rect);
    };
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @class RenderObject
     * @brief Base class for objects that are displayed on a Map instance.
     * Defines base methods and attributes such as position, texture, and size that are shared between map objects.
     */
    class RenderObject {
    protected:
        // Position
        float x;
        float y;

        // Size
        int width;
        int height;

        // Color/texture
        sf::Color color;
        sf::Texture* texture;

        // Map
        td::Map map;

        // Render
        sf::RectangleShape drawable;
    public:
        RenderObject();
        ~RenderObject();

        // Map
        virtual void setMap(td::Map& m);

        // Position
        sf::Vector2f getPosition(bool center=false) const;
        virtual void setStartPosition(float start_x, float start_y);
        virtual void setStartTile(int row, int col);

        // Render
        virtual void draw(sf::RenderTarget* target);
        void setColor(sf::Color c);
        void setTexture(const std::string& file);

        // Size
        td::Util::size getSize() const;
        void setSize(int w, int h, bool center_in_tile = false);
    };

    //------------------------------------------------------------------------------------------------------------------

    /**
     * @class Player
     * @brief The user-controlled player that can move around and explore Map instances.
     * Inherits from RenderObject.
     */
    class Player : public RenderObject {
    private:
        // Movement
        sf::Keyboard::Key up_key;
        sf::Keyboard::Key left_key;
        sf::Keyboard::Key down_key;
        sf::Keyboard::Key right_key;

        // Gameplay
        std::vector<td::Item*> inventory;
        td::Tile checkpoint;
    protected:
        // Movement
        float speed;

        // Gameplay
        int health;
        int max_health;
    public:
        // Constructor/destructor
        Player();
        ~Player();

        // Map
        void setMap(td::Map& m) override;

        // Movement
        virtual void move(float elapsed);
        void setMovementKeys(sf::Keyboard::Key up, sf::Keyboard::Key left, sf::Keyboard::Key down,
                             sf::Keyboard::Key right);
        void setMoveSpeed(float move_speed);

        // Position
        void spawn();
        void respawn();

        // Checkpoints
        bool onCheckpoint();
        void setCheckpoint();

        // End tiles
        bool onEnd();

        // Enemy interaction
        bool isTouchingEnemy();
        std::vector<td::Enemy*> getTouchingEnemies();
        bool isTouchingCircleEnemy();
        std::vector<td::Enemy*> getTouchingCircleEnemies();

        // Item interaction
        bool isTouchingItem();
        std::vector<td::Item*> getTouchingItems();
        void obtainItem(td::Item* item);

        // Inventory
        std::vector<td::Item*> getInventory();
        void clearInventory();
        void resetInventory();

        // Health
        int getHealth() const;
        void setHealth(int h);
        void setMaxHealth(int mh);
        void loseHealth(int health_points);
        void gainHealth(int health_points);
        bool isDead() const;
    };
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @class Enemy
     * @brief An entity that harms Player instances. Harm is inflicted when an enemy collides with a player.
     * Moves via a waypoint system.
     * Inherits from Player.
     */
    class Enemy : public Player {
    private:
        // Harm
        int harm;

        // Movement
        std::vector<sf::Vector2f> waypoints;
        int move_option{};
        unsigned int current_waypoint_index;
        int direction;
    public:
        // Constructor/destructor
        Enemy();
        Enemy(const td::Map& map, int width, int height, sf::Color color, int harm = 1);
        ~Enemy();

        void reset();

        // Map
        void setMap(td::Map& m) override;

        // Harm
        int getHarm() const;
        void setHarm(int health_points);

        /**
         * @enum MoveOptions
         * @brief Enemy waypoints move options.
         */
        enum MoveOptions {
            LOOP = 1,
            BACK_AND_FORTH = 2
        };
        void setWaypoints(const std::vector<sf::Vector2f>& enemy_waypoints, bool tiles=true);
        void setMoveOption(int enemy_move_option);
        void setStartPosition(float start_x, float start_y) override;
        void setStartTile(int row, int col) override;
        void move(float elapsed) override;
    };
    //------------------------------------------------------------------------------------------------------------------

    /**
     * @class Item
     * @brief An obtainable entity that may be collected by Player instances.
     * Each item has an obtained and committed status to track membership in a player's inventory.
     * Inherits from RenderObject.
     */
    class Item : public RenderObject {
    private:
        bool obtained;
        bool committed;
    public:
        Item();
        Item(const td::Map& map, int width, int height, sf::Color color);
        ~Item();

        void reset();

        // Render
        void draw(sf::RenderTarget* target) override;

        // Obtained status
        bool isObtained() const;
        void setObtained(bool item_obtained);

        // Committed status
        bool isCommitted() const;
        void setCommitted(bool item_committed);
    };
}

#endif //ENGINE_LIBRARY_HPP
