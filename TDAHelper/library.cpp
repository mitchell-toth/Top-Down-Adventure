/**
 * @file library.cpp
 * @brief The TDAHelper implementation file.
 *
 * @authors Mitchell Toth, Jacob Haimes
 * @date May 2021
 */

#include "library.hpp"

/* Util */

/**
 * @brief Helper to locate a value within a std::vector object.
 * @tparam V
 * @param vector The std::vector to search through.
 * @param val The value to search for.
 * @return The integer index of the value, or -1 if not found.
 */
template<typename V>
int td::Util::find(std::vector<V> vector, V val) {
    auto it = std::find(vector.begin(), vector.end(), val);
    if (it != vector.end()) return it - vector.begin();
    return -1;
}

/**
 * @brief Helper to check if a key exists within a std::map object.
 * @tparam K
 * @tparam V
 * @param map The std::map to search through.
 * @param key The map key to search for.
 * @return True if the map contains the key, False otherwise.
 */
template<typename K, typename V>
bool td::Util::keyInMap(std::map<K, V> map, K key) {
    if (map.count(key) != 0) return true;
    return false;
}

/**
 * @brief Calculate the Euclidean distance between two points.
 * @param x1 x value of the first point
 * @param y1 y value of the first point
 * @param x2 x value of the second point
 * @param y2 y value of the second point
 * @return The floating point distance of a straight line passing between the points.
 */
float td::Util::dist(float x1, float y1, float x2, float y2) {
    return (float)std::sqrt(std::pow(x2-x1, 2) +std::pow(y2-y1, 2));
}
//------------------------------------------------------------------------------------------------------------------


/* Text */

/**
 * @brief Helper to print text to an SFML render target.
 * @param target The render target on which to display the text.
 * @param s The string to print.
 * @param config A configuration struct that defines where and how to draw the text.
 * @param relativeToView A boolean telling how to process the text's x and y position.
 * True = relative to the render target's view. False = absolute to the render target as a whole.
 */
void td::Text::print(sf::RenderTarget* target, const std::string &s, const td::Text::Config& config, bool relativeToView) {
    // Set up the text, size, and color
    sf::Text text = sf::Text();
    text.setFont(config.font);
    text.setString(s);
    text.setCharacterSize(config.size);
    text.setFillColor(config.color);

    auto x = (float)config.x;
    auto y = (float)config.y;
    if (relativeToView) {
        sf::Vector2f viewPos = target->mapPixelToCoords({(int)config.x, (int)config.y});
        x = viewPos.x;
        y = viewPos.y;
    }

    // Set horizontal alignment
    if (config.align == td::Text::Align::LEFT) {  // Align left
        text.setPosition(x, y);
    }
    else if (config.align == td::Text::Align::CENTER) {  // Center text horizontally. Ignore the x passed in
        text.setPosition(
                (float)((target->getView().getSize().x * 0.5) - (text.getLocalBounds().width * 0.5)), y);
    }
    else {  // Align right
        text.setPosition(x - text.getLocalBounds().width, y);
    }

    // Draw text to target window
    target->draw(text);
}
//------------------------------------------------------------------------------------------------------------------


/* Music */

/**
 * @brief Music class constructor. Default, no parameters.
 */
td::Music::Music() = default;
/**
 * @brief Music class constructor.
 * @param path The string path to a music file.
 * @param loop Boolean to determine if the music should loop or not.
 * @param volume How loud the music should be. Default value: 100.f.
 * @param pitch The pitch of the music. Default value: 1.f.
 */
td::Music::Music(const std::string &path, bool loop, float volume, float pitch) {
    if (!this->music.openFromFile(path))
        throw std::invalid_argument("Could not load audio at path " + path);
    this->music.setLoop(loop);
    this->music.setVolume(volume);
    this->music.setPitch(pitch);
}
/**
 * @brief Music class destructor.
 */
td::Music::~Music() = default;

/**
 * @brief Play the music.
 */
void td::Music::play() {
    this->music.play();
}

/**
 * @brief Stop the music.
 */
void td::Music::stop() {
    this->music.stop();
}
//------------------------------------------------------------------------------------------------------------------


/* Sound */

/**
 * @brief Sound class constructor. Default, no parameters.
 */
td::Sound::Sound() = default;
/**
 * Sound class constructor.
 * @param path The string path to a sound file.
 * @param volume How loud the sound should be. Default value: 100.f.
 * @param pitch The pitch of the sound. Default value: 1.f.
 */
td::Sound::Sound(const std::string &path, float volume, float pitch) {
    if (!this->buffer.loadFromFile(path))
        throw std::invalid_argument("Could not load audio at path " + path);
    this->sound.setBuffer(this->buffer);
    this->sound.setVolume(volume);
    this->sound.setPitch(pitch);
}
/**
 * @brief Sound class destructor.
 */
td::Sound::~Sound() = default;

/**
 * @brief Play the sound effect.
 */
void td::Sound::play() {
    this->sound.play();
}

/**
 * @brief Stop the sound effect.
 */
void td::Sound::stop() {
    this->sound.stop();
}
//------------------------------------------------------------------------------------------------------------------


/* Shapes */

/**
 * @brief Helper to create an SFML RectangleShape object.
 * @param x Starting x coordinate.
 * @param y Starting y coordinate.
 * @param width The rectangle's width.
 * @param height The rectangle's height.
 * @param color The rectangle's fill color.
 * @return A newly created RectangleShape object.
 */
sf::RectangleShape td::Shapes::rect(float x, float y, int width, int height, sf::Color color) {
    sf::RectangleShape rect;
    rect.setPosition(sf::Vector2f(x, y));
    rect.setSize(sf::Vector2f(width, height));
    rect.setFillColor(color);
    return rect;
}

/**
 * @brief Helper to create an SFML CircleShape object.
 * @param x Starting x coordinate.
 * @param y Starting y coordinate.
 * @param radius The circle's radius.
 * @param color The circle's fill color.
 * @return A newly created CircleShape object.
 */
sf::CircleShape td::Shapes::circ(float x, float y, float radius, sf::Color color) {
    sf::CircleShape circ;
    circ.setRadius(radius);
    circ.setPosition(x, y);
    circ.setFillColor(color);
    return circ;
}

/**
 * @brief Helper to create an SFML VertexArray line object.
 * @param x1 The x position of the first point.
 * @param y1 The y position of the first point.
 * @param x2 The x position of the second point.
 * @param y2 The y position of the second point.
 * @param color The line's color.
 * @return A newly created VertexArray containing two points.
 */
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

/**
 * @brief ClickableMenu class constructor. Default, no parameters.
 */
td::ClickableMenu::ClickableMenu() {
    this->initVariables();
}
/**
 * @brief ClickableMenu class constructor.
 * @param target The SFML RenderWindow to draw the menu on.
 * @param start_x The menu's x position.
 * @param start_y The menu's y position.
 * @param padding Padding around each menu button's text. Values given in pixels and formatted like CSS padding.
 * If one value is passed, the value will be applied in all 4 directions.
 * If two values are passed, the first will count as both top and bottom, and the second will count as right and left.
 * If four values passed, each value is applied clockwise, starting with the top.
 * @param menuItems A 2D vector containing the menu's string options.
 * @param textConfig An instance of td::Text::Config defining how the text for menu items should be rendered.
 */
td::ClickableMenu::ClickableMenu(sf::RenderWindow* target, float start_x, float start_y, const std::vector<int>& padding,
                                 std::vector<std::vector<std::string>> menuItems,
                                 const td::Text::Config& textConfig) {
    this->initVariables();  // Set to defaults
    // Now set specific items to the new values
    this->target = target;

    // Get start position relative to view
    this->setPosition(start_x, start_y);

    this->setPadding(padding);
    this->menuItems = std::move(menuItems);
    this->textConfig = textConfig;

    // Set up the menu item on-hover rectangles
    this->createOnHoverRectangles();
}
/**
 * @brief ClickableMenu class destructor.
 */
td::ClickableMenu::~ClickableMenu() = default;

/**
 * @brief Initialize the class attributes to default values.
 */
void td::ClickableMenu::initVariables() {
    this->x = 0;
    this->y = 0;
    this->padding = {0, 0, 0, 0};
    this->optionColors = std::vector<std::vector<sf::Color>>();
    this->buttonWidth = -1;
    this->buttonHeight = -1;
    this->menuItems = std::vector<std::vector<std::string>>();
    this->menuItemRects = std::vector<std::vector<sf::RectangleShape>>();
    this->onHoverColor = sf::Color(140, 140, 140, 100);
    this->outlineColor = sf::Color::Transparent;
    this->outlineThickness = 0;
    this->textConfig = {};
}

/**
 * @brief Construct the rectangles that will be overlaid atop each menu item.
 * These rectangles will listen for mouse hover events and will become highlighted when moused over.
 * Special care is taken if align is set to td::Text::Align::CENTER.
 */
void td::ClickableMenu::createOnHoverRectangles() {
    // Create a vector of colored rectangles to go behind the menu options text
    for (int r=0; r<this->menuItems.size(); r++) {
        this->menuItemRects.emplace_back(std::vector<sf::RectangleShape>());

        float rect_x = this->x;
        std::vector<sf::RectangleShape> rects = std::vector<sf::RectangleShape>();

        for (int c=0; c<this->menuItems[r].size(); c++) {
            float rect_y;
            int width, height;
            if (this->buttonWidth != -1 && this->buttonHeight != -1) {
                width = this->buttonWidth;
                height = this->buttonHeight;
            }
            else {
                sf::Text text = sf::Text(sf::String(this->menuItems[r][c]), this->textConfig.font, this->textConfig.size);
                width = (int)text.getGlobalBounds().width + this->padding[3] + this->padding[1];
                height = (int)text.getGlobalBounds().height + this->padding[0] + this->padding[2];
            }

            rect_y = this->y + (float)(height * r);
            sf::RectangleShape rect = td::Shapes::rect(rect_x, rect_y, width, height);

            if (this->textConfig.align == td::Text::Align::CENTER) {  // Center horizontally
                rects.emplace_back(rect);
            }
            else {
                this->menuItemRects[r].emplace_back(rect);
            }
            rect_x += (float)width;
        }

        // If the alignment is CENTER, iterate over the rectangles to find their total width
        // Then draw them at the appropriate starting x location using that information
        if (this->textConfig.align == td::Text::Align::CENTER) {
            int totalWidth = 0;
            // Get the total width
            for (const auto& rect : rects) {
                totalWidth += rect.getSize().x;
            }

            auto start_x = (float)((this->target->getView().getSize().x * 0.5) - (totalWidth * 0.5));

            // Place the rects in this->menuItemRects at the correct locations
            for (auto& rect : rects) {
                rect.setPosition(start_x, rect.getPosition().y);
                this->menuItemRects[r].emplace_back(rect);
                start_x += rect.getSize().x;
            }
        }
    }
}

/**
 * @brief Return the string options available in the menu.
 * @return A 2D vector containing all menu item strings.
 */
std::vector<std::vector<std::string>> td::ClickableMenu::getMenuItems() {
    return this->menuItems;
}

/**
 * @brief Return the menu's starting x and y coordinates.
 * @return The view-adjusted x and y position of the top-left corner of the menu.
 */
sf::Vector2f td::ClickableMenu::getPosition() const {
    return {this->x, this->y};
}

/**
 * @brief Specify the render window that the menu should use.
 * @param rw An SFML RenderWindow.
 */
void td::ClickableMenu::setRenderWindow(sf::RenderWindow* rw) {
    this->target = rw;
}

/**
 * @brief Set the top-left corner position of the menu.
 * @param start_x The menu's desired x position.
 * @param start_y The menu's desired y position.
 */
void td::ClickableMenu::setPosition(float start_x, float start_y) {
    sf::Vector2f viewPos = this->target->mapPixelToCoords({(int)start_x, (int)start_y});
    this->x = viewPos.x;
    this->y = viewPos.y;
}

/**
 * @brief Set the padding (in pixels) that goes around each menu option string.
 * @param p The padding around each menu button's text. Values given in pixels and formatted like CSS padding.
 * If one value is passed, the value will be applied in all 4 directions.
 * If two values are passed, the first will count as both top and bottom, and the second will count as right and left.
 * If four values passed, each value is applied clockwise, starting with the top.
 */
void td::ClickableMenu::setPadding(const std::vector<int>& p) {
    if (p.size() == 1) this->padding = {p[0], p[0], p[0], p[0]};
    else if (p.size() == 2) this->padding = {p[0], p[1], p[0], p[1]};
    else if (p.size() != 4) this->padding = {0, 0, 0, 0};
    else this->padding = p;
}

/**
 * @brief Specify the options that are to be in the menu.
 * @param items A 2D vector of string menu items.
 * Each string represents a button, and each vector of strings represents a row of buttons.
 */
void td::ClickableMenu::setMenuItems(const std::vector<std::vector<std::string>>& items) {
    this->menuItems = items;
    this->createOnHoverRectangles();
}

/**
 * @brief Set the text styling and configuration for each menu option.
 * @param config An instance of td::Text::Config.
 */
void td::ClickableMenu::setTextConfig(const td::Text::Config& config) {
    this->textConfig = config;
}

/**
 * @brief Set the absolute width and height for each menu item button.
 * This should only be specified if the default sizing and padding are not desirable.
 * @param width Button width.
 * @param height Button height.
 */
void td::ClickableMenu::setButtonSize(int width, int height) {
    this->buttonWidth = width;
    this->buttonHeight = height;
}

/**
 * @brief Specify a color for each menu option.
 * @param colors A parallel 2D vector containing a color for each menu option button.
 */
void td::ClickableMenu::setOptionColors(const std::vector<std::vector<sf::Color>>& colors) {
    this->optionColors = colors;
}

/**
 * @brief Specify the color that lights up on hover over a menu item.
 * @param color The on-hover highlight color. Default value: sf::Color(140, 140, 140, 100).
 */
void td::ClickableMenu::setOnHoverColor(sf::Color color) {
    this->onHoverColor = color;
}

/**
 * @brief Specify the color that outlines each menu item.
 * @param color Outline color. Default value: sf::Color::Transparent;
 * @param thickness Outline thickness. Default value: 0.
 */
void td::ClickableMenu::setOutline(sf::Color color, int thickness) {
    this->outlineColor = color;
    this->outlineThickness = thickness;
}

/**
 * @brief Render the menu by printing the text options and hover rectangles.
 */
void td::ClickableMenu::drawMenu() {
    for (int r=0; r<this->menuItems.size(); r++) {
        for (int c=0; c<this->menuItems[r].size(); c++) {
            // Get the hover rectangle
            sf::RectangleShape rect = this->menuItemRects[r][c];
            sf::Text text = sf::Text(sf::String(this->menuItems[r][c]), this->textConfig.font, this->textConfig.size);

            // Set the text's x and y based on the hover rectangle's position
            this->textConfig.x = (int)(rect.getPosition().x + (rect.getSize().x * 0.5) - (text.getGlobalBounds().width * 0.5));
            this->textConfig.y = (int)(rect.getPosition().y - (text.getGlobalBounds().height * 0.5) + (text.getGlobalBounds().height * 0.5));

            this->textConfig.align = td::Text::Align::LEFT;
            if (this->optionColors.size() >= r+1 && this->optionColors[r].size() >= c+1) {
                this->textConfig.color = this->optionColors[r][c];
            }

            // Draw the text
            td::Text::print(this->target, this->menuItems[r][c], this->textConfig, false);

            // Draw the hover rectangle. Will be invisible unless an outline is set
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(this->outlineColor);
            rect.setOutlineThickness(1);
            this->target->draw(rect);
        }
    }
}

/**
 * @brief The menu's mouse hover event listener.
 * Checks if the mouse is within any of the menu options rectangles. If so, draw the highlighted rectangle.
 * Must be called each frame.
 */
void td::ClickableMenu::onMouseOver() {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(*this->target);  // Get mouse x and y
    sf::Vector2f viewPos = this->target->mapPixelToCoords(pixelPos);  // Get mouse x and y relative to view

    // Iterate over each rectangle to check if it contains mouse_x and mouse_y
    for (int r=0; r<this->menuItems.size(); r++) {
        for (int c = 0; c < this->menuItems[r].size(); c++) {
            if (this->menuItemRects[r][c].getGlobalBounds().contains(viewPos.x, viewPos.y)) {
                this->menuItemRects[r][c].setFillColor(this->onHoverColor);
                this->target->draw(this->menuItemRects[r][c]);
            }
        }
    }
}

/**
 * @brief The menu's mouse click event listener.
 * Checks if a mouse click occurred in a menu item rectangle. If so, return the corresponding item string.
 * Must be called each frame.
 * @return The string corresponding to the clicked menu option. If no option was clicked this frame, return "".
 */
std::string td::ClickableMenu::onMouseClick() {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(*this->target);  // Get mouse x and y
    sf::Vector2f viewPos = this->target->mapPixelToCoords(pixelPos);  // Get mouse x and y relative to view

    // Iterate over each rectangle to check if it contains mouse_x and mouse_y
    for (int r = 0; r < this->menuItems.size(); r++) {
        for (int c = 0; c < this->menuItems[r].size(); c++) {
            if (this->menuItemRects[r][c].getGlobalBounds().contains(viewPos.x, viewPos.y)) {
                return this->menuItems[r][c];  // Return string option corresponding to selected item
            }
        }
    }
    return "";
}
//------------------------------------------------------------------------------------------------------------------


/* SpriteSheet */

/**
 * @brief SpriteSheet class constructor. Default, no parameters.
 */
td::SpriteSheet::SpriteSheet() = default;
/**
 * @brief SpriteSheet class destructor. Frees each sprite's texture pointer.
 */
td::SpriteSheet::~SpriteSheet() {
    for (auto const& key_val : this->mapping) {
        delete key_val.second.getTexture();
    }
}

/**
 * @brief Add a sprite to the sprite sheet. Creates a sprite with the given color and maps it to the given ID.
 * @param id A char ID that uniquely identifies this sprite from the others in the sheet.
 * @param color The sprite's color (if the sprite has no texture, see also td::SpriteSheet::addTexture).
 */
void td::SpriteSheet::addSprite(char id, sf::Color color) {
    sf::RectangleShape rect = sf::RectangleShape();
    rect.setFillColor(color);
    this->mapping[id] = rect;
}

/**
 * @brief Add a sprite to the sprite sheet. Creates a sprite with the given texture and maps it to the given ID.
 * @param id A char ID that uniquely identifies this sprite from the others in the sheet.
 * @param file The sprite's texture. This takes precedence over any color given previously.
 */
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

/**
 * @brief Tile class constructor. Default, no parameters.
 */
td::Tile::Tile() = default;
/**
 * @brief Tile class constructor.
 * @param sprite_id The char ID for the tile's sprite appearance.
 * @param type_id The char ID for the tile's functionality and behavior.
 * @param row The tile's row.
 * @param col The tile's column.
 */
td::Tile::Tile(char sprite_id, char type_id, int row, int col) {
    this->sprite_id = sprite_id;
    this->type_id = type_id;
    this->row = row;
    this->col = col;
}
/**
 * @brief Tile class destructor.
 */
td::Tile::~Tile() = default;

/**
 * @brief Get the tile's rectangle object.
 * @param tile_size The tile size to use when calculating the bounds of the tile's rectangle object.
 * @return A RectangleShape that represents the bounds of the tile.
 */
sf::RectangleShape td::Tile::getRect(int tile_size) const {
    return td::Shapes::rect((float)(tile_size * this->col), (float)(tile_size * this->row), tile_size, tile_size);
}

/**
 * @brief Get the tile's drawable sprite object.
 * @param sprite_sheet The sprite sheet to use when interpreting the tile's sprite ID.
 * @param tile_size The tile size to use when getting the tile's bounding rectangle.
 * @return The RectangleShape that underlies the tile's sprite.
 */
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

/**
 * @brief Get the x and y locations of the tile. Translates the tile's row and column to absolute x and y positioning.
 * @param tile_size The tile size to use when calculating the x and y coordinates.
 * @return A Vector2i of the tile's x and y position.
 */
sf::Vector2i td::Tile::getPosition(int tile_size) const {
    return {this->col * tile_size, this->row * tile_size};
}
//------------------------------------------------------------------------------------------------------------------


/* Map */

/**
 * @brief Map class constructor. Default, no parameters.
 */
td::Map::Map() {
    this->initVariables();
}
/**
 * @brief Map class constructor.
 * @param path The string path to a map file txt.
 */
td::Map::Map(const std::string& path) {
    this->initVariables();
    this->readMap(path);
}
/**
 * @brief Map class destructor.
 */
td::Map::~Map() = default;

/**
 * @brief Initialize map attributes to defaults.
 * Defines a default mapping of certain chars to tile types.
 */
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

/**
 * @brief Read in a file path for the game map txt and translate it to a 2D vector of Tile objects.
 * Effectively creates a tile grid out of the txt file.
 * Reads in the map two characters at a time, where the first char is interpreted as a sprite_id and the second
 * is interpreted as a type_id. The sprite_id governs appearance, and the type_id governs functionality.
 * @param path The string path to a map file txt.
 */
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
                if (start_tile_set)
                    throw std::invalid_argument("Multiple starting positions given. Only one allowed.");
                this->player_start_row = r;
                this->player_start_col = (int)c/2;
                start_tile_set = true;
            }
            if (td::Util::find(this->getTileType(td::Map::TileTypes::CHECKPOINT), type_id) != -1) {
                this->checkpointList.emplace_back(r,c/2);
            }

            this->map[r].emplace_back(tile);
        }
        r++;
    }
    mapFile.close();
}

/**
 * @brief Display the map in the game window.
 * @param target An SFML RenderTarget on which to draw the map.
 */
void td::Map::draw(sf::RenderTarget* target) {
    for (const auto& row : this->map) {
        for (const auto& tile : row) {
            // Create the corresponding graphical tile and display it
            target->draw(tile.getSprite(this->sprite_sheet, this->tile_size));
        }
    }
}

/**
 * @brief Display the map's enemies.
 * @param target An SFML RenderTarget.
 */
void td::Map::drawEnemies(sf::RenderTarget *target) {
    for (auto& enemy: this->enemies) {
        enemy->draw(target);
    }
}

/**
 * @brief Display the map's items.
 * @param target An SFML RenderTarget.
 */
void td::Map::drawItems(sf::RenderTarget *target) {
    for (auto item: this->items) {
        item->draw(target);
    }
}

/**
 * @brief Get the tile size being used by the map.
 * @return Int tile size.
 */
int td::Map::getTileSize() const {
    return this->tile_size;
}

/**
 * @brief Retrieve the tile at a given x and y location.
 * @param x The tile's x position.
 * @param y The tile's y position.
 * @return The tile at the x any y location.
 */
td::Tile td::Map::getTile(float x, float y) {
    int r = (int)(y/(float)this->tile_size);
    int c = (int)(x/(float)this->tile_size);
    return this->map[r][c];
}

/**
 * @brief Return a copy of the map.
 * @return A copy of the working Map object.
 */
std::vector<std::vector<td::Tile>> td::Map::getMap() {
    return this->map;
}

/**
 * @brief Get the char tile type_ids from an integer tile type.
 * @param type Integer tile type, likely specified from an enumeration (the td::Map::TileTypes enum).
 * @return A vector of the corresponding tile type chars.
 */
std::vector<char> td::Map::getTileType(int type) {
    return this->tile_types[type];
}

/**
 * @brief Get player starting row and column.
 * @return The player's starting tile, as specified in the loaded in map file.
 */
td::Tile td::Map::getPlayerStartTile() {
    return this->map[this->player_start_row][this->player_start_col];
}

/**
 * @brief Get map size. In pixels by default.
 * @param rows_cols Boolean to overwrite the default and instead return the map's number of rows and columns.
 * Default value: false.
 * @return The map's size in pixels, or in number of rows can columns depending on the value of rows_cols.
 */
sf::Vector2i td::Map::getMapSize(bool rows_cols) {
    int num_rows = this->map.size();
    int num_cols = this->map[0].size();
    if (rows_cols)
        return {num_cols, num_rows};
    else
        return {num_cols * this->tile_size, num_rows * this->tile_size};
}

/**
 * @brief Get the map's enemies.
 * @return A vector of pointers to the maps' Enemy objects.
 */
std::vector<td::Enemy*>* td::Map::getEnemies() {
    return &this->enemies;
}

/**
 * @brief Get the map's items.
 * @return A vector of pointers to the maps' Item objects.
 */
std::vector<td::Item*>* td::Map::getItems() {
    return &this->items;
}

/**
 * @brief Set the map's sprite sheet, which is used to determine what to draw at each tile.
 * @param sheet The sprite sheet to use.
 */
void td::Map::setSpriteSheet(const td::SpriteSheet& sheet) {
    this->sprite_sheet = sheet;
}

/**
 * @brief Set the size of the map tiles, in pixels.
 * @param size The desired tile size.
 */
void td::Map::setTileSize(int size) {
    if (size < 0 || size > this->max_allowed_tile_size) {
        throw std::invalid_argument("Invalid tile size.");
    }
    this->tile_size = size;
}

/**
 * @brief Set a special tile type, whether by overwriting a default one or creating a new one.
 * @param type Integer tile type, likely specified from an enumeration (the td::Map::TileTypes enum).
 * @param type_ids A vector of chars that correspond to the integer type.
 */
void td::Map::setTileType(int type, std::vector<char> type_ids) {
    this->tile_types[type] = std::move(type_ids);
}

/**
 * @brief Add an enemy to the map.
 * @param enemy A pointer to the Enemy object to add.
 */
void td::Map::addEnemy(td::Enemy* enemy) {
    this->enemies.emplace_back(enemy);
}

/**
 * @brief Move all enemies according to their waypoints.
 * @param elapsed The time elapsed between this frame and the last.
 */
void td::Map::moveEnemies(float elapsed) {
    for (auto enemy : this->enemies) {
        enemy->move(elapsed);
    }
}

/**
 * @brief Reset all enemies to their starting locations.
 */
void td::Map::resetEnemies() {
    for (auto enemy : this->enemies) {
        enemy->reset();
    }
}

/**
 * @brief Add an item to the map.
 * @param item A pointer to the Item object to add.
 */
void td::Map::addItem(td::Item* item) {
    this->items.emplace_back(item);
}

/**
 * @brief Reset all items to be un-obtained.
 */
void td::Map::resetItems() {
    for (auto item : this->items) {
        item->reset();
    }
}

/**
 * @brief Checks if the player is colliding with any tiles of a certain type. Uses td::Map::getCollisions.
 * @param map The map on which to test for collision.
 * @param type_ids The type IDs of tiles to watch out for when checking for player collision.
 * @param rect The player's bounding rectangle.
 * @return Boolean of whether or not a collision was detected. True = collision, False = no collision.
 */
bool td::Map::collides(td::Map& map, const std::vector<char>& type_ids, const sf::RectangleShape& rect) {
    return !td::Map::getCollisions(map, type_ids, rect).empty();
}

/**
 * @brief Get all tiles of certain types that the player is colliding with.
 * Look at all tiles around a given location (x,y) and check if the current location
 * intersects with any surrounding tiles of a specific type.
 * Looks in an n x n tile grid around the location given, where n depends on the size of the player.
 * @param map The map on which to test for collision.
 * @param type_ids The type IDs of tiles to watch out for when checking for player collision.
 * @param rect The player's bounding rectangle.
 * @return A vector of tiles (of the correct type) that were found to be colliding with the player.
 */
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

/**
 * @brief RenderObject class constructor.
 */
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
    this->CPTexture = nullptr;

    // Render
    this->drawable = td::Shapes::rect(this->x, this->y,this->width, this->height);
    this->CPdrawable = td::Shapes::rect(this->x, this->y,this->width, this->height);
}
/**
 * @brief RenderObject class destructor.
 */
td::RenderObject::~RenderObject() {
    delete this->texture;
}

/**
 * @brief Set the map that the player will roam around.
 * @param m A Map instance.
 */
void td::RenderObject::setMap(td::Map &m) {
    this->map = m;
}

/**
 * @brief Get the objects's position. Optionally set 'center' to true to get the position from the objects's center.
 * @param center Boolean to set whether the objects's position should be calculated from the objects's center.
 * True = from center, False = from top-left corner.
 * Default value: false.
 * @return A Vector2f of the objects's x and y coordinates.
 */
sf::Vector2f td::RenderObject::getPosition(bool center) const {
    if (center)
        return {this->x + ((float)this->width/2), this->y + ((float)this->height/2)};
    return {this->x, this->y};
}

/**
 * @brief Set the objects's starting position, x and y.
 * @param start_x Starting x position.
 * @param start_y Starting y position.
 */
void td::RenderObject::setStartPosition(float start_x, float start_y) {
    this->x = start_x;
    this->y = start_y;
}

/**
 * @brief Set starting position by row and column. The row and col values are translated to absolute x and y.
 * @param row Starting row.
 * @param col Starting column.
 */
void td::RenderObject::setStartTile(int row, int col) {
    this->x = (float)(col * this->map.getTileSize()) + ((float)(this->map.getTileSize()-this->width)/2);
    this->y = (float)(row * this->map.getTileSize()) + ((float)(this->map.getTileSize()-this->height)/2);
}

/**
 * @brief Draw the object.
 * @param target An SFML RenderTarget on which to draw the object.
 */
void td::RenderObject::draw(sf::RenderTarget* target) {
    this->drawable.setPosition(sf::Vector2f(this->x, this->y));
    this->drawable.setSize(sf::Vector2f(this->width, this->height));
    target->draw(this->drawable);
}

/**
 * @brief Draw the checkpoint mark.
 * @param target An SFML RenderTarget on which to draw the checkpoint mark.
 * @param cp_x Checkpoint x position.
 * @param cp_y Checkpoint y position.
 */
void td::RenderObject::drawCP(sf::RenderTarget* target, int cp_x, int cp_y) {
    this->CPdrawable.setPosition(sf::Vector2f((float)cp_x * (float)this->map.getTileSize(), (float)cp_y * (float)this->map.getTileSize()));
    this->CPdrawable.setSize(sf::Vector2f(this->map.getTileSize(), this->map.getTileSize()));
    target->draw(this->CPdrawable);
}

/**
 * @brief Draw the file image.
 * @param target An SFML RenderTarget on which to draw the file image.
 * @param img_x The image's x position.
 * @param img_y The image's y position.
 * @param file The string path to an image file.
 */
void td::RenderObject::drawFileImage(sf::RenderTarget* target, int img_x, int img_y, const std::string& file) {
    sf::RectangleShape fileImage;
    fileImage.setSize(sf::Vector2f(this->map.getTileSize(), this->map.getTileSize()));
    fileImage.setPosition(sf::Vector2f(img_y * this->map.getTileSize(), img_x * this->map.getTileSize()));
    auto* image = new sf::Texture();
    if (!image->loadFromFile(file)) {
        throw std::invalid_argument("Could not load texture at path " + file);
    }
    fileImage.setTexture(image);
    target->draw(fileImage);
}

/**
 * @brief Set the object's color.
 * @param c The desired color.
 */
void td::RenderObject::setColor(sf::Color c) {
    this->color = c;
    this->drawable.setFillColor(c);
}

/**
 * @brief Set the objects's texture. Will take precedence over any object color specified previously.
 * @param file The string path to a texture file.
 */
void td::RenderObject::setTexture(const std::string& file) {
    auto* player_texture = new sf::Texture();
    if (!player_texture->loadFromFile(file)) {
        throw std::invalid_argument("Could not load texture at path " + file);
    }
    this->texture = player_texture;
    this->drawable.setTexture(player_texture);
}


/**
 * @brief Set the checkpoints texture. Will take precedence over any object color specified previously.
 * @param file The string path to a texture file.
 */
void td::RenderObject::setCPTexture(const std::string& file) {
    auto* CP_texture = new sf::Texture();
    if (!CP_texture->loadFromFile(file)) {
        throw std::invalid_argument("Could not load texture at path " + file);
    }
    this->CPTexture = CP_texture;
    this->CPdrawable.setTexture(CP_texture);
}

/**
 * @brief Get the object's width and height.
 * @return A td::Util::size instance of the object's size.
 */
td::Util::size td::RenderObject::getSize() const {
    return {this->width, this->height};
}

/**
 * @brief Set the object's width and height.
 * @param w The object's desired width.
 * @param h The object's desired height.
 * @param center_in_tile Boolean to determine whether to re-draw the object in the middle of the object's current tile.
 * True = re-center the object, False = leave as is.
 * Default value: false.
 */
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

/**
 * @brief Player class constructor.
 */
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
/**
 * @brief Player class destructor.
 */
td::Player::~Player() = default;

/**
 * @brief Set the map that the player will roam around.
 * Overrides the RenderObject setMap() to add extra functionality such as setup and spawning.
 * @param m A td::Map instance.
 */
void td::Player::setMap(td::Map &m) {
    this->map = m;
    this->checkpoint = m.getPlayerStartTile();
    this->spawn();
}

/**
 * @brief Listen for player movement. Upon correct key presses, update the player's position.
 * @param elapsed The time delta from the last frame to this one.
 * Ensures consistent movement speeds across all fps values.
 */
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

/**
 * @brief Alter the default player movement keys.
 * @param up Desired Up key.
 * @param left Desired Left key.
 * @param down Desired Down key.
 * @param right Desired Right key.
 */
void td::Player::setMovementKeys(sf::Keyboard::Key up, sf::Keyboard::Key left, sf::Keyboard::Key down,
                                 sf::Keyboard::Key right) {
    this->up_key = up;
    this->left_key = left;
    this->down_key = down;
    this->right_key = right;
}

/**
 * @brief Set the player's speed.
 * @param move_speed Float speed.
 */
void td::Player::setMoveSpeed(float move_speed) {
    this->speed = move_speed * ((float)this->map.getTileSize()/8);
}

/**
 * @brief Set the player's position according to the starting point specified on the map.
 * This can be called at the start of a level to position the player at the map's start.
 */
void td::Player::spawn() {
    this->health = this->max_health;
    td::Tile tile = this->map.getPlayerStartTile();
    sf::Vector2i pos = tile.getPosition(this->map.getTileSize());
    this->x = pos.x + ((float)(this->map.getTileSize()-this->width)/2);
    this->y = pos.y + ((float)(this->map.getTileSize()-this->height)/2);
}

/**
 * @brief Set the player's position according to the latest checkpoint the player reached.
 * Like spawn, but looks at checkpoints and does not necessarily position the player at the map's start.
 */
void td::Player::respawn() {
    this->health = this->max_health;
    sf::Vector2i pos = this->checkpoint.getPosition(this->map.getTileSize());
    this->x = pos.x + ((float)(this->map.getTileSize()-this->width)/2);
    this->y = pos.y + ((float)(this->map.getTileSize()-this->height)/2);
}

/**
 * @brief Check if the player is currently colliding with a checkpoint tile.
 * @return Boolean. True = player is currently on a checkpoint tile, False = player is not on a checkpoint tile.
 */
bool td::Player::onCheckpoint() {
    sf::RectangleShape p_rect = td::Shapes::rect(this->x, this->y, this->width, this->height);
    return td::Map::collides(this->map, this->map.getTileType(td::Map::TileTypes::CHECKPOINT), p_rect);
}

/**
 * @brief Check if the player is currently colliding with a checkpoint tile.
 * Looks around for a checkpoint tile that the player is touching.
 * If it finds one, it sets the player's checkpoint variable to that tile.
 * If not, it sets the player's checkpoint tile at the current tile.
 * It thus supports both formal checkpoints and informal save points.
 */
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


/**
 * @brief Returns the current checkpoint tile.
 */
td::Tile td::Player::getCheckpoint() {
    return this->checkpoint;
}

/**
 * @brief Check if the player is currently colliding with an end tile.
 * @return Boolean. True = player is currently on an end tile, False = player is not on an end tile.
 */
bool td::Player::onEnd() {
    sf::RectangleShape p_rect = td::Shapes::rect(this->x, this->y, this->width, this->height);
    return td::Map::collides(this->map, this->map.getTileType(td::Map::TileTypes::END), p_rect);
}

/**
 * @brief Check if the player is currently colliding with an enemy. Uses td::Player::getTouchingEnemies.
 * @return Boolean. True = player is currently colliding with an enemy, False = player is not touching an enemy.
 */
bool td::Player::isTouchingEnemy() {
    return !this->getTouchingEnemies().empty();
}

/**
 * @brief Get the enemies that the player is touching.
 * Iterates over each enemy and checks if its bounding box is overlapping that of the player.
 * @return A vector of pointers to all enemies the player is currently colliding with.
 */
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

/**
 * @brief Additional implementation of td::Player::isTouchingEnemy(), just now with circles.
 * @return Boolean. True = player is currently colliding with an enemy, False = player is not touching an enemy.
 */
bool td::Player::isTouchingCircleEnemy() {
    return !this->getTouchingCircleEnemies().empty();
}

/**
 * @brief Additional implementation of td::Player::getTouchingEnemies(), just now with circles.
 * Treats each enemy as a circle object instead of a rectangle.
 * This is especially useful when the texture on an enemy makes it look circular.
 * @return A vector of pointers to all enemies the player is currently colliding with.
 */
std::vector<td::Enemy*> td::Player::getTouchingCircleEnemies() {
    std::vector<td::Enemy*> touching_enemies = std::vector<td::Enemy*>();

    sf::RectangleShape p_rect = td::Shapes::rect(this->x, this->y, this->width, this->height);
    for (auto enemy : *this->map.getEnemies()) {
        // Create a temporary circle object for the enemy. The radius is enemy width minus 1 to give some grace.
        sf::CircleShape enemy_circ = td::Shapes::circ(
                enemy->getPosition().x, enemy->getPosition().y, (float)enemy->getSize().width/2 - 1);
        if (p_rect.getGlobalBounds().intersects(enemy_circ.getGlobalBounds())) {
            touching_enemies.emplace_back(enemy);
        }
    }
    return touching_enemies;
}

/**
 * @brief Check if the player is currently colliding with any un-obtained items. Uses td::Player::getTouchingItems.
 * @return Boolean. True = player is currently colliding with an un-obtained object, False = player is not.
 */
bool td::Player::isTouchingItem() {
    return !this->getTouchingItems().empty();
}

/**
 * @brief Get the items that the player is touching.
 * Iterates over each item and checks if the item's bounds overlap with that of the player.
 * @return A vector of pointers to all items the player is currently colliding with.
 */
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

/**
 * @brief Add an item to the player's inventory. Then set that item's 'obtained' status to true.
 * @param item A pointer to the item that was just obtained by the player.
 */
void td::Player::obtainItem(td::Item* item) {
    this->inventory.emplace_back(item);
    item->setObtained(true);
}

/**
 * @brief Get the player's obtained items.
 * @return A vector of pointers to all items the player has obtained.
 */
std::vector<td::Item*> td::Player::getInventory() {
    return this->inventory;
}

/**
 * @brief Clear any un-committed items from the player's inventory.
 * Take care to set each item's state back to un-obtained and un-committed.
 */
void td::Player::clearInventory() {
    int size = this->inventory.size();
    for (int i=size-1; i>=0; i--) {
        td::Item* item = this->inventory[i];
        if (!item->isCommitted()) {
            item->setObtained(false);
            this->inventory.erase(this->inventory.begin() + i);
        }
    }
}

/**
 * @brief Completely clear the player's inventory.
 * Take care to set each item's state back to un-obtained and un-committed.
 */
void td::Player::resetInventory() {
    for (auto item : this->inventory) {
        item->setObtained(false);
        item->setCommitted(false);
    }
    this->inventory.clear();
}

/**
 * @brief Get the player's health.
 * @return The player's integer health points.
 */
int td::Player::getHealth() const {
    return this->health;
}

/**
 * @brief Set the player's health.
 * @param h Health points.
 */
void td::Player::setHealth(int h) {
    this->health = h;
}

/**
 * @brief Set the player's maximum health.
 * @param mh Maximum health points.
 */
void td::Player::setMaxHealth(int mh) {
    this->max_health = mh;
}

/**
 * @brief Decrement the player's health by a given amount.
 * @param health_points Health points to decrement by.
 */
void td::Player::loseHealth(int health_points) {
    this->health -= health_points;
}

/**
 * @brief Increment the player's health by a given amount.
 * @param health_points Health points to increment by.
 */
void td::Player::gainHealth(int health_points) {
    this->health += health_points;
}

/**
 * @brief Check if the player's health has dropped to or below 0.
 * @return Boolean. True = health is at or below 0, False = the player still has some life.
 */
bool td::Player::isDead() const {
    return this->health <= 0;
}
//------------------------------------------------------------------------------------------------------------------


/* Enemy */

/**
 * @brief Enemy class constructor. No parameters.
 */
td::Enemy::Enemy() {
    this->harm = 1;
    this->waypoints = {};
    this->move_option = td::Enemy::MoveOptions::LOOP;
    this->current_waypoint_index = 0;
    this->direction = 1;
}
/**
 * @brief Enemy class constructor.
 * @param map The td::Map instance that the enemy will move on and interact with.
 * @param width Enemy width.
 * @param height Enemy height.
 * @param color Enemy fill color.
 * @param harm The amount of harm the enemy deals when colliding with a Player instance. Default value: 1.
 */
td::Enemy::Enemy(const td::Map& map, int width, int height, sf::Color color, int harm) {
    this->map = map;
    this->width = width;
    this->height = height;
    this->color = color;
    this->harm = harm;
    this->waypoints = {};
    this->move_option = td::Enemy::MoveOptions::LOOP;
    this->current_waypoint_index = 0;
    this->direction = 1;
}
/**
 * @brief Enemy class destructor.
 */
td::Enemy::~Enemy() = default;

/**
 * @brief Reset the enemy's position to the starting waypoint.
 */
void td::Enemy::reset() {
    if (!this->waypoints.empty()) {
        this->x = this->waypoints[0].x;
        this->y = this->waypoints[0].y;
    }
    else {
        this->x = 0; this->y = 0;
    }
}

/**
 * @brief Set the map that the enemy will be on.
 * Overwrites base Player setMap() so that the checkpoint setup and call spawn() are omitted.
 * @param m The td::Map instance that the enemy will move on and interact with.
 */
void td::Enemy::setMap(td::Map& m) {
    this->map = m;
}

/**
 * @brief Get the amount of damage that the enemy deals.
 * @return Integer damage amount.
 */
int td::Enemy::getHarm() const {
    return this->harm;
};

/**
 * @brief Set the amount of damage that the enemy deals.
 * @param health_points Number of health points of damage to deal.
 */
void td::Enemy::setHarm(int health_points) {
    this->harm = health_points;
}

/**
 * @brief Set the waypoints the enemy should follow when moving.
 * @param enemy_waypoints A vector of Vector2f waypoints containing x and y values.
 * @param tiles Boolean to determine whether to interpret each x and y waypoints as tile rows and columns or not.
 * True = interpret as rows and columns, False = interpret as absolute x and y.
 * Default value: true.
 */
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

/**
 * @brief Set the enemy's waypoint movement option, whether LOOP or BACK_AND_FORTH.
 * LOOP allows the waypoints to wrap back around, where the first waypoint becomes the last waypoint's target.
 * BACK_AND_FORTH causes the enemy's direction to reverse when the last (or first) waypoint is reached.
 * @param enemy_move_option Integer option, likely specified via the td::Enemy::MoveOptions enum.
 */
void td::Enemy::setMoveOption(int enemy_move_option) {
    this->move_option = enemy_move_option;
}

/**
 * @brief Manually update the enemy's first waypoint.
 * @param start_x Starting x position.
 * @param start_y Starting y position.
 */
void td::Enemy::setStartPosition(float start_x, float start_y) {
    if (!this->waypoints.empty()) {
        this->waypoints[0] = {start_x, start_y};
    }
}

/**
 * @brief Manually update the enemy's first waypoint, using rows and columns.
 * @param row Starting row.
 * @param col Starting column.
 */
void td::Enemy::setStartTile(int row, int col) {
    if (!this->waypoints.empty()) {
        this->waypoints[0] = {
                (float)(col * this->map.getTileSize()) + ((float)(this->map.getTileSize()-this->width)/2),
                (float)(row * this->map.getTileSize()) + ((float)(this->map.getTileSize()-this->height)/2)
        };
    }
}

/**
 * @brief Move the enemy, following a set of waypoints.
 * @param elapsed The time delta from the last frame to this one.
 * Ensures consistent movement speeds across all fps values.
 */
void td::Enemy::move(float elapsed) {
    // If the move option is back and forth, reverse the direction upon reaching the last waypoint
    if (this->move_option == td::Enemy::MoveOptions::BACK_AND_FORTH) {
        if ((this->current_waypoint_index + this->direction) < 0 ||
            (this->current_waypoint_index + this->direction) >= this->waypoints.size()) {
            this->direction *= -1;  // Switch the direction
        }
    }
    // Otherwise, if the move option is to loop, the waypoints will wrap around
    sf::Vector2f target_waypoint = this->waypoints[(this->current_waypoint_index + this->direction) % this->waypoints.size()];

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
        // If the move option is back and forth, reverse the direction upon reaching the last waypoint
        if (this->move_option == td::Enemy::MoveOptions::BACK_AND_FORTH) {
            if ((this->current_waypoint_index + this->direction) < 0 ||
                (this->current_waypoint_index + this->direction) >= this->waypoints.size()) {
                this->direction *= -1;  // Switch the direction
            }
        }
        // Otherwise, if the move option is to loop, the waypoints will wrap around
        this->current_waypoint_index = (this->current_waypoint_index + this->direction) % this->waypoints.size();
    }

    // Update the enemy's x and y
    this->x = new_x;
    this->y = new_y;
}
//------------------------------------------------------------------------------------------------------------------


/* Item */

/**
 * @brief Item class constructor. No parameters.
 */
td::Item::Item() : RenderObject() {
    this->obtained = false;
    this->committed = false;
}
/**
 * @brief Item class constructor.
 * @param map The td::Map instance that the item is to be placed on.
 * @param width Item width.
 * @param height Item height.
 * @param color Item fill color.
 */
td::Item::Item(const td::Map& map, int width, int height, sf::Color color) : RenderObject() {
    this->map = map;
    this->obtained = false;
    this->committed = false;
    this->width = width;
    this->height = height;
    this->color = color;
}
/**
 * @brief Item class destructor.
 */
td::Item::~Item() = default;

/**
 * @brief Reset the item's state to un-obtained and un-committed.
 */
void td::Item::reset() {
    this->obtained = false;
    this->committed = false;
}

/**
 * @brief Render the item, but don't render it if it has been obtained already.
 * @param target An SFML RenderTarget.
 */
void td::Item::draw(sf::RenderTarget* target) {
    if (!this->obtained) {
        this->drawable.setPosition(sf::Vector2f(this->x, this->y));
        this->drawable.setSize(sf::Vector2f(this->width, this->height));
        target->draw(this->drawable);
    }
}

/**
 * @brief Check if the item been obtained by the player.
 * @return Boolean. True = obtained by player, False = has not been obtained.
 */
bool td::Item::isObtained() const {
    return this->obtained;
}

/**
 * @brief Set whether the item should be consider obtained.
 * @param item_obtained Boolean obtained status.
 */
void td::Item::setObtained(bool item_obtained) {
    this->obtained = item_obtained;
}

/**
 * @brief Check if the item been committed (secured) by the player.
 * Committed items are items that won't be lost if the player dies, for instance.
 * @return Boolean. True = committed by player, False = has not been committed.
 */
bool td::Item::isCommitted() const {
    return this->committed;
}

/**
 * @brief Set whether the item should be consider committed (secured).
 * @param item_committed Boolean committed status.
 */
void td::Item::setCommitted(bool item_committed) {
    this->committed = item_committed;
}
