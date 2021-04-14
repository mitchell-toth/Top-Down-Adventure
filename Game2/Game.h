#ifndef GAME_H
#define GAME_H

#include <TDAHelper/library.hpp>

class Game {
    private:
        // Variables
        sf::RenderWindow* window;
        sf::VideoMode videoMode;
        sf::Event ev;
        int fps;
        int pause;

        // Fonts/text
        sf::Font font;
        sf::Text text;

        // Initialization
        void initVariables();
        void initFonts();
        void initText();
        void initWindow();

        // Game functions
        void pollEvents();

    public:
        // Constructor/destructor
        Game();
        virtual ~Game();

        // Game functions
        bool running() const;
        bool paused() const;
        void update();
        void render();
};


#endif //GAME_H
