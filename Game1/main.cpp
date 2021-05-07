
#include "Game.h"

int main() {
    std::cout << "Game 1" << std::endl;
    Game game = Game();
    while(game.running()) {
        game.elapsed = game.clock.restart().asSeconds();
        if (!(game.paused()))
            game.update();
        game.render();
    }
    return 0;
}