#ifndef PLAYER_H
#define PLAYER_H

#include <TDAHelper/library.hpp>

class Player {
private:
    int x{};
    int y{};
public:
    // Constructor/destructor
    Player();
    ~Player();

    void setPosition(int pos_x, int pos_y);
    sf::RectangleShape getRenderObj() const;
};

#endif //PLAYER_H
