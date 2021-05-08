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
    sprite_sheet.addSprite('h', sf::Color::Black);
    sprite_sheet.addSprite('w', sf::Color::Black);
    sprite_sheet.addSprite('a', sf::Color::Yellow);
    sprite_sheet.addSprite('`', sf::Color::White);
    sprite_sheet.addSprite('\'',sf::Color(220, 220, 220));
    sprite_sheet.addSprite('s', sf::Color(139, 246, 153));
    sprite_sheet.addSprite('c', sf::Color(139, 246, 153));
    sprite_sheet.addSprite('e', sf::Color(139, 246, 153));

    // Enemy config
    td::Enemy* enemy;
    Maps::enemy_width = (int)(tile_size*0.45);
    Maps::enemy_height = (int)(tile_size*0.45);
    Maps::enemy_harm = 100;

    // Coin config
    td::Item* coin;
    Maps::coin_width = (int)(tile_size*0.4);
    Maps::coin_height = (int)(tile_size*0.4);

    // Initialize the maps object to return
    std::vector<td::Map> maps = std::vector<td::Map>();

    td::Map map = td::Map();
    //------------------------------------------------------------------------------------------------------------------

    /* Map 1: */
    map = td::Map("../assets/maps/map1.txt");
    map.setTileSize(tile_size);
    map.setTileType(td::Map::TileTypes::WALL, {'#'});
    map.setSpriteSheet(sprite_sheet);

    // Add enemies
    int speed = 58;
    Maps::addEnemy(map, {{5,13}, {5, 6}}, speed);
    Maps::addEnemy(map, {{6,6}, {6, 13}}, speed);
    Maps::addEnemy(map, {{7,13}, {7, 6}}, speed);
    Maps::addEnemy(map, {{8,6}, {8, 13}}, speed);
    Maps::addEnemy(map, {{9,13}, {9, 6}}, speed);

    // Add map
    maps.emplace_back(map);
    //------------------------------------------------------------------------------------------------------------------

    /* Map 2: */
    map = td::Map("../assets/maps/map2.txt");
    map.setTileSize(tile_size);
    map.setTileType(td::Map::TileTypes::WALL, {'#'});
    map.setSpriteSheet(sprite_sheet);

    // Add enemies
    speed = 40;
    Maps::addEnemy(map, {{4, 5}, {10, 5}}, speed);
    Maps::addEnemy(map, {{10, 6}, {4, 6}}, speed);
    Maps::addEnemy(map, {{4, 7}, {10, 7}}, speed);
    Maps::addEnemy(map, {{10, 8}, {4, 8}}, speed);
    Maps::addEnemy(map, {{4, 9}, {10, 9}}, speed);
    Maps::addEnemy(map, {{10, 10}, {4, 10}}, speed);
    Maps::addEnemy(map, {{4, 11}, {10, 11}}, speed);
    Maps::addEnemy(map, {{10, 12}, {4, 12}}, speed);
    Maps::addEnemy(map, {{4, 13}, {10, 13}}, speed);
    Maps::addEnemy(map, {{10, 14}, {4, 14}}, speed);

    // Add coins
    Maps::addCoin(map, 6, 9);
    Maps::addCoin(map, 6, 10);
    Maps::addCoin(map, 7, 9);
    Maps::addCoin(map, 7, 10);
    Maps::addCoin(map, 8, 9);
    Maps::addCoin(map, 8, 10);

    // Add map
    maps.emplace_back(map);
    //------------------------------------------------------------------------------------------------------------------

    /* Map 3: */

    map = td::Map("../assets/maps/map3.txt");
    map.setTileSize(tile_size);
    map.setTileType(td::Map::TileTypes::WALL, {'#'});
    map.setTileType(td::Map::TileTypes::CHECKPOINT, {'e', 'c'});
    map.setSpriteSheet(sprite_sheet);

    // Add enemies
    speed = 40;
    Maps::addEnemy(map, {{10, 13}, {10, 6}, {4, 6}, {4, 13}}, speed);
    Maps::addEnemy(map, {{9, 13}, {10, 13}, {10, 6}, {4, 6}, {4, 13}}, speed);
    Maps::addEnemy(map, {{8, 13}, {10, 13}, {10, 6}, {4, 6}, {4, 13}}, speed);
    Maps::addEnemy(map, {{7, 13}, {10, 13}, {10, 6}, {4, 6}, {4, 13}}, speed);
    Maps::addEnemy(map, {{6, 13}, {10, 13}, {10, 6}, {4, 6}, {4, 13}}, speed);
    Maps::addEnemy(map, {{5, 13}, {10, 13}, {10, 6}, {4, 6}, {4, 13}}, speed);
    Maps::addEnemy(map, {{4, 13}, {10, 13}, {10, 6}, {4, 6}}, speed);
    Maps::addEnemy(map, {{4, 12}, {4, 13}, {10, 13}, {10, 6}, {4, 6}}, speed);
    Maps::addEnemy(map, {{4, 11}, {4, 13}, {10, 13}, {10, 6}, {4, 6}}, speed);

    Maps::addEnemy(map, {{4, 6}, {4, 13}, {10, 13}, {10, 6}}, speed);
    Maps::addEnemy(map, {{5, 6}, {4, 6}, {4, 13}, {10, 13}, {10, 6}}, speed);
    Maps::addEnemy(map, {{6, 6}, {4, 6}, {4, 13}, {10, 13}, {10, 6}}, speed);
    Maps::addEnemy(map, {{7, 6}, {4, 6}, {4, 13}, {10, 13}, {10, 6}}, speed);
    Maps::addEnemy(map, {{8, 6}, {4, 6}, {4, 13}, {10, 13}, {10, 6}}, speed);
    Maps::addEnemy(map, {{9, 6}, {4, 6}, {4, 13}, {10, 13}, {10, 6}}, speed);
    Maps::addEnemy(map, {{10, 6}, {4, 6}, {4, 13}, {10, 13}}, speed);
    Maps::addEnemy(map, {{10, 7}, {10, 6}, {4, 6}, {4, 13}, {10, 13}}, speed);
    Maps::addEnemy(map, {{10, 8}, {10, 6}, {4, 6}, {4, 13}, {10, 13}}, speed);

    // Add coins
    Maps::addCoin(map, 3, 8);
    Maps::addCoin(map, 4, 14);
    Maps::addCoin(map, 11, 11);
    Maps::addCoin(map, 10, 5);

    // Add map
    maps.emplace_back(map);
    //------------------------------------------------------------------------------------------------------------------

    /* Map 4: */

    map = td::Map("../assets/maps/map4.txt");
    map.setTileSize(tile_size);
    map.setTileType(td::Map::TileTypes::WALL, {'#'});
    map.setTileType(td::Map::TileTypes::CHECKPOINT, {'s'});
    map.setTileType(td::Map::TileTypes::END, {'s'});
    map.setSpriteSheet(sprite_sheet);

    // Add enemies
    speed = 26;
    Maps::addEnemy(map, {{6, 5}, {3, 5}, {3, 8}, {6, 8}}, speed);
    Maps::addEnemy(map, {{9, 5}, {6, 5}, {6, 8}, {9, 8}}, speed);
    Maps::addEnemy(map, {{11, 5}, {9, 5}, {9, 8}, {11, 8}}, speed);
    Maps::addEnemy(map, {{4, 8}, {4, 11}, {10, 11}, {10, 8}}, speed);
    Maps::addEnemy(map, {{5, 11}, {5, 14}, {11, 14}, {11, 11}}, speed);
    Maps::addEnemy(map, {{11, 14}, {11, 11}, {5, 11}, {5, 14}}, speed);
    Maps::addEnemy(map, {{3, 11}, {3, 14}, {5, 14}, {5, 11}}, speed);

    // Add coins
    Maps::addCoin(map, 4, 8);
    Maps::addCoin(map, 4, 11);
    Maps::addCoin(map, 4, 14);
    Maps::addCoin(map, 6, 5);
    Maps::addCoin(map, 6, 10);
    Maps::addCoin(map, 9, 13);
    Maps::addCoin(map, 10, 11);
    Maps::addCoin(map, 10, 8);
    Maps::addCoin(map, 10, 5);
    Maps::addCoin(map, 11, 14);

    // Add map
    maps.emplace_back(map);
    //------------------------------------------------------------------------------------------------------------------

    /* Map 5: */

    map = td::Map("../assets/maps/map5.txt");
    map.setTileSize(tile_size);
    map.setTileType(td::Map::TileTypes::WALL, {'#'});
    map.setSpriteSheet(sprite_sheet);

    // Add enemies
    speed = 10;
    Maps::addEnemy(map, {{6, 7}, {6, 8}}, speed);
    Maps::addEnemy(map, {{7, 8}, {7, 7}}, speed);
    Maps::addEnemy(map, {{8, 7}, {8, 8}}, speed);
    Maps::addEnemy(map, {{8, 5}, {8, 6}}, speed);
    Maps::addEnemy(map, {{8, 5}, {8, 6}}, speed);
    Maps::addEnemy(map, { {9, 6}, {9, 5}}, speed);
    Maps::addEnemy(map, {{10, 5}, {10, 6}}, speed);

    Maps::addEnemy(map, {{10, 7}, {11, 7}}, speed);
    Maps::addEnemy(map, {{11, 8}, {10, 8}}, speed);
    Maps::addEnemy(map, {{10, 9}, {11, 9}}, speed);
    Maps::addEnemy(map, {{11, 10}, {10, 10}}, speed);
    Maps::addEnemy(map, {{10, 11}, {11, 11}}, speed);
    Maps::addEnemy(map, {{11, 12}, {10, 12}}, speed);

    Maps::addEnemy(map, {{6, 11}, {6, 12}}, speed);
    Maps::addEnemy(map, {{7, 12}, {7, 11}}, speed);
    Maps::addEnemy(map, {{8, 11}, {8, 12}}, speed);
    Maps::addEnemy(map, {{8, 13}, {8, 14}}, speed);
    Maps::addEnemy(map, {{9, 14}, {9, 13}}, speed);
    Maps::addEnemy(map, {{10, 13}, {10, 14}}, speed);

    // Add map
    maps.emplace_back(map);
    //------------------------------------------------------------------------------------------------------------------

    /* Map 6: */

    map = td::Map("../assets/maps/map6.txt");
    map.setTileSize(tile_size);
    map.setTileType(td::Map::TileTypes::WALL, {'#'});
    map.setSpriteSheet(sprite_sheet);

    // Add enemies
    speed = 28;
    Maps::addEnemy(map, {{4, 3}, {4, 4}, {5, 4}, {5, 3}}, speed);
    Maps::addEnemy(map, {{4, 5}, {4, 6}, {5, 6}, {5, 5}}, speed);
    Maps::addEnemy(map, {{6, 3}, {6, 4}, {7, 4}, {7, 3}}, speed);
    Maps::addEnemy(map, {{6, 5}, {6, 6}, {7, 6}, {7, 5}}, speed);
    Maps::addEnemy(map, {{8, 3}, {8, 4}, {9, 4}, {9, 3}}, speed);
    Maps::addEnemy(map, {{8, 5}, {8, 6}, {9, 6}, {9, 5}}, speed);
    Maps::addEnemy(map, {{10, 3}, {10, 4}, {11, 4}, {11, 3}}, speed);
    Maps::addEnemy(map, {{10, 5}, {10, 6}, {11, 6}, {11, 5}}, speed);

    Maps::addEnemy(map, {{10, 7}, {10, 8}, {11, 8}, {11, 7}}, speed);
    Maps::addEnemy(map, {{10, 9}, {10, 10}, {11, 10}, {11, 9}}, speed);
    Maps::addEnemy(map, {{8, 8}, {8, 9}, {9, 9}, {9, 8}}, speed);
    Maps::addEnemy(map, {{8, 10}, {8, 11}, {9, 11}, {9, 10}}, speed);
    Maps::addEnemy(map, {{6, 8}, {6, 9}, {7, 9}, {7, 8}}, speed);
    Maps::addEnemy(map, {{6, 10}, {6, 11}, {7, 11}, {7, 10}}, speed);
    Maps::addEnemy(map, {{4, 9}, {4, 10}, {5, 10}, {5, 9}}, speed);
    Maps::addEnemy(map, {{4, 11}, {4, 12}, {5, 12}, {5, 11}}, speed);

    Maps::addEnemy(map, {{4, 13}, {4, 14}, {5, 14}, {5, 13}}, speed);
    Maps::addEnemy(map, {{4, 15}, {4, 16}, {5, 16}, {5, 15}}, speed);
    Maps::addEnemy(map, {{6, 13}, {6, 14}, {7, 14}, {7, 13}}, speed);
    Maps::addEnemy(map, {{6, 15}, {6, 16}, {7, 16}, {7, 15}}, speed);
    Maps::addEnemy(map, {{8, 13}, {8, 14}, {9, 14}, {9, 13}}, speed);
    Maps::addEnemy(map, {{8, 15}, {8, 16}, {9, 16}, {9, 15}}, speed);
    Maps::addEnemy(map, {{10, 13}, {10, 14}, {11, 14}, {11, 13}}, speed);
    Maps::addEnemy(map, {{10, 15}, {10, 16}, {11, 16}, {11, 15}}, speed);

    // Add coins
    Maps::addCoin(map, 10, 3);
    Maps::addCoin(map, 10, 4);
    Maps::addCoin(map, 11, 3);
    Maps::addCoin(map, 11, 4);

    Maps::addCoin(map, 7, 9);
    Maps::addCoin(map, 7, 10);
    Maps::addCoin(map, 8, 9);
    Maps::addCoin(map, 8, 10);

    Maps::addCoin(map, 4, 15);
    Maps::addCoin(map, 4, 16);
    Maps::addCoin(map, 5, 15);
    Maps::addCoin(map, 5, 16);

    // Add map
    maps.emplace_back(map);
    //------------------------------------------------------------------------------------------------------------------

    /* Map 7: */

    map = td::Map("../assets/maps/map7.txt");
    map.setTileSize(tile_size);
    map.setTileType(td::Map::TileTypes::WALL, {'#'});
    map.setSpriteSheet(sprite_sheet);

    // Add enemies
    speed = 36;
    Maps::addEnemy(map, {{5, 5}, {9, 5}}, speed);
    Maps::addEnemy(map, {{9, 6}, {5, 6}}, speed);
    Maps::addEnemy(map, {{5, 7}, {9, 7}}, speed);
    Maps::addEnemy(map, {{9, 8}, {5, 8}}, speed);
    Maps::addEnemy(map, {{5, 9}, {9, 9}}, speed);
    Maps::addEnemy(map, {{9, 10}, {5, 10}}, speed);
    Maps::addEnemy(map, {{5, 11}, {9, 11}}, speed);
    Maps::addEnemy(map, {{9, 12}, {5, 12}}, speed);
    Maps::addEnemy(map, {{5, 13}, {9, 13}}, speed);
    Maps::addEnemy(map, {{9, 14}, {5, 14}}, speed);

    Maps::addEnemy(map, {{5, 5}, {9, 9}, {5, 13}, {6, 14}, {9, 11}, {5, 7}, {7, 5}, {9, 7}, {5, 11}, {8, 14}, {9, 13}, {5, 9}, {9, 5}}, 43, true, td::Enemy::MoveOptions::BACK_AND_FORTH);
    Maps::addEnemy(map, {{9, 5}, {5, 9}, {9, 13}, {8, 14}, {5, 11}, {9, 7}, {7, 5}, {5, 7}, {9, 11}, {6, 14}, {5, 13}, {9, 9}, {5, 5}}, 43, true, td::Enemy::MoveOptions::BACK_AND_FORTH);

    // Add map
    maps.emplace_back(map);
    //------------------------------------------------------------------------------------------------------------------

    return maps;
}


// Configure the title text that will make up each map's title screen
std::vector<std::string> Maps::initTitleScreens() {
    std::vector<std::string> titles = std::vector<std::string>();
    titles.emplace_back("YOU DON'T KNOW WHAT\nYOU'RE GETTING INTO.");
    titles.emplace_back("DON'T EVEN BOTHER\nTRYING.");
    titles.emplace_back("I CAN ALMOST\nGUARANTEE THAT\nYOU WILL FAIL.");
    titles.emplace_back("DON'T GET CONFUSED,\nNOW.");
    titles.emplace_back("HARDER THAN IT\nLOOKS.");
    titles.emplace_back("GIVE UP, THIS ONE\nISN'T EVEN HARD.");
    titles.emplace_back("NOT SO EASY,\nIS IT?");
    return titles;
}


// Helper to make a new enemy and add it to a map
void Maps::addEnemy(td::Map& map, const std::vector<sf::Vector2f>& waypoints, int speed, bool tiles,
                    int enemy_move_option, int width, int height, sf::Color color, int harm) {
    auto enemy = new td::Enemy(map, width, height, color, harm);
    enemy->setWaypoints(waypoints, tiles);
    enemy->setMoveSpeed((float)speed);
    enemy->setMoveOption(enemy_move_option);
    enemy->setTexture("../assets/textures/enemy.png");
    map.addEnemy(enemy);
}


// Helper to make a new coin and add it to a map
void Maps::addCoin(td::Map& map, int row, int col, int width, int height, sf::Color color) {
    auto coin = new td::Item(map, width, height, color);
    coin->setStartTile(row, col);
    coin->setTexture("../assets/textures/coin.png");
    map.addItem(coin);
}
