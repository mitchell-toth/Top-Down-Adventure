#include "game.h"

// Constructor
Game::Game() {
    this->initVariables();
    this->initFonts();
    this->initWindow();
    this->initMaps();
    this->initPlayer();
    this->initSounds();
}

// Destructor
Game::~Game() {
    delete this->window;
    // Release each map's enemy and item references
    for (auto map : this->maps) {
        // Enemies
        for (auto enemy : *map.getEnemies()) {
            delete enemy;
        }
        // Items
        for (auto item : *map.getItems()) {
            delete item;
        }
    }
    // Sounds
    delete this->music;
    delete this->hitEnemySound;
    delete this->mapTitleScreenSound;
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
    this->view = sf::View();
    this->angle = 0;
    this->respawnPlayer = false;
    this->elapsed = 0;
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
            this->videoMode, "Dungeon Explorer", sf::Style::Titlebar | sf::Style::Close);

    // Center the window
    this->window->setPosition(sf::Vector2i(
            sf::VideoMode::getDesktopMode().width * 0.5 - this->window->getSize().x * 0.5,
            sf::VideoMode::getDesktopMode().height * 0.5 - this->window->getSize().y * 0.5
    ));
    // Set frame rate
    this->window->setFramerateLimit(this->fps);
}


// Initialize game map
void Game::initMaps() {
    this->tile_size = 8;  // Tile size
    this->map_index = 0;  // Default is first map
    // Set up all the maps, complete with enemies and sprite sheets
    this->maps = Maps::initMaps(this->tile_size);
    // Set the current map to be the first one
    this->current_map = this->maps[this->map_index];
    this->numCheckpoints = this->current_map.checkpointList.size();
}


// Initialize the player
void Game::initPlayer() {
    this->player = Player();
    this->player.p.setMap(this->current_map);
    this->player.p.setSize((int)(this->tile_size*0.8), (int)(this->tile_size*0.8), true);
    this->player.p.setMovementKeys(sf::Keyboard::W,sf::Keyboard::A,
                                   sf::Keyboard::S, sf::Keyboard::D);
    this->player.p.setMoveSpeed(30);
    this->player.p.setTexture("../assets/sprites/Player.png");
}

// Initialize game music and sound effects
void Game::initSounds() {
    // Game music
}


// Update
void Game::update() {
    this->pollEvents();  // Poll for game loop events

    // Respawn the player after a pause
    if (this->respawnPlayer) {
        this->respawnPlayer = false;
        this->player.p.respawn();
    }

    // Handle player movement
    this->player.p.move(this->elapsed);

    if (this->player.p.onCheckpoint()) {
        this->player.p.setCheckpoint();
        if(this->player.p.getCheckpoint().row == this->current_map.checkpointList[abs(numCheckpoints-5)].x && this->player.p.getCheckpoint().col == this->current_map.checkpointList[abs(numCheckpoints-5)].y) {
            numCheckpoints--;
            std::cout << numCheckpoints << std::endl;
        }
        else if (this->player.p.getCheckpoint().row == this->current_map.checkpointList[abs(numCheckpoints-4)].x && this->player.p.getCheckpoint().col == this->current_map.checkpointList[abs(numCheckpoints-4)].y) {

        }
        else {
            numCheckpoints = this->current_map.checkpointList.size();
        }
    }

    // Move enemies
    this->current_map.moveEnemies(this->elapsed);

    // Handle enemy collision
    if (this->player.p.isTouchingEnemy()) {
        for (auto enemy: this->player.p.getTouchingEnemies()) {
            this->player.p.loseHealth(enemy->getHarm());
            lives--;
        }
    }

    // Handle item collision
    if (this->player.p.isTouchingItem()) {
        std::vector<td::Item*> touching_items = this->player.p.getTouchingItems();
        for (auto item: touching_items) {
            this->player.p.obtainItem(item);
        }
    }

    // Respawn if player is dead
    if (this->player.p.isDead()) {
        this->pauseRespawn();
    }

    // Check if the player has reached the end goal
    if (this->player.p.onEnd()) {
        // Don't advance to the next map if the player hasn't collected all the map's coins
        if(numCheckpoints == 0) {
            this->loadNextMap();
        }
        else {
            this->player.p.respawn();
        }
    }
}


// Render
void Game::render() {
    // Update the pause variable
    if (this->paused()) this->pause--;

    // Clear previous frame renders
    this->window->clear(this->background_color);

    // Configure the camera view
    this->view.reset(sf::FloatRect(0, 0, this->videoMode.width, this->videoMode.height));
    this->view.rotate(this->angle);
    this->view.setCenter((float)(this->current_map.getMapSize().x)/2,(float)(this->current_map.getMapSize().y)/2);
    this->view.zoom(0.16);
    this->window->setView(this->view);

    // Render the map
    this->current_map.draw(this->window);

    // Mark current checkpoint
    this->player.p.setCPTexture("../assets/sprites/fire.png");
    for(int i=0; i < abs(numCheckpoints-5); i++) {
        this->player.p.drawCP(this->window, this->current_map.checkpointList[i].y, this->current_map.checkpointList[i].x);
    }

    // Open door if all checkpoints lit
    if(numCheckpoints == 0){
        this->player.p.drawFileImage(this->window,5,19,"../assets/sprites/OCTR.png");
        this->player.p.drawFileImage(this->window,6,19,"../assets/sprites/newfloor2.png");
        this->player.p.drawFileImage(this->window,7,19,"../assets/sprites/OCBR.png");
    }

    // Render the player
    this->player.p.draw(this->window);

    // Render enemies
    this->current_map.drawEnemies(this->window);
    // Render items
    this->current_map.drawItems(this->window);

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


// Pause before respawning the player
void Game::pauseRespawn() {
    this->respawnPlayer = true;
    this->pause = 0;
}


// Load the next map and place the player on it
void Game::loadNextMap() {
    this->map_index++;
    if (this->map_index >= this->maps.size()) {
        this->map_index = 0;  // Loop back around to the first map
    }
    // Get the next map
    this->current_map = this->maps[this->map_index];
    // Configure player to use the new map
    this->player.p.setMap(this->current_map);
    this->player.p.clearInventory();
}
