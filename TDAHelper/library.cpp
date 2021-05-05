#include "library.hpp"

#include <utility>

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

// Calculate the distance between two points
float td::Util::dist(float x1, float y1, float x2, float y2) {
    return (float)std::sqrt(std::pow(x2-x1, 2) +std::pow(y2-y1, 2));
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


/* Music */

// Constructor/destructor
td::Music::Music() = default;
td::Music::Music(const std::string &path, bool loop, float volume, float pitch) {
    if (!this->music.openFromFile(path))
        throw std::invalid_argument("Could not load audio at path " + path);
    this->music.setLoop(loop);
    this->music.setVolume(volume);
    this->music.setPitch(pitch);
}
td::Music::~Music() = default;

// Play the sound effect
void td::Music::play() {
    this->music.play();
}
//------------------------------------------------------------------------------------------------------------------


/* Sound */

// Constructor/destructor
td::Sound::Sound() = default;
td::Sound::Sound(const std::string &path, float volume, float pitch) {
    if (!this->buffer.loadFromFile(path))
        throw std::invalid_argument("Could not load audio at path " + path);
    this->sound.setBuffer(this->buffer);
    this->sound.setVolume(volume);
    this->sound.setPitch(pitch);
}
td::Sound::~Sound() = default;

// Play the sound effect
void td::Sound::play() {
    this->sound.play();
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


/* ClickableMenu */

// Constructor/destructor
td::ClickableMenu::ClickableMenu() {
    this->x = 0;
    this->y = 0;
    this->buttonWidth = 60;
    this->buttonHeight = 60;
}
td::ClickableMenu::ClickableMenu(sf::RenderWindow* target, int start_x, int start_y, int buttonWidth, int buttonHeight,
                                 std::vector<std::vector<const std::string&>> menuItems,
                                 const td::Text::Config& textConfig) {
    this->x = start_x;
    this->y = start_y;
    this->buttonWidth = buttonWidth;
    this->buttonHeight = buttonHeight;

    this->menuItems = std::move(menuItems);

    this->lastMouseoverOption = -1;

    this->menuItemRects = std::vector<sf::RectangleShape>(this->menuItems.size());
    //Create a vector of colored rectangles to go behind the menu options text
    for (int i=0; i<this->menuItems.size(); i++) {
        float rect_x, rect_y;
        if (this->textConfig.align == td::Text::Align::CENTER) {  // Center horizontally
            rect_x = (float)((this->target->getSize().x * 0.5) - 210);
            rect_y = (float)((i * this->buttonHeight) + (this->y - 5));
        }
        else {
            rect_x = (float)(this->x - 210);
            rect_y = (float)((i * this->buttonHeight) + (y - 5));
        }

        sf::RectangleShape rect = td::Shapes::rect(rect_x, rect_y, 420, this->buttonHeight);
        this->menuItemRects[i] = rect;
    }
}

td::ClickableMenu::~ClickableMenu()=default;

//Render the menu by printing the text options
void td::ClickableMenu::drawMenu(sf::RenderTarget* target) {
    for (int i=0; i<this->menuItems.size(); i++) {
        // print the button text
    }
}

//Check if the mouse is within any of the menu options rectangles. If so, draw the highlighted rectangle
void td::ClickableMenu::onMouseOver() {
    int mouse_x = sf::Mouse::getPosition(*this->target).x;  // Get mouse x
    int mouse_y = sf::Mouse::getPosition(*this->target).y;  // Get mouse y

    //Iterate over each rectangle to check if it contains mouse_x and mouse_y
    for (int i=0; i<this->menuItems.size(); i++) {
        if (this->menuItemRects[i].getGlobalBounds().contains(mouse_x, mouse_y)) {
            this->menuItemRects[i].setFillColor(sf::Color(140, 140, 140, 100));
            this->target->draw(this->menuItemRects[i]);

            //Play a sound if this is a new menu option hover
            if (this->lastMouseoverOption != i) {
                this->lastMouseoverOption = i;
            }
        }
    }
}

//Check if a mouse click occurred in a menu item. If so, return the corresponding state
int td::ClickableMenu::onMouseClick() {
    int mouse_x = sf::Mouse::getPosition(*this->target).x;  // Get mouse x
    int mouse_y = sf::Mouse::getPosition(*this->target).y;  // Get mouse y

    //Iterate over each rectangle to check if it contains mouse_x and mouse_y
    for (int i=0; i<this->numMenuItems; i++) {
        if (this->menuItemRects[i].getGlobalBounds().contains(mouse_x, mouse_y)) {
            return this->states[i];  // Return state corresponding to selected item
        }
    }
    return currentState;
}
//------------------------------------------------------------------------------------------------------------------


/* SpriteSheet */

// Constructor
td::SpriteSheet::SpriteSheet() = default;
// Destructor
td::SpriteSheet::~SpriteSheet() {
    for (auto const& key_val : this->mapping) {
        delete key_val.second.getTexture();
    }
}

// Add a sprite to the sprite sheet
// Creates a sprite with the given texture and maps it to the given ID
void td::SpriteSheet::addSprite(char id, sf::Color color) {
    sf::RectangleShape rect = sf::RectangleShape();
    rect.setFillColor(color);
    this->mapping[id] = rect;
}

// Add a sprite texture
void td::SpriteSheet::addTexture(char id, const std::string& file) {
    auto* texture = new sf::Texture();
    if (!texture->loadFromFile(file)) {
        throw std::invalid_argument("Could not load texture at path " + file);
    }
    sf::RectangleShape rect;
    rect.setTexture(texture);
    this->mapping[id] = rect;
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
        sf::RectangleShape sprite = sprite_sheet.mapping[this->sprite_id];
        if(sprite.getTexture() == nullptr) {
            t.setFillColor(sprite.getFillColor());
            return t;
        } else {
            t.setTexture(sprite.getTexture());
            return t;
        }
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
    this->enemies = std::vector<td::Enemy*>();
    this->items = std::vector<td::Item*>();
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
                    throw std::invalid_argument("Multiple starting positions given. Only one allowed.");
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
    for (auto& enemy: this->enemies) {
        enemy->draw(target);
    }
}

void td::Map::drawItems(sf::RenderTarget *target) {
    for (auto item: this->items) {
        item->draw(target);
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
std::vector<td::Enemy*>* td::Map::getEnemies() {
    return &this->enemies;
}

// Get the map's items
std::vector<td::Item*>* td::Map::getItems() {
    return &this->items;
}

// Set the map's sprite sheet, used to determine what to draw at each tile
void td::Map::setSpriteSheet(const td::SpriteSheet& sheet) {
    this->sprite_sheet = sheet;
}

// Set the size of the map tiles, in pixels
void td::Map::setTileSize(int size) {
    if (size < 0 || size > this->max_allowed_tile_size) {
        throw std::invalid_argument("Invalid tile size.");
    }
    this->tile_size = size;
}

// Set a special tile type, whether by overwriting a default one or creating a new one
void td::Map::setTileType(int type, std::vector<char> type_ids) {
    this->tile_types[type] = std::move(type_ids);
}

// Add an enemy to the map
void td::Map::addEnemy(td::Enemy* enemy) {
    this->enemies.emplace_back(enemy);
}

// Move all enemies
void td::Map::moveEnemies(float elapsed) {
    for (auto enemy : this->enemies) {
        enemy->move(elapsed);
    }
}

// Reset all enemies to their starting locations
void td::Map::resetEnemies() {
    for (auto enemy : this->enemies) {
        enemy->reset();
    }
}

// Add an item to the map
void td::Map::addItem(td::Item* item) {
    this->items.emplace_back(item);
}

// Reset all items to be un-obtained
void td::Map::resetItems() {
    for (auto item : this->items) {
        item->reset();
    }
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

/* RenderObject */

// Constructor/destructor
td::RenderObject::RenderObject() {
    // Position
    this->x = 0;
    this->y = 0;

    // Size
    this->width = td::Tile::DEFAULT_TILE_SIZE;
    this->height = td::Tile::DEFAULT_TILE_SIZE;

    // Color
    this->color = sf::Color::White;
    this->texture = nullptr;

    // Render
    this->drawable = td::Shapes::rect(this->x, this->y,this->width, this->height);
}
td::RenderObject::~RenderObject() {
    delete this->texture;
}

// Set the map that the player will roam around
void td::RenderObject::setMap(td::Map &m) {
    this->map = m;
}

// Get the player's position
// Optionally set 'center' to true to get the position from the player's center
sf::Vector2f td::RenderObject::getPosition(bool center) const {
    if (center)
        return {this->x + ((float)this->width/2), this->y + ((float)this->height/2)};
    return {this->x, this->y};
}

// Set starting position, x and y
void td::RenderObject::setStartPosition(float start_x, float start_y) {
    this->x = start_x;
    this->y = start_y;
}

// Set starting position, row and col
void td::RenderObject::setStartTile(int row, int col) {
    this->x = (float)(col * this->map.getTileSize()) + ((float)(this->map.getTileSize()-this->width)/2);
    this->y = (float)(row * this->map.getTileSize()) + ((float)(this->map.getTileSize()-this->height)/2);
}

// Draw the player
void td::RenderObject::draw(sf::RenderTarget* target) {
    this->drawable.setPosition(sf::Vector2f(this->x, this->y));
    this->drawable.setSize(sf::Vector2f(this->width, this->height));
    target->draw(this->drawable);
}

// Set the player's color
void td::RenderObject::setColor(sf::Color c) {
    this->color = c;
    this->drawable.setFillColor(c);
}

// Set the player's sprite texture
void td::RenderObject::setTexture(const std::string& file) {
    auto* player_texture = new sf::Texture();
    if (!player_texture->loadFromFile(file)) {
        throw std::invalid_argument("Could not load texture at path " + file);
    }
    this->texture = player_texture;
    this->drawable.setTexture(player_texture);
}

// Get the player's width and height
td::Util::size td::RenderObject::getSize() const {
    return {this->width, this->height};
}

// Set the player's width and height
void td::RenderObject::setSize(int w, int h, bool center_in_tile) {
    this->width = w;
    this->height = h;
    if (center_in_tile) {
        td::Tile tile = this->map.getTile(this->x, this->y);
        sf::Vector2i pos = tile.getPosition(this->map.getTileSize());
        this->x = pos.x + ((float)(this->map.getTileSize()-this->width)/2);
        this->y = pos.y + ((float)(this->map.getTileSize()-this->height)/2);
    }
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
    this->inventory = std::vector<td::Item*>();
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

// Listen for player movement
// Upon correct key presses, update the player's position
// Passed a time delta to ensure consistent speeds across all fps values
void td::Player::move(float elapsed) {
    float new_x = this->x;
    float new_y = this->y;
    float move_amount = this->speed * elapsed;

    // Handle keyboard inputs:
    // Each handler section optimistically sets the new coordinate position.
    // If that position turns out to intersect with a wall, then instead move the remaining distance.
    // This has the effect of clamping the position to align with the tile size.
    if (sf::Keyboard::isKeyPressed(this->up_key)) {     // UP
        new_y = this->y - move_amount;
        sf::RectangleShape rect = td::Shapes::rect(new_x, new_y, this->width, this->height);
        if (td::Map::collides(this->map, this->map.getTileType(td::Map::TileTypes::WALL), rect))
            new_y = std::floor(this->y) - (float)((int)this->y % this->map.getTileSize());
    }
    if (sf::Keyboard::isKeyPressed(this->down_key)) {   // DOWN
        new_y = this->y + move_amount;
        sf::RectangleShape rect = td::Shapes::rect(new_x, new_y, this->width, this->height);
        if (td::Map::collides(this->map, this->map.getTileType(td::Map::TileTypes::WALL), rect))
            new_y = std::floor(this->y) + ((float)((int)(this->map.getTileSize() - ((int)(this->y + (float)this->height) % this->map.getTileSize())) % this->map.getTileSize()));
    }
    if (sf::Keyboard::isKeyPressed(this->left_key)) {   // LEFT
        new_x = this->x - move_amount;
        sf::RectangleShape rect = td::Shapes::rect(new_x, new_y, this->width, this->height);
        if (td::Map::collides(this->map, this->map.getTileType(td::Map::TileTypes::WALL), rect))
            new_x = std::floor(this->x) - (float)((int)this->x % this->map.getTileSize());
    }
    if (sf::Keyboard::isKeyPressed(this->right_key)) {  // RIGHT
        new_x = this->x + move_amount;
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
    this->speed = move_speed * ((float)this->map.getTileSize()/8);
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

// Check if the player is currently colliding with an end tile
bool td::Player::onEnd() {
    sf::RectangleShape p_rect = td::Shapes::rect(this->x, this->y, this->width, this->height);
    return td::Map::collides(this->map, this->map.getTileType(td::Map::TileTypes::END), p_rect);
}

// Check if the player is currently colliding with an enemy
bool td::Player::isTouchingEnemy() {
    return !this->getTouchingEnemies().empty();
}

// Get the enemies that the player is touching
std::vector<td::Enemy*> td::Player::getTouchingEnemies() {
    std::vector<td::Enemy*> touching_enemies = std::vector<td::Enemy*>();

    sf::RectangleShape p_rect = td::Shapes::rect(this->x, this->y, this->width, this->height);
    for (auto enemy : *this->map.getEnemies()) {
        sf::RectangleShape enemy_rect = td::Shapes::rect(
                enemy->getPosition().x, enemy->getPosition().y, enemy->getSize().width, enemy->getSize().height);
        if (p_rect.getGlobalBounds().intersects(enemy_rect.getGlobalBounds())) {
            touching_enemies.emplace_back(enemy);
        }
    }
    return touching_enemies;
}

// Check if the player is currently colliding with any un-obtained items
bool td::Player::isTouchingItem() {
    return !this->getTouchingItems().empty();
}

// Get the items that the player is touching
std::vector<td::Item*> td::Player::getTouchingItems() {
    std::vector<td::Item*> touching_items = std::vector<td::Item*>();

    sf::RectangleShape p_rect = td::Shapes::rect(this->x, this->y, this->width, this->height);
    for (auto& item : *this->map.getItems()) {
        if (!item->isObtained()) {
            sf::RectangleShape item_rect = td::Shapes::rect(
                    item->getPosition().x, item->getPosition().y, item->getSize().width, item->getSize().height);
            if (p_rect.getGlobalBounds().intersects(item_rect.getGlobalBounds())) {
                touching_items.emplace_back(item);
            }
        }
    }
    return touching_items;
}

// Add an item to the player's inventory.
// Then set that item's 'obtained' status to true
void td::Player::obtainItem(td::Item* item) {
    this->inventory.emplace_back(item);
    item->setObtained(true);
}

// Get a vector of the player's obtained items
std::vector<td::Item*> td::Player::getInventory() {
    return this->inventory;
}

// Clear the player's inventory.
// Take care to set each item's state back to un-obtained
void td::Player::clearInventory() {
    for (auto item: this->inventory) {
        item->setObtained(false);
    }
    this->inventory.clear();
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
    this->waypoints = {};
    this->move_option = td::Enemy::MoveOptions::LOOP;
    this->current_waypoint_index = 0;
}
td::Enemy::Enemy(const td::Map& map, int width, int height, sf::Color color, int harm) {
    this->map = map;
    this->width = width;
    this->height = height;
    this->color = color;
    this->harm = harm;
    this->waypoints = {};
    this->move_option = td::Enemy::MoveOptions::LOOP;
    this->current_waypoint_index = 0;
}
td::Enemy::~Enemy() = default;

// Reset the enemy's position to the starting waypoint
void td::Enemy::reset() {
    if (!this->waypoints.empty()) {
        this->x = this->waypoints[0].x;
        this->y = this->waypoints[0].y;
    }
    else {
        this->x = 0; this->y = 0;
    }
}

// Set the map that the enemy will be on
// Overwrites base Player setMap() so that the call to spawn() is omitted
void td::Enemy::setMap(td::Map& m) {
    this->map = m;
}

// Get the amount of damage that the enemy does
int td::Enemy::getHarm() const {
    return this->harm;
};

// Set the amount of damage that the enemy does
void td::Enemy::setHarm(int health_points) {
    this->harm = health_points;
}

// Set the waypoints the enemy should follow when moving
void td::Enemy::setWaypoints(const std::vector<sf::Vector2f>& enemy_waypoints, bool tiles) {
    std::vector<sf::Vector2f> position_waypoints = enemy_waypoints;
    // If tiles passed in, translate the rows and columns to x and y positions
    if (tiles) {
        for (auto& waypoint : position_waypoints) {
            // Rows stored in x, columns stored in y. Switch them and multiply by tile_size
            float translated_col = waypoint.y * (float)this->map.getTileSize()
                    + ((float)(this->map.getTileSize()-this->width)/2);
            float translated_row = waypoint.x * (float)this->map.getTileSize()
                    + ((float)(this->map.getTileSize()-this->height)/2);
            waypoint.x = translated_col;
            waypoint.y = translated_row;
        }
    }
    this->waypoints = position_waypoints;

    if (!this->waypoints.empty()) {
        this->x = this->waypoints[0].x;
        this->y = this->waypoints[0].y;
    }
}

void td::Enemy::setMoveOption(int enemy_move_option) {
    this->move_option = enemy_move_option;
}

// Update the enemy's first waypoint
void td::Enemy::setStartPosition(float start_x, float start_y) {
    if (!this->waypoints.empty()) {
        this->waypoints[0] = {start_x, start_y};
    }
}
void td::Enemy::setStartTile(int row, int col) {
    if (!this->waypoints.empty()) {
        this->waypoints[0] = {
                (float)(col * this->map.getTileSize()) + ((float)(this->map.getTileSize()-this->width)/2),
                (float)(row * this->map.getTileSize()) + ((float)(this->map.getTileSize()-this->height)/2)
        };
    }
}

// Move the enemy, following a set of waypoints
void td::Enemy::move(float elapsed) {
    sf::Vector2f target_waypoint = this->waypoints[(this->current_waypoint_index + 1) % this->waypoints.size()];

    float current_x = this->x;
    float current_y = this->y;

    // Move the enemy toward the target waypoint
    float target_x = target_waypoint.x;
    float target_y = target_waypoint.y;

    // Get line slope, y = mx + b
    float m;
    if (target_x == current_x) { m = 1; }
    else { m = ((target_y - current_y)*-1) / (target_x - current_x); }

    // Get how much x and y should be updated
    float dx;
    float dy;
    if (std::abs(m) >= 1) {
        dx = this->speed * elapsed * m;
        dy = this->speed * elapsed;
    }
    else {
        dx = this->speed * elapsed;
        dy = this->speed * elapsed * m;
    }

    // Get signs
    float sign_x;
    if (current_x > target_x) { sign_x = -1; }
    else if (current_x < target_x) { sign_x = 1; }
    else { sign_x = 0; }

    float sign_y;
    if (current_y > target_y) { sign_y = -1; }
    else if (current_y < target_y) { sign_y = 1; }
    else { sign_y = 0; }

    // Get the new x and y
    float new_x = (current_x + (sign_x * dx));
    float new_y = (current_y + (sign_y * dy));

    // Get distance to target. If distance is within reach, snap to it
    float distance_remaining = std::abs(td::Util::dist(current_x, current_y, target_x, target_y));
    float distance_traveled = std::abs(td::Util::dist(current_x, current_y, new_x, new_y));
    if (distance_traveled > distance_remaining) {
        new_x = target_x;
        new_y = target_y;
    }

    // If reached target waypoint, update waypoint index
    if (new_x == target_x && new_y == target_y) {
        this->current_waypoint_index = (this->current_waypoint_index + 1) % this->waypoints.size();
    }

    // Update the enemy's x and y
    this->x = new_x;
    this->y = new_y;
}
//------------------------------------------------------------------------------------------------------------------


/* Item */

// Constructor/destructor
td::Item::Item() : RenderObject() {
    this->obtained = false;
}
td::Item::Item(const td::Map& map, int width, int height, sf::Color color) : RenderObject() {
    this->map = map;
    this->obtained = false;
    this->width = width;
    this->height = height;
    this->color = color;
}
td::Item::~Item() = default;

// Reset the item's state to un-obtained
void td::Item::reset() {
    this->obtained = false;
}

// Render the item, but don't render it if it has been obtained already
void td::Item::draw(sf::RenderTarget* target) {
    if (!this->obtained) {
        this->drawable.setPosition(sf::Vector2f(this->x, this->y));
        this->drawable.setSize(sf::Vector2f(this->width, this->height));
        target->draw(this->drawable);
    }
}

// Has the item been obtained by the player?
bool td::Item::isObtained() const {
    return this->obtained;
}

// Set if the item should be consider obtained
void td::Item::setObtained(bool item_obtained) {
    this->obtained = item_obtained;
}
