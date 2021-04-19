#include "Player.h"

Player::Player() = default;

Player::~Player() = default;

void Player::setPosition(int pos_x, int pos_y) {
    this->x = pos_x;
    this->y = pos_y;
}

sf::RectangleShape Player::getRenderObj() const {
    return td::Shapes::rect(this->x, this->y, 20, 20, sf::Color::Yellow);
}
