#ifndef ENGINE_LIBRARY_HPP
#define ENGINE_LIBRARY_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

namespace td {
    void test();

    class Window {
        private:
            // Variables
            sf::RenderWindow *window;
            sf::VideoMode videoMode;

        public:
            // Constructor/destructor
            Window();
            ~Window();

            // Initialization
            void setWindowSize(int w, int h);
            sf::RenderWindow* initWindow(int fps);
    };
}

#endif //ENGINE_LIBRARY_HPP
