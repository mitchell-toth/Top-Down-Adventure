#include "library.hpp"

/* General */




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


/* Tile */

td::Tile::Tile() = default;

td::Tile::Tile(char sprite, char type, int row, int col) {
    this->sprite = sprite;
    this->type = type;
    this->row = row;
    this->col = col;
}

td::Tile::~Tile() = default;



/* Map */

// Constructor
td::Map::Map() {
    this->initVariables();
}
td::Map::Map(const std::string& path) {
    this->initVariables();
    this->readMap(path);
}

// Destructor
td::Map::~Map() = default;

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
    // Also create a tile map with encoded information
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
            td::Tile t = row[c];
            sf::RectangleShape tile = td::Shapes::rect(this->tile_size*c, this->tile_size*r,
                                                       this->tile_size, this->tile_size);
            switch (t.sprite) {
                case '0':
                    // Empty tile, skip
                    break;
                case '1':
                    tile.setFillColor(sf::Color::Green);
                    target->draw(tile);
                    break;
                case '2':
                    tile.setFillColor(sf::Color::Red);
                    target->draw(tile);
                    break;
                default:
                    tile.setFillColor(sf::Color::Blue);
                    target->draw(tile);
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
