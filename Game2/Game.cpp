#include "Game.h"

// Constructor
Game::Game() {
    this->initVariables();
    this->initFonts();
    this->initWindow();
    this->initMap();
    this->initPlayer();
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
    this->tile_size = 40;
}


// Initialize game fonts
void Game::initFonts() {
    this->font.loadFromFile("../assets/fonts/impact.ttf");
}


// Initialize game window
void Game::initWindow() {
    this->videoMode.width = 800;
    this->videoMode.height = 800;

    // Allocate the window
    this->window = new sf::RenderWindow(
            this->videoMode, "Title", sf::Style::Titlebar | sf::Style::Close);

    // Center the window
    this->window->setPosition(sf::Vector2i(
            sf::VideoMode::getDesktopMode().width * 0.5 - this->window->getSize().x * 0.5,
            sf::VideoMode::getDesktopMode().height * 0.5 - this->window->getSize().y * 0.5
    ));

    // Set frame rate
    this->window->setFramerateLimit(60);
}


// Initialize game map
void Game::initMap() {
    // Load in the map
    this->map = td::Map("../assets/map/map.txt");
    this->map.setTileSize(this->tile_size);

    // Set the map's sprite sheet and ID mapping
    td::SpriteSheet sprite_sheet = td::SpriteSheet();
    sprite_sheet.addSprite('1', sf::Color::Green);
    sprite_sheet.addSprite('2', sf::Color::Red);
    sprite_sheet.addSprite('3', sf::Color::Blue);
    this->map.setSpriteSheet(sprite_sheet);
}


// Initialize the player
void Game::initPlayer() {
    this->player = Player();
    this->player.p.setSize(this->tile_size, this->tile_size);
    this->player.p.setPosition(this->map, 1, 1);
    this->player.p.setMovementKeys(sf::Keyboard::W,sf::Keyboard::A,
                                   sf::Keyboard::S, sf::Keyboard::D);
}


// Update
void Game::update() {
    this->pollEvents();  // Poll for game loop events

    // Handle player movement
    this->player.p.move(this->map);
}


// Render
void Game::render() {
    // Clear previous frame renders
    this->window->clear();

    // td::Text::print(this->window, "Hello!", {.font=this->font, .y=100, .align=td::Text::Align::CENTER});

    // Render the map
    this->map.draw(this->window);
    // Render the player
    this->player.p.draw(this->window);

    // Display what has been rendered
    this->window->display();
}


// Event polling
void Game::pollEvents() {
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
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
