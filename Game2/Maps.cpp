#include "Maps.h"

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
    td::Enemy enemy = td::Enemy();
    enemy.setSize((int)(tile_size*0.4), (int)(tile_size*0.4));
    enemy.setColor(sf::Color::Blue);
    enemy.setHarm(100);

    // Initialize the maps object to return
    std::vector<td::Map> maps = std::vector<td::Map>();
    //------------------------------------------------------------------------------------------------------------------

    /* Map 1: */
    td::Map map1 = td::Map("../assets/maps/map.txt");
    map1.setTileType(td::Map::TileTypes::WALL, {'#', '|'});
    map1.setSpriteSheet(sprite_sheet);

    // Add enemies
    enemy.setMap(map1);
    enemy.setStartTile(5, 8); map1.addEnemy(enemy);
    enemy.setStartTile(6, 8); map1.addEnemy(enemy);

    // Add map
    maps.emplace_back(map1);
    //------------------------------------------------------------------------------------------------------------------

    /* Map 2: */
    td::Map map2 = td::Map("../assets/maps/map2.txt");
    map2.setTileType(td::Map::TileTypes::WALL, {'#', '|'});
    map2.setSpriteSheet(sprite_sheet);

    // Add enemies
    enemy.setMap(map2);
    enemy.setStartTile(4, 8); map2.addEnemy(enemy);
    enemy.setStartTile(7, 8); map2.addEnemy(enemy);

    // Add map
    maps.emplace_back(map2);
    //------------------------------------------------------------------------------------------------------------------

    return maps;
}
