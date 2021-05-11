#ifndef MAPS_H
#define MAPS_H

#include <TDAHelper/library.hpp>

namespace Maps {
    static int enemy_width;
    static int enemy_height;
    static int enemy_harm;
    static int coin_width;
    static int coin_height;
    std::vector<td::Map> initMaps(int tile_size);
    void addEnemy(td::Map& map, const std::vector<sf::Vector2f>& waypoints,
                  int speed = 50, int enemy_move_option = td::Enemy::MoveOptions::LOOP,
                  int width = Maps::enemy_width, int height = Maps::enemy_height,
                  sf::Color color = sf::Color::Blue, int harm = Maps::enemy_harm);
    void addCoin(td::Map& map, int row, int col, int width = Maps::coin_width, int height = Maps::coin_height,
                 sf::Color color = sf::Color::Yellow);

    void checkpointOrder(td::Map& map, int order[]);

};


#endif //MAPS_H
