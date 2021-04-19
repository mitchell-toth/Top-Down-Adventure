/*
 * Top-Down Adventure demo, made with C++, SFML, and a custom TDAHelper engine
 * Mitchell Toth
 * April 2021
 */

#include "Game.h"

int main() {
    std::cout << "Game 2" << std::endl;
    Game game = Game();
    while(game.running()) {
        if (!(game.paused()))
            game.update();
        game.render();
    }
    return 0;
}