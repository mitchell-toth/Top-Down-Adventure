#include "Game.h"

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
    this->numDeaths = 0;

    this->state = State::INTRO_SCREEN;
    // this->state = State::PLAYING;
}


// Initialize game fonts
void Game::initFonts() {
    this->regFont.loadFromFile("../assets/fonts/Aller_Rg.ttf");
    this->capsFont.loadFromFile("../assets/fonts/OstrichSans-Heavy.otf");
}


// Initialize game window
void Game::initWindow() {
    this->videoMode.width = 1000;
    this->videoMode.height = 750;
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
void Game::initMaps() {
    this->tile_size = 64;  // Tile size
    this->map_index = 1;  // Default is first map
    // Set up all the maps, complete with enemies and sprite sheets
    this->maps = Maps::initMaps(this->tile_size);
    // Set the current map to be the first one
    this->current_map = this->maps[this->map_index];
}


// Initialize the player
void Game::initPlayer() {
    this->player = Player();
    this->player.p.setMap(this->current_map);
    this->player.p.setSize((int)(this->tile_size*0.8), (int)(this->tile_size*0.8), true);
    this->player.p.setMovementKeys(sf::Keyboard::W,sf::Keyboard::A,
                                   sf::Keyboard::S, sf::Keyboard::D);
    this->player.p.setMoveSpeed(30);
    this->player.p.setTexture("../assets/textures/player.png");
}


// Initialize game music and sound effects
void Game::initSounds() {
    // Game music
    this->music = new td::Music("../assets/sounds/music.wav");
    this->music->play();

    // Sounds effects
    this->hitEnemySound = new td::Sound("../assets/sounds/enemy-hit.wav");
    this->mapTitleScreenSound = new td::Sound("../assets/sounds/map-title-screen.wav");
}


// Update
void Game::update() {
    this->pollEvents();  // Poll for game loop events

    // If not playing the game, don't bother handling game logic
    if (this->state != State::PLAYING) return;

    // Respawn the player after a pause
    if (this->respawnPlayer) {
        this->respawnPlayer = false;
        this->player.p.respawn();
    }

    // Handle player movement
    this->player.p.move(this->elapsed);

    if (this->player.p.onCheckpoint()) {
        this->player.p.setCheckpoint();
    }

    // Move enemies
    this->current_map.moveEnemies(this->elapsed);

    // Handle enemy collision. It's important that the enemies have moved before this point
    if (this->player.p.isTouchingEnemy()) {
        for (auto enemy: this->player.p.getTouchingEnemies()) {
            this->player.p.loseHealth(enemy->getHarm());
        }
    }

    // Handle item collision
    if (this->player.p.isTouchingItem()) {
        for (auto item: this->player.p.getTouchingItems()) {
            this->player.p.obtainItem(item);
        }
    }

    // Respawn if player is dead
    if (this->player.p.isDead()) {
        this->numDeaths++;
        this->hitEnemySound->play();
        this->pauseRespawn();
    }

    // Check if the player has reached the end goal
    if (this->player.p.onEnd()) {
        // Don't advance to the next map if the player hasn't collected all the map's coins
        if (this->player.p.getInventory().size() == this->current_map.getItems()->size()) {
            this->mapTitleScreenSound->play();
            this->loadNextMap();
        }
    }
}


// Render
void Game::render() {
    // Update the pause variable
    if (this->paused()) this->pause--;

    switch (this->state) {
        case State::INTRO_SCREEN:
            this->drawIntroScreen1();
            return;
        case MAIN_MENU:
            return;
        case State::LEVEL_SELECT:
            return;
        case MAP_TITLE_SCREEN:
            return;
        default:
            break;
    }

    // Clear previous frame renders
    this->window->clear(this->background_color);

    // Configure the camera view
    this->view.reset(sf::FloatRect(0, 0, this->videoMode.width, this->videoMode.height));
    this->view.rotate(this->angle);
    this->view.setCenter((float)(this->current_map.getMapSize().x)/2,(float)(this->current_map.getMapSize().y)/2);
    this->view.zoom(1.28);
    this->window->setView(this->view);

    // Render the map
    this->current_map.draw(this->window);

    // Draw the HUD
    std::stringstream ss;
    // Print the current level number
    ss << "LEVEL: " << this->map_index+1;
    td::Text::print(this->window, ss.str(),
                    {.font=this->capsFont, .x=(int)(this->current_map.getMapSize().x * 0.03), .y=-5, .size=60, .align=td::Text::Align::LEFT});
    // Print the number of fails
    ss.str(std::string());
    ss << "FAILS: " << this->numDeaths;
    td::Text::print(this->window, ss.str(),
                    {.font=this->capsFont, .x=(int)(this->current_map.getMapSize().x * 0.95), .y=-5, .size=60, .align=td::Text::Align::RIGHT});

    // Render the player
    this->player.p.draw(this->window);

    // Render items
    this->current_map.drawItems(this->window);
    // Render enemies
    this->current_map.drawEnemies(this->window);

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
    this->pause = 50;
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

    // Reset map items
    this->current_map.resetEnemies();
    this->current_map.resetItems();
}


// Create the first intro screen
void Game::drawIntroScreen1() {
    // Clear previous frame renders
    this->window->clear(sf::Color::Black);

    // Create a menu with one clickable button for "PLAY GAME"
    td::ClickableMenu menu = td::ClickableMenu(this->window, 0, (float)this->window->getSize().y/2 - 28,
                                               {10, 85, 10, 85},{{"PLAY GAME"}},
                                               {.font=this->capsFont, .size=32, .align=td::Text::Align::CENTER});
    menu.setOutline(sf::Color::White, 1);
    menu.drawMenu();
    menu.onMouseOver();

    // Print text
    td::Text::print(this->window, "Finished Loading!", {.font=this->regFont, .x=(int)(this->window->getSize().x/2), .y=(int)(this->window->getSize().y/2 - 60), .size=18, .align=td::Text::Align::RIGHT});
    td::Text::print(this->window, "This is The World's Hardest Game.", {.font=this->regFont, .x=(int)(this->window->getSize().x/2), .y=(int)(this->window->getSize().y/2 + 22), .size=18, .align=td::Text::Align::CENTER});
    td::Text::print(this->window, "It is harder than any game you have", {.font=this->regFont, .x=(int)(this->window->getSize().x/2), .y=(int)(this->window->getSize().y/2 + 44), .size=18, .align=td::Text::Align::CENTER});
    td::Text::print(this->window, "ever played, or ever will play.", {.font=this->regFont, .x=(int)(this->window->getSize().x/2), .y=(int)(this->window->getSize().y/2 + 66), .size=18, .align=td::Text::Align::CENTER});

    // Display the rendered objects
    this->window->display();
}
