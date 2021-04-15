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
    this->font.loadFromFile("../assets/fonts/impact.ttf");
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


// Update
void Game::update() {
    this->pollEvents();  // Poll for game loop events
}


// Render
void Game::render() {
    // Clear previous frame renders
    this->window->clear();

    td::Text::Config c = {.text=this->text, .align=td::Text::Align::CENTER};
    td::Text::print(this->window, "Hello!", {.text=this->text, .y=100, .align=td::Text::Align::CENTER});
    std::stringstream ss;
    ss << "Hello world!" << " Hi there";
    td::Text::print(this->window, ss.str(), c);

    sf::RectangleShape rect = td::Shapes::rect(200, 200, 50, 50);
    this->window->draw(rect);

    sf::VertexArray line = td::Shapes::line(0, 0, 800, 800, sf::Color::Red);
    this->window->draw(line);

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
