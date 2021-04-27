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
bool Game::paused() {
    this->pause--;
    return this->pause > 0;
}


// Initialize variables to default values
void Game::initVariables() {
    this->window = nullptr;
    this->fps = 60;
    this->pause = 0;
    this->tile_size = 8;
    this->view = sf::View();
    this->angle = 0;
}


// Initialize game fonts
void Game::initFonts() {
    this->font.loadFromFile("../assets/fonts/impact.ttf");
}


// Initialize game window
void Game::initWindow() {
    this->videoMode.width = 1000;
    this->videoMode.height = 650;
    this->background_color = sf::Color(158, 154, 229);

    // Allocate the window
    this->window = new sf::RenderWindow(
            this->videoMode, "World's Hardest Game", sf::Style::Titlebar | sf::Style::Close);

    // Center the window
    this->window->setPosition(sf::Vector2i(
            sf::VideoMode::getDesktopMode().width * 0.5 - this->window->getSize().x * 0.5,
            sf::VideoMode::getDesktopMode().height * 0.5 - this->window->getSize().y * 0.5
    ));

    // Set frame rate
    this->window->setFramerateLimit(this->fps);
}


// Initialize game map
void Game::initMap() {
    // Load in the map
    this->map = td::Map("../assets/map/map.txt");
    // this->map.setTileSize(this->tile_size);
    this->map.setTileType(td::Map::TileTypes::WALL, {'#', '|'});

    // Set the map's sprite sheet and ID mapping
    td::SpriteSheet sprite_sheet = td::SpriteSheet();
    sprite_sheet.addSprite('#', sf::Color::Transparent);
    sprite_sheet.addSprite('w', sf::Color::Black);
    sprite_sheet.addSprite('a', sf::Color::Yellow);
    sprite_sheet.addSprite('`', sf::Color(200, 200, 200));
    sprite_sheet.addSprite('\'', sf::Color::White);
    sprite_sheet.addSprite('c', sf::Color(139, 246, 153));
    sprite_sheet.addSprite('e', sf::Color(139, 246, 153));
    this->map.setSpriteSheet(sprite_sheet);

    td::Enemy enemy = td::Enemy();
    enemy.setMap(this->map);
    enemy.setSize((int)(this->tile_size*0.4), (int)(this->tile_size*0.4));
    enemy.setColor(sf::Color::Blue);
    enemy.setHarm(100);

    enemy.setStartTile(5, 8);
    this->map.addEnemy(enemy);
    enemy.setStartTile(6, 8);
    this->map.addEnemy(enemy);
}


// Initialize the player
void Game::initPlayer() {
    this->player = Player();
    this->player.p.setMap(this->map);
    this->player.p.setSize((int)(this->tile_size*0.8), (int)(this->tile_size*0.8), true);
    this->player.p.setMovementKeys(sf::Keyboard::W,sf::Keyboard::A,
                                   sf::Keyboard::S, sf::Keyboard::D);
    this->player.p.setMoveSpeed(30);
    this->player.p.setColor(sf::Color::Red);
}


// Update
void Game::update() {
    this->pollEvents();  // Poll for game loop events

    // Handle player movement
    this->player.p.move();

    if (this->player.p.onCheckpoint()) {
        this->player.p.setCheckpoint();
    }

    // Handle enemy collision
    if (this->player.p.isTouchingEnemy()) {
        std::vector<td::Enemy> touching_enemies = this->player.p.getTouchingEnemies();
        for (const auto& enemy: touching_enemies) {
            this->player.p.loseHealth(enemy.getHarm());
            if (this->player.p.isDead()) {
                this->player.p.respawn();
            }
        }
    }

    /*
    //Private
    this->player.p.setCheckpoint(tile);
    // Public
    DONE -- this->player.p.setMap(map); // Set the map the player is to use and roam around on
    DONE -- this->player.p.getHealth(); // Get health
    DONE -- this->player.p.isDead();    // If health <= 0
    DONE -- this->player.p.spawn();     // Place at starting location
    this->player.p.respawn();   // Place at latest checkpoint or starting location
    */
}


// Render
void Game::render() {
    // Clear previous frame renders
    this->window->clear(this->background_color);

    // td::Text::print(this->window, "Hello!", {.font=this->font, .y=100, .align=td::Text::Align::CENTER});

    this->view.reset(sf::FloatRect(0, 0, this->videoMode.width, this->videoMode.height));
    this->view.rotate(this->angle);
    // this->angle += 0.05;
    // this->view.setViewport(sf::FloatRect(0.f, 0.f, 0.5f, 1.f));
    // this->view.setCenter(this->player.p.getPosition(true));
    this->view.setCenter((float)(this->map.getMapSize().x)/2,(float)(this->map.getMapSize().y)/2);
    this->view.zoom(0.18);
    this->window->setView(this->view);

    // Render the map
    this->map.draw(this->window);

    // Render the player
    this->player.p.draw(this->window);

    // Render enemies
    this->map.drawEnemies(this->window);
    // this->enemy.draw(this->window);

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
