#include "map.h"
#include "player.h"

/*
 * @brief Hello world
 * @param tile_size The tile size
 */
std::vector<td::Map> Maps::initMaps(int tile_size) {
    /* Configuration: */
    // Create a sprite sheet and ID mapping to use with the maps
    td::SpriteSheet sprite_sheet = td::SpriteSheet();
    sprite_sheet.addTexture('t',"../assets/sprites/newWall.png");
    sprite_sheet.addTexture('l',"../assets/sprites/newWallL.png");
    sprite_sheet.addTexture('r',"../assets/sprites/newWallR.png");
    sprite_sheet.addTexture('b',"../assets/sprites/newWallB.png");
    sprite_sheet.addTexture('C',"../assets/sprites/wallCenter.png");
    sprite_sheet.addTexture('T',"../assets/sprites/ICTR.png");
    sprite_sheet.addTexture('L',"../assets/sprites/ICTL.png");
    sprite_sheet.addTexture('R',"../assets/sprites/ICBR.png");
    sprite_sheet.addTexture('B',"../assets/sprites/ICBL.png");
    sprite_sheet.addTexture('1',"../assets/sprites/OCTR.png");
    sprite_sheet.addTexture('2',"../assets/sprites/OCTL.png");
    sprite_sheet.addTexture('3',"../assets/sprites/OCBR.png");
    sprite_sheet.addTexture('4',"../assets/sprites/OCBL.png");
    sprite_sheet.addTexture('5',"../assets/sprites/wallEnd.png");
    sprite_sheet.addTexture('6',"../assets/sprites/wallEndR.png");
    sprite_sheet.addTexture('7',"../assets/sprites/wallEndB.png");
    sprite_sheet.addTexture('8',"../assets/sprites/wallEndL.png");
    sprite_sheet.addTexture('w',"../assets/sprites/wallMid.png");
    sprite_sheet.addTexture('x',"../assets/sprites/wallMidR.png");
    sprite_sheet.addTexture('y',"../assets/sprites/wallMidB.png");
    sprite_sheet.addTexture('z',"../assets/sprites/wallMidL.png");
    sprite_sheet.addTexture('S',"../assets/sprites/rock.png");
    sprite_sheet.addSprite('a', sf::Color::Yellow);
    sprite_sheet.addTexture('f', "../assets/sprites/newFloor2.png");
    sprite_sheet.addSprite('\'',sf::Color(200, 200, 200));
    sprite_sheet.addTexture('s', "../assets/sprites/CPON.png");
    sprite_sheet.addTexture('c', "../assets/sprites/CP.png");
    sprite_sheet.addTexture('e', "../assets/sprites/Exit.png");


    // Enemy config
    td::Enemy* enemy;
    Maps::enemy_width = (int)(tile_size*0.75);
    Maps::enemy_height = (int)(tile_size*0.75);
    Maps::enemy_harm = 100;

    // Coin config
    td::Item* coin;
    Maps::coin_width = (int)(tile_size);
    Maps::coin_height = (int)(tile_size);

    // Initialize the maps object to return
    std::vector<td::Map> maps = std::vector<td::Map>();
    //------------------------------------------------------------------------------------------------------------------

    /* Map 1: */
    td::Map map1 = td::Map("../assets/maps/map.txt");
    map1.setTileSize(tile_size);
    map1.setTileType(td::Map::TileTypes::WALL, {'#', '|'});
    map1.setSpriteSheet(sprite_sheet);
    int map1checkpointOrder [5] = {4,1,2,0,3};
    Maps::checkpointOrder(map1, map1checkpointOrder);

    // Add enemies
    Maps::addEnemy(map1, {{5, 7},{10, 7},{10, 11},{11, 11},{11, 13},{7, 13}, {7, 10}, {5, 10}}, 10);
    Maps::addEnemy(map1, {{11, 13},{7, 13}, {7, 10}, {5, 10},{5, 7},{10, 7},{10, 11},{11, 11}}, 10);
    Maps::addEnemy(map1, {{1,15}, {2,15}}, 10);
    Maps::addEnemy(map1, {{2,18}, {11,18}}, 10);
    Maps::addEnemy(map1, {{1,7}, {3,7}, {3,9}, {1,9}}, 10);
    Maps::addEnemy(map1, {{6,4}, {10,4}, {10,6}, {6,6}}, 10);

    // Add lives
    Maps::addCoin(map1, 0, 17);
    Maps::addCoin(map1, 0, 18);
    Maps::addCoin(map1, 0, 19);

    // Add map
    maps.emplace_back(map1);
    //------------------------------------------------------------------------------------------------------------------

    /* Map 2: */
    td::Map map2 = td::Map("../assets/maps/map2.txt");
    map2.setTileSize(tile_size);
    map2.setTileType(td::Map::TileTypes::WALL, {'#', '|'});
    map2.setSpriteSheet(sprite_sheet);
    int map2checkpointOrder [5] = {4,1,2,0,3};
    Maps::checkpointOrder(map2, map2checkpointOrder);

    // Add enemies
    int speed = 58;
    Maps::addEnemy(map2, {{4,5}, {11, 5}}, speed);
    Maps::addEnemy(map2, {{7,1}, {7, 5}}, speed);
    Maps::addEnemy(map2, {{5,15}, {5, 18}}, speed);
    Maps::addEnemy(map2, {{1,15}, {8, 15}}, speed);

    // Add lives
    Maps::addCoin(map2, 0, 17);
    Maps::addCoin(map2, 0, 18);
    Maps::addCoin(map2, 0, 19);

    // Add map
    maps.emplace_back(map2);
    //------------------------------------------------------------------------------------------------------------------


    /* Map 3: */
    td::Map map3 = td::Map("../assets/maps/map3.txt");
    map3.setTileSize(tile_size);
    map3.setTileType(td::Map::TileTypes::WALL, {'#', '|'});
    map3.setSpriteSheet(sprite_sheet);
    int map3checkpointOrder [5] = {2,1,4,3,0};
    Maps::checkpointOrder(map3, map3checkpointOrder);

    // Add enemies
    speed = 30;
    Maps::addEnemy(map3, {{5,9}, {7, 9}, {7,11}, {5, 11}}, 30);
    Maps::addEnemy(map3, {{1,15}, {11, 15}}, speed);
    Maps::addEnemy(map3, {{2,5}, {2, 18}}, speed);
    Maps::addEnemy(map3, {{10,4}, {10, 18}}, speed);

    // Add lives
    Maps::addCoin(map3, 0, 17);
    Maps::addCoin(map3, 0, 18);
    Maps::addCoin(map3, 0, 19);


    // Add map
    maps.emplace_back(map3);
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
    enemy->setTexture("../assets/sprites/Enemy.png");
    map.addEnemy(enemy);
}


// Helper to make a new coin and add it to a map
void Maps::addCoin(td::Map& map, int row, int col, int width, int height, sf::Color color) {
    auto coin = new td::Item(map, width, height, color);
    coin->setStartTile(row, col);
    coin->setTexture("../assets/sprites/lives.png");
    map.addItem(coin);
}

void Maps::checkpointOrder(td::Map &map, int *order) {
    std::vector<sf::Vector2f> tempArray;
    for(int i=0; i < map.checkpointList.size(); i++) {
        tempArray.emplace_back(map.checkpointList[order[i]].x, map.checkpointList[order[i]].y);
    }
    map.checkpointList = tempArray;
}
