#ifndef GAME_H
#define GAME_H

#include <TDAHelper/library.hpp>
#include "Player.h"
#include "Maps.h"

class Game {
    private:
        // Variables
        sf::RenderWindow* window{};
        sf::VideoMode videoMode;
        sf::Event ev{};
        sf::View view{};
        int fps{};
        int pause{};

        // State
        enum State {
            INTRO_SCREEN = 1,
            MAIN_MENU = 2,
            LEVEL_SELECT = 3,
            MAP_TITLE_SCREEN = 4,
            PLAYING = 5
        };
        State state{};

        // Map
        td::Map current_map;
        std::vector<td::Map> maps;
        int map_index{};

        // Map config
        int tile_size{};
        sf::Color background_color;
        float angle{};

        // Gameplay
        Player player;

        // Fonts/text
        sf::Font regFont;
        sf::Font capsFont;

        // Music
        td::Music* music{};

        // Sounds effects
        td::Sound* hitEnemySound{};
        td::Sound* mapTitleScreenSound{};

        // Gameplay
        bool respawnPlayer{};
        int numDeaths{};

        //Functions:
        // Initialization
        void initVariables();
        void initFonts();
        void initWindow();
        void initMaps();
        void initPlayer();
        void initSounds();

        // Game functions
        void pollEvents();

        // Gameplay
        void pauseRespawn();
        void loadNextMap();

        // Intro and title screen helpers
        void drawIntroScreen1();

    public:
        // Constructor/destructor
        Game();
        virtual ~Game();

        // Game clock
        sf::Clock clock;
        float elapsed{};

        // Game functions
        bool running() const;
        bool paused() const;
        void update();
        void render();
};


#endif //GAME_H
