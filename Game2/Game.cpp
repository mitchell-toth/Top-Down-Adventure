#include "Game.h"

// Constructor
Game::Game() {
    this->initVariables();
    this->initFonts();
    this->initText();
    this->initWindow();
}


// Destructor
Game::~Game() {
    delete this->window;
}


// Is the game running?
bool Game::running() const {
    return this->window->isOpen();
}


// Is the game paused?
bool Game::paused() const {
    return this->pause > 0;
}


// Initialize variables to default values
void Game::initVariables() {
    this->window = nullptr;
    this->fps = 60;
    this->pause = 0;
}


// Initialize game fonts
void Game::initFonts() {
    this->font.loadFromFile("assets/fonts/impact.ttf");
}


// Initialize game text
void Game::initText() {
    this->text.setFont(this->font);
    this->text.setCharacterSize(12);
    this->text.setFillColor(sf::Color::White);
    this->text.setString(("NONE"));
}


// Initialize game window
void Game::initWindow() {
    td::Window w = td::Window();
    this->window = w.initWindow(this->fps);
}


// Update
void Game::update() {
    this->pollEvents();  // Poll for game loop events
}


// Render
void Game::render() {
    // Clear previous frame renders
    this->window->clear();

    // Display on window
    this->window->display();
}


// Event polling
void Game::pollEvents() {
    while(this->window->pollEvent(this->ev)) {
        switch(this->ev.type) {
            case sf::Event::Closed:                 // Red X to close window
                this->window->close();
                break;
            case sf::Event::KeyPressed:             // Watch for key presses
                if (this->ev.key.code == sf::Keyboard::Escape) {
                    this->window->close();
                }
                break;
        }
    }
}
