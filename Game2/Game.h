#ifndef GAME_H
#define GAME_H

#include <TDAHelper/library.hpp>
#include "Player.h"

class Game {
    private:
        // Variables
        sf::RenderWindow* window{};
        sf::VideoMode videoMode;
        sf::Event ev{};
        sf::View view{};
        int fps{};
        int pause{};

        // Map
        td::Map map;
        int tile_size{};
        sf::Color background_color;
        float angle;

        // Gameplay
        Player player;
        // std::vector<td::Enemy> enemies;

        // Fonts/text
        sf::Font font;

        // Initialization
        void initVariables();
        void initFonts();
        void initWindow();
        void initMap();
        void initPlayer();

        // Game functions
        void pollEvents();

    public:
        // Constructor/destructor
        Game();
        virtual ~Game();

        // Game functions
        bool running() const;
        bool paused();
        void update();
        void render();
};


#endif //GAME_H
