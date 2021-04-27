#include "library.hpp"

/* Util */

// Helper to locate a value within a std::vector object
// Returns -1 if not found
template<typename V>
int td::Util::find(std::vector<V> vector, V val) {
    auto it = std::find(vector.begin(), vector.end(), val);
    if (it != vector.end()) return it - vector.begin();
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
sf::RectangleShape td::Shapes::rect(float x, float y, int width, int height, sf::Color color) {
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
    return td::Shapes::rect((float)(tile_size * this->col), (float)(tile_size * this->row), tile_size, tile_size);
}

// Get the tile's sprite
sf::RectangleShape td::Tile::getSprite(td::SpriteSheet& sprite_sheet, int tile_size) const {
    sf::RectangleShape t = this->getRect(tile_size);
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
sf::Vector2i td::Tile::getPosition(int tile_size) const {
    return {this->col * tile_size, this->row * tile_size};
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
    this->tile_size = td::Tile::DEFAULT_TILE_SIZE;
    this->max_allowed_tile_size = 1000;
    this->tile_types = {
            {td::Map::TileTypes::WALL, {'w'}},
            {td::Map::TileTypes::START, {'s'}},
            {td::Map::TileTypes::CHECKPOINT, {'c'}},
            {td::Map::TileTypes::END, {'e'}},
            {td::Map::TileTypes::DOOR, {'d'}},
            {td::Map::TileTypes::KEY, {'k'}}
    };
    this->player_start_row = 0;
    this->player_start_col = 0;
}

// Read in a file path for the game map and translate it to a 2D vector
void td::Map::readMap(const std::string &path) {
    // Read in a file
    std::ifstream mapFile;
    mapFile.open(path);

    // Boolean to enforce that only one start tile is specified in the map
    bool start_tile_set = false;

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

            // Make a tile at this location
            td::Tile tile = td::Tile(sprite_id, type_id, r, (int)c/2);
            // Check if this is a starting tile. If so, mark it. Only one start tile allowed
            if (td::Util::find(this->getTileType(td::Map::TileTypes::START), type_id) != -1) {
                if (start_tile_set) {
                    throw std::invalid_argument("Error: Multiple starting positions given. Only one allowed.");
                }
                this->player_start_row = r;
                this->player_start_col = (int)c/2;
                start_tile_set = true;
            }
            this->map[r].emplace_back(tile);
        }
        r++;
    }
    mapFile.close();
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

// Display the map's enemies
void td::Map::drawEnemies(sf::RenderTarget *target) {
    for (const auto& enemy: this->enemies) {
        enemy.draw(target);
    }
}

// Get the tile size
int td::Map::getTileSize() const {
    return this->tile_size;
}

// Retrieve the tile at a given x and y location
td::Tile td::Map::getTile(float x, float y) {
    int r = (int)(y/(float)this->tile_size);
    int c = (int)(x/(float)this->tile_size);
    return this->map[r][c];
}

// Return a copy of the map
std::vector<std::vector<td::Tile>> td::Map::getMap() {
    return this->map;
}

// Get the char tile type_id from an integer tile type
std::vector<char> td::Map::getTileType(int type) {
    return this->tile_types[type];
}

// Get player starting row and column
td::Tile td::Map::getPlayerStartTile() {
    return this->map[this->player_start_row][this->player_start_col];
}

// Get map size, in pixels by default, or by rows and columns if 'true' passed
sf::Vector2i td::Map::getMapSize(bool rows_cols) {
    int num_rows = this->map.size();
    int num_cols = this->map[0].size();
    if (rows_cols)
        return {num_cols, num_rows};
    else
        return {num_cols * this->tile_size, num_rows * this->tile_size};
}

// Get the map's enemies
std::vector<td::Enemy> td::Map::getEnemies() {
    return this->enemies;
}

// Set the map's sprite sheet, used to determine what to draw at each tile
void td::Map::setSpriteSheet(const td::SpriteSheet& sheet) {
    this->sprite_sheet = sheet;
}

// Set the size of the map tiles, in pixels
void td::Map::setTileSize(int size) {
    if (size < 0 || size > this->max_allowed_tile_size) {
        throw std::invalid_argument("Invalid tile size!");
    }
    this->tile_size = size;
}

// Set a special tile type, whether by overwriting a default one or creating a new one
void td::Map::setTileType(int type, std::vector<char> type_ids) {
    this->tile_types[type] = std::move(type_ids);
}

// Add an enemy to the map
void td::Map::addEnemy(const td::Enemy& enemy) {
    this->enemies.emplace_back(enemy);
}

// Checks if the player is colliding with any tiles of a certain type
bool td::Map::collides(td::Map& map, const std::vector<char>& type_ids, const sf::RectangleShape& rect) {
    return !td::Map::getCollisions(map, type_ids, rect).empty();
}

// Get all tiles of certain types that the player is colliding with.
// Look at all tiles around a given location (x,y) and check if the current location
//  intersects with any surrounding tiles of a specific type.
// Looks in a 3x3 tile grid around the location given, with center at the location's top-left corner tile
std::vector<td::Tile>
td::Map::getCollisions(td::Map &map, const std::vector<char> &type_ids, const sf::RectangleShape &rect) {
    std::vector<td::Tile> tiles = std::vector<td::Tile>();
    // Get the tile at the location's top-left corner
    td::Tile current_tile = map.getTile(rect.getPosition().x, rect.getPosition().y);
    // Setup to look at the tiles around (and at) this current tile, in a 3x3 grid if possible
    // If the location is at the edge of the map, then reduce the size of the search to avoid Index errors
    int row = current_tile.row;
    int col = current_tile.col;
    int search_size_row = std::ceil(rect.getSize().y / (float)map.getTileSize());
    int search_size_col = std::ceil(rect.getSize().x / (float)map.getTileSize());
    int r_start = row-search_size_row; if (r_start < 0) r_start = row;
    int c_start = col-search_size_col; if (c_start < 0) c_start = col;
    int r_end = row+search_size_row; if (r_end > map.getMapSize(true).y) r_end = row;
    int c_end = col+search_size_col; if (c_end > map.getMapSize(true).x) c_end = col;

    // Now iterate over the tiles, checking if the given location's bounding box intersects one of them
    for (int r=r_start; r<=r_end; r++) {
        for (int c=c_start; c<=c_end; c++) {
            td::Tile tile = map.getMap()[r][c];
            if ((td::Util::find(type_ids, tile.type_id) != -1) &&
                rect.getGlobalBounds().intersects(tile.getRect(map.getTileSize()).getGlobalBounds())) {
                tiles.emplace_back(tile);  // Collision!
            }
        }
    }
    return tiles;
}
//------------------------------------------------------------------------------------------------------------------


/* Player */

// Constructor
td::Player::Player() {
    // Position
    this->x = 0;
    this->y = 0;

    // Size
    this->width = td::Tile::DEFAULT_TILE_SIZE;
    this->height = td::Tile::DEFAULT_TILE_SIZE;

    // Color
    this->color = sf::Color::White;

    // Movement
    this->up_key = sf::Keyboard::Up;
    this->left_key = sf::Keyboard::Left;
    this->down_key = sf::Keyboard::Down;
    this->right_key = sf::Keyboard::Right;
    this->speed = 50;

    // Gameplay
    this->max_health = 100;
    this->health = this->max_health;
    this->inventory = std::vector<char>();
    this->checkpoint = td::Tile('0', this->map.getTileType(td::Map::TileTypes::CHECKPOINT).front(), 0, 0);
}
// Destructor
td::Player::~Player() = default;

// Set the map that the player will roam around
void td::Player::setMap(td::Map &m) {
    this->map = m;
    this->checkpoint = m.getPlayerStartTile();
    this->spawn();
}

// Draw the player
void td::Player::draw(sf::RenderTarget* target) const {
    sf::RectangleShape rect = td::Shapes::rect(this->x, this->y,this->width, this->height);
    rect.setFillColor(this->color);
    target->draw(rect);
}

// Set the player's color
void td::Player::setColor(sf::Color c) {
    this->color = c;
}

// Listen for player movement
// Upon correct key presses, update the player's position
// Passed a time delta to ensure consistent speeds across all fps values
void td::Player::move(float elapsed) {
    float new_x = this->x;
    float new_y = this->y;

    // Handle keyboard inputs:
    // Each handler section optimistically sets the new coordinate position.
    // If that position turns out to intersect with a wall, then instead move the remaining distance.
    // This has the effect of clamping the position to align with the tile size.
    if (sf::Keyboard::isKeyPressed(this->up_key)) {     // UP
        new_y = this->y - (this->speed * elapsed);
        sf::RectangleShape rect = td::Shapes::rect(new_x, new_y, this->width, this->height);
        if (td::Map::collides(this->map, this->map.getTileType(td::Map::TileTypes::WALL), rect))
            new_y = std::floor(this->y) - (float)((int)this->y % this->map.getTileSize());
    }
    if (sf::Keyboard::isKeyPressed(this->down_key)) {   // DOWN
        new_y = this->y + (this->speed * elapsed);
        sf::RectangleShape rect = td::Shapes::rect(new_x, new_y, this->width, this->height);
        if (td::Map::collides(this->map, this->map.getTileType(td::Map::TileTypes::WALL), rect))
            new_y = std::floor(this->y) + ((float)((int)(this->map.getTileSize() - ((int)(this->y + (float)this->height) % this->map.getTileSize())) % this->map.getTileSize()));
    }
    if (sf::Keyboard::isKeyPressed(this->left_key)) {   // LEFT
        new_x = this->x - (this->speed * elapsed);
        sf::RectangleShape rect = td::Shapes::rect(new_x, new_y, this->width, this->height);
        if (td::Map::collides(this->map, this->map.getTileType(td::Map::TileTypes::WALL), rect))
            new_x = std::floor(this->x) - (float)((int)this->x % this->map.getTileSize());
    }
    if (sf::Keyboard::isKeyPressed(this->right_key)) {  // RIGHT
        new_x = this->x + (this->speed * elapsed);
        sf::RectangleShape rect = td::Shapes::rect(new_x, new_y, this->width, this->height);
        if (td::Map::collides(this->map, this->map.getTileType(td::Map::TileTypes::WALL), rect))
            new_x = std::floor(this->x) + ((float)((int)(this->map.getTileSize() - ((int)(this->x + (float)this->width) % this->map.getTileSize())) % this->map.getTileSize()));
    }

    this->x = new_x;
    this->y = new_y;
}

// Alter the default player movement keys
void td::Player::setMovementKeys(sf::Keyboard::Key up, sf::Keyboard::Key left, sf::Keyboard::Key down,
                                 sf::Keyboard::Key right) {
    this->up_key = up;
    this->left_key = left;
    this->down_key = down;
    this->right_key = right;
}

// Set the player's speed
void td::Player::setMoveSpeed(float move_speed) {
    this->speed = move_speed;
}

// Set the player's position according to the starting point specified on the map
// To be called at the start of a level
void td::Player::spawn() {
    this->health = this->max_health;
    td::Tile tile = this->map.getPlayerStartTile();
    sf::Vector2i pos = tile.getPosition(this->map.getTileSize());
    this->x = pos.x + ((float)(this->map.getTileSize()-this->width)/2);
    this->y = pos.y + ((float)(this->map.getTileSize()-this->height)/2);
}

// Set the player's position according to the latest checkpoint the player reached
void td::Player::respawn() {
    this->health = this->max_health;
    sf::Vector2i pos = this->checkpoint.getPosition(this->map.getTileSize());
    this->x = pos.x + ((float)(this->map.getTileSize()-this->width)/2);
    this->y = pos.y + ((float)(this->map.getTileSize()-this->height)/2);
}

// Get the player's position
// Optionally set 'center' to true to get the position from the player's center
sf::Vector2f td::Player::getPosition(bool center) const {
    if (center)
        return {this->x + ((float)this->width/2), this->y + ((float)this->height/2)};
    return {this->x, this->y};
}

// Check if the player is currently colliding with a checkpoint tile
bool td::Player::onCheckpoint() {
    sf::RectangleShape p_rect = td::Shapes::rect(this->x, this->y, this->width, this->height);
    return td::Map::collides(this->map, this->map.getTileType(td::Map::TileTypes::CHECKPOINT), p_rect);
}

// Check if the player is currently colliding with a checkpoint tile.
// Looks around for a checkpoint tile that the player is touching.
//  If it finds one, it sets the player's checkpoint variable to that tile.
//  If not, it sets the player's checkpoint tile at the current tile.
// Supports both formal checkpoints and informal save points.
void td::Player::setCheckpoint() {
    sf::RectangleShape p_rect = td::Shapes::rect(this->x, this->y, this->width, this->height);
    std::vector<td::Tile> checkpoints = td::Map::getCollisions(this->map, this->map.getTileType(td::Map::TileTypes::CHECKPOINT), p_rect);
    if (!checkpoints.empty()) {
        this->checkpoint = checkpoints.back();
    }
    else {
        this->checkpoint = this->map.getTile(this->x, this->y);
    }
}

// Get the player's width and height
td::Util::size td::Player::getSize() const {
    return {this->width, this->height};
}

// Set the player's width and height
void td::Player::setSize(int w, int h, bool center_in_tile) {
    this->width = w;
    this->height = h;
    if (center_in_tile) {
        this->x += ((float)(this->map.getTileSize()-this->width)/2);
        this->y += ((float)(this->map.getTileSize()-this->height)/2);
    }
}

// Check if the player is currently colliding with an enemy
bool td::Player::isTouchingEnemy() {
    return !this->getTouchingEnemies().empty();
}

// Get the enemy that the player is touching
std::vector<td::Enemy> td::Player::getTouchingEnemies() {
    std::vector<td::Enemy> touching_enemies = std::vector<td::Enemy>();

    sf::RectangleShape p_rect = td::Shapes::rect(this->x, this->y, this->width, this->height);
    for (const auto& enemy : this->map.getEnemies()) {
        sf::RectangleShape enemy_rect = td::Shapes::rect(
                enemy.getPosition().x, enemy.getPosition().y, enemy.getSize().width, enemy.getSize().height);
        if (p_rect.getGlobalBounds().intersects(enemy_rect.getGlobalBounds())) {
            touching_enemies.emplace_back(enemy);
        }
    }
    return touching_enemies;
}

// Get the player's health
int td::Player::getHealth() const {
    return this->health;
}

// Set the player's health
void td::Player::setHealth(int h) {
    this->health = h;
}

// Set the player's maximum health
void td::Player::setMaxHealth(int mh) {
    this->max_health = mh;
}

// Decrement the player's health by a given amount
void td::Player::loseHealth(int health_points) {
    this->health -= health_points;
}

// Increment the player's health by a given amount
void td::Player::gainHealth(int health_points) {
    this->health += health_points;
}

// Player health <= 0
bool td::Player::isDead() const {
    return this->health <= 0;
}
//------------------------------------------------------------------------------------------------------------------


/* Enemy */

// Constructor/destructor
td::Enemy::Enemy() {
    this->harm = 1;
}
td::Enemy::~Enemy() = default;

// Set the map that the enemy will be on
// Overwrites base Player setMap() so that the call to spawn() is omitted
void td::Enemy::setMap(td::Map& m) {
    this->map = m;
}

// Set starting x and y
void td::Enemy::setStartPosition(float start_x, float start_y) {
    this->x = start_x;
    this->y = start_y;
}

// Set starting tile row and column
void td::Enemy::setStartTile(int row, int col) {
    this->x = (float)(col * this->map.getTileSize()) + ((float)(this->map.getTileSize()-this->width)/2);
    this->y = (float)(row * this->map.getTileSize()) + ((float)(this->map.getTileSize()-this->height)/2);
}

// Get the amount of damage that the enemy does
int td::Enemy::getHarm() const {
    return this->harm;
};

// Set the amount of damage that the enemy does
void td::Enemy::setHarm(int health_points) {
    this->harm = health_points;
}
