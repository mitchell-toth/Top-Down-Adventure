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

// Get the tile's rectangle object
sf::RectangleShape td::Tile::getRect(int tile_size) const {
    return td::Shapes::rect(tile_size*this->col, tile_size*this->row, tile_size, tile_size);
}

// Get the tile's sprite
sf::RectangleShape td::Tile::getSprite(td::SpriteSheet& sprite_sheet, int tile_size) const {
    sf::RectangleShape t = td::Shapes::rect(tile_size*this->col, tile_size*this->row, tile_size, tile_size);
    // If the tile's sprite ID is mapped to a sprite, draw it
    // Otherwise, skip this tile and draw nothing (a transparent rect)
    if (td::Util::keyInMap(sprite_sheet.mapping, this->sprite_id)) {
        sf::Sprite sprite = sprite_sheet.mapping[this->sprite_id];
        t.setFillColor(sprite.getColor());
        return t;
    }
    t.setFillColor(sf::Color::Transparent);
    return t;
}

// Get the x and y locations of the tile
std::vector<int> td::Tile::getPosition(int tile_size) const {
    return std::vector<int>({this->col*tile_size, this->row*tile_size});
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
            char sprite_id = line[c];
            char type_id = line[c+1];
            this->map_raw[r].emplace_back(sprite_id);
            this->map_raw[r].emplace_back(type_id);

            td::Tile t = td::Tile(sprite_id, type_id, r, (int)c/2);
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
void td::Map::draw(sf::RenderTarget* target) {
    for (const auto& row : this->map) {
        for (const auto& tile : row) {
            // Create the corresponding graphical tile and display it
            target->draw(tile.getSprite(this->sprite_sheet, this->tile_size));
        }
    }
}

// Get the size of map tiles, in pixels
int td::Map::getTileSize() const {
    return this->tile_size;
}

// Set the size of the map tiles, in pixels
void td::Map::setTileSize(int size) {
    if (size < 0 || size > this->max_allowed_tile_size) {
        throw std::invalid_argument("Invalid tile size!");
    }
    this->tile_size = size;
}

// Retrieve the tile at a given x and y location
td::Tile td::Map::getTile(int x, int y) {
    int r = (int)(y/this->tile_size);
    int c = (int)(x/this->tile_size);
    return this->map[r][c];
}

// Return a copy of the map
std::vector<std::vector<td::Tile>> td::Map::getMap() {
    return this->map;
}
//------------------------------------------------------------------------------------------------------------------


/* Player */

// Constructor
td::Player::Player() {
    this->x = 0;
    this->y = 0;

    this->up_key = sf::Keyboard::W;
    this->down_key = sf::Keyboard::S;
    this->left_key = sf::Keyboard::A;
    this->right_key = sf::Keyboard::D;

    this->max_health = 100;
    this->health = this->max_health;
    this->inventory = std::vector<char>();
}
// Destructor
td::Player::~Player() = default;

// Draw the player
void td::Player::draw(sf::RenderTarget* target) const {
    sf::RectangleShape rect = td::Shapes::rect((int)this->x, (int)this->y, 40, 40);
    rect.setFillColor(sf::Color::Yellow);
    target->draw(rect);
}

// Listen for player movement
// Upon correct key presses, update the player's position
void td::Player::move(td::Map& map) {
    // Get time delta
    float elapsed = clock.restart().asSeconds();
    float speed = 50;

    float new_x = this->x;
    float new_y = this->y;

    // Handle keyboard inputs
    if (sf::Keyboard::isKeyPressed(this->up_key))
        new_y -= speed * elapsed;
    if (sf::Keyboard::isKeyPressed(this->down_key))
        new_y += speed * elapsed;
    if (sf::Keyboard::isKeyPressed(this->left_key))
        new_x -= speed * elapsed;
    if (sf::Keyboard::isKeyPressed(this->right_key))
        new_x += speed * elapsed;

    // Handle collisions with walls
    // If there's no collision, then move the player. Otherwise, ignore the movement
    if (!td::Player::collides(map, 'w', (int)this->x, (int)new_y))
        this->y = new_y;
    if (!td::Player::collides(map, 'w', (int)new_x, (int)this->y))
        this->x = new_x;
}


// Look at all tiles around a given location (x,y) and check if the current location
//  intersects with any surrounding tiles of a specific type.
// Looks in a 3x3 tile grid around the location given, with center at the location's top-left corner tile
bool td::Player::collides(td::Map& map, char type_id, int x, int y) {
    // Get the location's bounding box
    sf::RectangleShape p_rect = td::Shapes::rect(x, y, map.getTileSize(), map.getTileSize());

    // Get the tile at the location's top-left corner
    td::Tile current_tile = map.getTile(x, y);
    // Setup to look at the tiles around (and at) this current tile, in a 3x3 grid if possible
    // If the location is at the edge of the map, then reduce the size of the search to avoid Index errors
    int row = current_tile.row;
    int col = current_tile.col;
    int r_start = row-1; if (row == 0) r_start = row;
    int c_start = col-1; if (col == 0) c_start = col;
    int r_end = row+1; if (row == map.getMap().size()-1) r_end = row;
    int c_end = col+1; if (col == map.getMap()[0].size()-1) c_end = col;

    // Now iterate over the tiles, checking if the given location's bounding box intersects one of them
    for (int r=r_start; r<=r_end; r++) {
        for (int c=c_start; c<=c_end; c++) {
            td::Tile tile = map.getMap()[r][c];
            if (tile.type_id == 'w' && p_rect.getGlobalBounds().intersects(tile.getRect(map.getTileSize()).getGlobalBounds())) {
                return true;  // Collision!
            }
        }
    }
    return false;  // No collision
}

// Set the player's position to be a specific map row and column
void td::Player::setPosition(td::Map& map, int row, int col) {
    td::Tile tile = map.getMap()[row][col];
    std::vector<int> pos = tile.getPosition(map.getTileSize());
    this->x = pos[0];
    this->y = pos[1];
}
