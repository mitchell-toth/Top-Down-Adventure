#include "Maps.h"

/*
 * @brief Hello world
 * @param tile_size The tile size
 */
std::vector<td::Map> Maps::initMaps(int tile_size) {
    /* Configuration: */
    // Create a sprite sheet and ID mapping to use with the maps
    td::SpriteSheet sprite_sheet = td::SpriteSheet();
    sprite_sheet.addSprite('#', sf::Color::Transparent);
    sprite_sheet.addSprite('w', sf::Color::Black);
    sprite_sheet.addSprite('a', sf::Color::Yellow);
    sprite_sheet.addSprite('`', sf::Color::White);
    sprite_sheet.addSprite('\'',sf::Color(200, 200, 200));
    sprite_sheet.addSprite('s', sf::Color(139, 246, 153));
    sprite_sheet.addSprite('c', sf::Color(139, 246, 153));
    sprite_sheet.addSprite('e', sf::Color(139, 246, 153));

    // Enemy config
    td::Enemy* enemy;
    Maps::enemy_width = (int)(tile_size*0.55);
    Maps::enemy_height = (int)(tile_size*0.55);
    Maps::enemy_harm = 100;

    // Coin config
    td::Item* coin;
    Maps::coin_width = (int)(tile_size*0.4);
    Maps::coin_height = (int)(tile_size*0.4);

    // Initialize the maps object to return
    std::vector<td::Map> maps = std::vector<td::Map>();
    //------------------------------------------------------------------------------------------------------------------

    /* Map 1: */
    td::Map map1 = td::Map("../assets/maps/map.txt");
    map1.setTileType(td::Map::TileTypes::WALL, {'#', '|'});
    map1.setSpriteSheet(sprite_sheet);

    // Add enemies
    Maps::addEnemy(map1, {{5, 8}, {7, 8}, {5, 12}}, 10);
    // Maps::addEnemy(map1, {{6,8}}, 5);

    // Add coins
    Maps::addCoin(map1, 3, 3);
    Maps::addCoin(map1, 5, 5);

    // Add map
    maps.emplace_back(map1);
    //------------------------------------------------------------------------------------------------------------------

    /* Map 2: */
    td::Map map2 = td::Map("../assets/maps/map2.txt");
    map2.setTileType(td::Map::TileTypes::WALL, {'#', '|'});
    map2.setSpriteSheet(sprite_sheet);

    // Add enemies
    int speed = 58;
    Maps::addEnemy(map2, {{4,13}, {4, 6}}, speed);
    Maps::addEnemy(map2, {{5,6}, {5, 13}}, speed);
    Maps::addEnemy(map2, {{6,13}, {6, 6}}, speed);
    Maps::addEnemy(map2, {{7,6}, {7, 13}}, speed);
    Maps::addEnemy(map2, {{8,13}, {8, 6}}, speed);

    // Add map
    maps.emplace_back(map2);
    //------------------------------------------------------------------------------------------------------------------

    return maps;
}


// Helper to make a new enemy and add it to a map
void Maps::addEnemy(td::Map& map, const std::vector<sf::Vector2f>& waypoints, int speed, int enemy_move_option,
                    int width, int height, sf::Color color, int harm) {
    auto enemy = new td::Enemy(map, width, height, color, harm);
    enemy->setWaypoints(waypoints);
    enemy->setMoveSpeed((float)speed);
    enemy->setMoveOption(enemy_move_option);
    map.addEnemy(enemy);
}


// Helper to make a new coin and add it to a map
void Maps::addCoin(td::Map& map, int row, int col, int width, int height, sf::Color color) {
    auto coin = new td::Item(width, height, color);
    coin->setStartTile(row, col);
    map.addItem(coin);
}
