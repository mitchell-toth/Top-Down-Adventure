#include "library.hpp"

/* General */

// A simple test method to see if the Engine is linked correctly
void td::test() {
    std::cout << "Hello, World!" << std::endl;
}


/* class Window */

// Window constructor
td::Window::Window() {
    this->window = nullptr;
    this->videoMode.width = 800;
    this->videoMode.height = 800;
}


// Window destructor
td::Window::~Window() {
    delete this->window;
}


// Set the size of the game window
void td::Window::setWindowSize(int w, int h) {
    this->videoMode.width = w;
    this->videoMode.height = h;
}


// Initialize game window
sf::RenderWindow *td::Window::initWindow(int fps) {
    // Allocate the window
    this->window = new sf::RenderWindow(
            this->videoMode, "Strong Pong (C++, SFML)", sf::Style::Titlebar | sf::Style::Close);

    // Center the window
    this->window->setPosition(sf::Vector2i(
            sf::VideoMode::getDesktopMode().width * 0.5 - this->window->getSize().x * 0.5,
            sf::VideoMode::getDesktopMode().height * 0.5 - this->window->getSize().y * 0.5
    ));

    // Set frame rate
    this->window->setFramerateLimit(fps);
    return this->window;
}
