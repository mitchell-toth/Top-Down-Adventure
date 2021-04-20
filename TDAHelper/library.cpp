#include "library.hpp"


/* Util */

// Helper to locate a value within a std::vector object
// Returns -1 if not found
template<typename V>
int td::Util::find(std::vector<V> vector, V val) {
    auto it = std::find(vector.begin(), vector.end(), val);
    if (it != vector.end()) {
        return it - vector.begin();
    }
    return -1;
}

// Helper to check if a key exists within a std::map object
template<typename K, typename V>
bool td::Util::keyInMap(std::map<K, V> map, K key) {
    if (map.count(key) != 0) return true;
    return false;
}
//------------------------------------------------------------------------------------------------------------------


/* Text */

// Helper to print text to the screen. Takes a window target, a text string, and a configuration object.
void td::Text::print(sf::RenderTarget* target, const std::string &s, const td::Text::Config& config) {
    // Set up the text, size, and color
    sf::Text text = sf::Text();
    text.setFont(config.font);
    text.setString(s);
    text.setCharacterSize(config.size);
    text.setFillColor(config.color);

    // Set horizontal alignment
    if (config.align == td::Text::Align::LEFT) {  // Align left
        text.setPosition((float)config.x, (float)config.y);
    }
    else if (config.align == td::Text::Align::CENTER) {  // Center text horizontally. Ignore the x passed in
        text.setPosition(
                (float)((target->getSize().x * 0.5) - (text.getLocalBounds().width * 0.5)), (float)config.y);
    }
    else {  // Align right
        text.setPosition((float)config.x - text.getLocalBounds().width, (float)config.y);
    }

    // Draw text to target window
    target->draw(text);
}
//------------------------------------------------------------------------------------------------------------------


/* Shapes */

// Construct and return a rectangle
sf::RectangleShape td::Shapes::rect(int x, int y, int width, int height, sf::Color color) {
    sf::RectangleShape rect;
    rect.setPosition(sf::Vector2f(x, y));
    rect.setSize(sf::Vector2f(width, height));
    rect.setFillColor(color);
    return rect;
}

// Construct and return a line
sf::VertexArray td::Shapes::line(int x1, int y1, int x2, int y2, sf::Color color) {
    sf::VertexArray l(sf::LinesStrip, 2);
    l[0].position = sf::Vector2f(x1, y1);
    l[0].color  = sf::Color(color);
    l[1].position = sf::Vector2f(x2, y2);
    l[1].color  = sf::Color(color);
    return l;
}
//------------------------------------------------------------------------------------------------------------------


/* Tile */

// Constructor
td::Tile::Tile() = default;
// Destructor
td::Tile::~Tile() = default;

// Constructor + params
td::Tile::Tile(char sprite_id, char type_id, int row, int col) {
    this->sprite_id = sprite_id;
    this->type_id = type_id;
    this->row = row;
    this->col = col;
}


/* SpriteSheet */

// Constructor
td::SpriteSheet::SpriteSheet() = default;
// Destructor
td::SpriteSheet::~SpriteSheet() = default;

// Add a sprite to the sprite sheet
// Creates a sprite with the given texture and maps it to the given ID
void td::SpriteSheet::addSprite(char id, sf::Color color) {
    sf::Sprite sprite = sf::Sprite();
    sprite.setColor(color);
    this->mapping[id] = sprite;
}
//------------------------------------------------------------------------------------------------------------------


/* Map */

// Constructor
td::Map::Map() {
    this->initVariables();
}
// Destructor
td::Map::~Map() = default;

// Constructor + params
td::Map::Map(const std::string& path) {
    this->initVariables();
    this->readMap(path);
}

// Initialize map attributes
void td::Map::initVariables() {
    this->tile_size = 50;
    this->max_allowed_tile_size = 1000;
}

// Read in a file path for the game map and translate it to a 2D vector.
void td::Map::readMap(const std::string &path) {
    // Read in a file
    std::ifstream mapFile;
    mapFile.open(path);

    // Create the raw map from the supplied map file
    // Also create the game's tile map with encoded information
    std::string line;
    int r = 0;
    while (std::getline(mapFile, line)) {
        this->map_raw.emplace_back(std::vector<char>());
        this->map.emplace_back(std::vector<td::Tile>());
        for (int c=0; c<line.length(); c+=2) {
            char sprite = line[c];
            char type = line[c+1];
            this->map_raw[r].emplace_back(sprite);
            this->map_raw[r].emplace_back(type);

            td::Tile t = td::Tile(sprite, type, r, c);
            this->map[r].emplace_back(t);
        }
        r++;
    }
    mapFile.close();
}

// Set the map's sprite sheet, used to determine what to draw at each tile
void td::Map::setSpriteSheet(const td::SpriteSheet& sheet) {
    this->sprite_sheet = sheet;
}

// Print out the raw map for debugging purposes
void td::Map::printMap() {
    for (const auto& row : this->map_raw) {
        for (char val : row) {
            std::cout << val;
        }
        std::cout << std::endl;
    }
}

// Display the map in the game window
void td::Map::renderMap(sf::RenderTarget* target) {
    for (int r=0; r<this->map.size(); r++) {
        std::vector<td::Tile> row = this->map[r];
        for (int c=0; c<row.size(); c++) {
            // Grab the tile at this row and column
            td::Tile tile = row[c];
            // Create a corresponding graphical tile
            sf::RectangleShape t = td::Shapes::rect(this->tile_size*c, this->tile_size*r,
                                                       this->tile_size, this->tile_size);

            // If the tile's sprite ID is mapped to a sprite, draw it
            // Otherwise, skip this tile and draw nothing
            if (td::Util::keyInMap(this->sprite_sheet.mapping, tile.sprite_id)) {
                sf::Sprite sprite = this->sprite_sheet.mapping[tile.sprite_id];
                t.setFillColor(sprite.getColor());
                target->draw(t);
            }
        }
    }
}

// Set the size of the map tiles, in pixels
void td::Map::setTileSize(int size) {
    if (size < 0 || size > this->max_allowed_tile_size) {
        throw std::invalid_argument("Invalid tile size!");
    }
    this->tile_size = size;
}
