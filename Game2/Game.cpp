#include "Game.h"

// Constructor
Game::Game() {
    this->initVariables();
    this->initFonts();
    this->initWindow();
    this->initMaps();
    this->initView();
    this->initPlayer();
    this->initSounds();
    this->initMenus();
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
    this->zoom = 1.28;
    this->respawnPlayer = false;
    this->elapsed = 0;
    this->numDeaths = 0;

    this->state = State::INTRO_SCREEN;
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
    this->map_index = 0;   // Default is first map

    // Set up the base title screen "map"
    td::SpriteSheet title_sprite_sheet = td::SpriteSheet();
    title_sprite_sheet.addSprite('H', sf::Color::Black);
    title_sprite_sheet.addSprite('a', sf::Color(231, 231, 231));
    title_sprite_sheet.addSprite('b', sf::Color(233, 233, 233));
    title_sprite_sheet.addSprite('c', sf::Color(235, 235, 235));
    title_sprite_sheet.addSprite('d', sf::Color(237, 237, 237));
    title_sprite_sheet.addSprite('e', sf::Color(239, 239, 239));
    title_sprite_sheet.addSprite('f', sf::Color(241, 241, 241));
    title_sprite_sheet.addSprite('g', sf::Color(243, 243, 243));
    title_sprite_sheet.addSprite('h', sf::Color(245, 245, 245));
    title_sprite_sheet.addSprite('i', sf::Color(247, 247, 247));
    title_sprite_sheet.addSprite('j', sf::Color(249, 249, 249));
    title_sprite_sheet.addSprite('k', sf::Color(251, 251, 251));
    title_sprite_sheet.addSprite('l', sf::Color(253, 253, 253));
    title_sprite_sheet.addSprite('m', sf::Color(255, 255, 255));

    this->titleScreenBackground = td::Map("../assets/maps/title.txt");
    this->titleScreenBackground.setTileSize(this->tile_size);
    this->titleScreenBackground.setSpriteSheet(title_sprite_sheet);

    // Set up all the maps, complete with enemies and sprite sheets
    this->maps = Maps::initMaps(this->tile_size);
    // Set up the map title screens
    this->titleScreens = Maps::initTitleScreens();
    // Set the current map to be the first one
    this->current_map = this->maps[this->map_index];
}


// Initialize the camera view
void Game::initView() {
    this->view.reset(sf::FloatRect(0, 0, this->videoMode.width, this->videoMode.height));
    this->view.rotate(this->angle);
    this->view.setCenter((float)(this->current_map.getMapSize().x)/2,(float)(this->current_map.getMapSize().y)/2);
    this->view.zoom(this->zoom);
    this->window->setView(this->view);
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

    // Sounds effects
    this->hitEnemySound = new td::Sound("../assets/sounds/enemy-hit.wav");
    this->mapTitleScreenSound = new td::Sound("../assets/sounds/map-title-screen.wav");
}


void Game::initMenus() {
    // Intro menu on game load
    // One clickable button for "PLAY GAME"
    this->introMenu = td::ClickableMenu(this->window, (float)this->window->getSize().x/2 - 30, (float)this->window->getSize().y/2 - 30,
                                               {10, 85, 10, 85},{{"PLAY GAME"}},
                                               {.font=this->capsFont, .size=50, .align=td::Text::Align::CENTER});
    this->introMenu.setOutline(sf::Color::White, 1);

    // Main menu
    // Options for "PLAY GAME" and "LEVEL SELECT"
    this->titleMenu = td::ClickableMenu(this->window, (float)this->window->getSize().x/2 - 220, (float)this->window->getSize().y/2 + 20,
                                        {20, 85, 20, 85},{{"PLAY\nGAME", "LEVEL\nSELECT"}},
                                        {.font=this->capsFont, .size=100, .align=td::Text::Align::CENTER, .color=sf::Color::Black});
    this->titleMenu.setOptionColors({{sf::Color::Red, sf::Color::Green}});

    // HUD menu
    // One option for "MENU" to go back to the main menu
    this->hudMenu = td::ClickableMenu(this->window, (float)(this->window->getSize().x * 0.01), (float)(this->window->getSize().y * 0.93),
                                        {10},{{"MENU"}},{.font=this->capsFont, .size=60});
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
            this->map_index++;
            if (this->map_index >= this->maps.size()) {
                //TODO: win screen?
                this->map_index = 0;  // Loop back around to the first map
            }
            this->loadMap(this->map_index);
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
    this->view.zoom(this->zoom);
    this->window->setView(this->view);

    // Handle state and corresponding title screens
    switch (this->state) {
        case State::INTRO_SCREEN:
            this->drawIntroScreen(); return;
        case MAIN_MENU:
            this->drawMainMenu(); return;
        case State::LEVEL_SELECT:
            return;
        case MAP_TITLE_SCREEN:
            this->drawMapTitleScreen(); return;
        default:
            break;
    }

    // Render the map
    this->current_map.draw(this->window);

    // Draw the HUD
    this->drawHUD();

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
            case sf::Event::MouseButtonReleased:    // Watch for mouse clicks
                // If on the intro loading screen...
                if (this->state == State::INTRO_SCREEN) {
                    if (this->introMenu.onMouseClick() == "PLAY GAME") {
                        this->state = State::MAIN_MENU;
                        this->music->play();
                    }
                }
                // If in the main menu...
                else if (this->state == State::MAIN_MENU) {
                    if (this->titleMenu.onMouseClick() == "PLAY\nGAME") {
                        this->state = State::PLAYING;
                        this->loadMap(this->map_index);
                    }
                    else if (this->titleMenu.onMouseClick() == "LEVEL\nSELECT") {
                        this->state = State::LEVEL_SELECT;
                    }
                }
                // If playing the game...
                else if (this->state == State::PLAYING) {
                    if (this->hudMenu.onMouseClick() == "MENU") {
                        this->state = State::MAIN_MENU;
                    }
                }
        }
    }
}


// Pause before respawning the player
void Game::pauseRespawn() {
    this->respawnPlayer = true;
    this->pause = 50;
}


// Load the next map and place the player on it
void Game::loadMap(int map_idx) {
    this->mapTitleScreenSound->play();

    // Get the next map
    this->current_map = this->maps[map_idx];

    // Configure player to use the new map
    this->player.p.setMap(this->current_map);
    this->player.p.clearInventory();

    // Reset map items
    this->current_map.resetEnemies();
    this->current_map.resetItems();

    // Display the map's title screen
    this->state = State::MAP_TITLE_SCREEN;
    this->pause = 110;
}


// Render the initial game load screen
// Drawn when state is INTRO_SCREEN
void Game::drawIntroScreen() {
    // Clear previous frame renders
    this->window->clear(sf::Color::Black);

    this->introMenu.drawMenu();
    this->introMenu.onMouseOver();

    // Print text
    td::Text::print(this->window, "Finished Loading!", {.font=this->regFont, .x=(int)(this->window->getSize().x/2 - 130), .y=(int)(this->window->getSize().y/2 - 60), .size=22});
    td::Text::print(this->window, "This is The World's Hardest Game.", {.font=this->regFont, .y=(int)(this->window->getSize().y/2 + 22), .size=22, .align=td::Text::Align::CENTER});
    td::Text::print(this->window, "It is harder than any game you have", {.font=this->regFont, .y=(int)(this->window->getSize().y/2 + 44), .size=22, .align=td::Text::Align::CENTER});
    td::Text::print(this->window, "ever played, or ever will play.", {.font=this->regFont, .y=(int)(this->window->getSize().y/2 + 66), .size=22, .align=td::Text::Align::CENTER});

    // Display the rendered objects
    this->window->display();
}


// Render the game's main menu
// Drawn when state is MAIN_MENU
void Game::drawMainMenu() {
    // Clear previous frame renders
    this->window->clear(sf::Color::White);

    this->titleScreenBackground.draw(this->window);

    this->titleMenu.drawMenu();
    this->titleMenu.onMouseOver();

    td::Text::print(this->window, "THE WORLD'S", {.font=this->capsFont, .x=115, .y=140, .size=40, .color=sf::Color::Black});
    td::Text::print(this->window, "HARDEST GAME", {.font=this->capsFont, .y=125, .size=220, .align=td::Text::Align::CENTER, .color=sf::Color(70, 134, 188)});

    td::Text::print(this->window, "MITCHELL TOTH",
                    {.font=this->capsFont, .x=(int)(this->window->getSize().x * 0.02), .y=-5, .size=60, .align=td::Text::Align::LEFT});

    td::Text::print(this->window, "MUSIC: SNAYK",
                    {.font=this->capsFont, .x=(int)(this->window->getSize().x * 0.98), .y=-5, .size=60, .align=td::Text::Align::RIGHT});

    // Display the rendered objects
    this->window->display();
}


// Show the title screen that corresponds to the current map
// Drawn when state is MAP_TITLE_SCREEN
void Game::drawMapTitleScreen() {
    if (!this->paused())
        this->state = State::PLAYING;

    this->window->clear(sf::Color::White);
    this->titleScreenBackground.draw(this->window);

    std::string s = titleScreens[this->map_index];

    int y = (int)(this->window->getSize().y/2) - 160;
    int next = 0;
    int last = 0;
    while ((next = s.find('\n', last)) != std::string::npos) {
        td::Text::print(this->window, s.substr(last, next-last), {.font=this->capsFont, .y=y, .size=130, .align=td::Text::Align::CENTER, .color=sf::Color::Black});
        last = next + 1;
        y += 100;
    }
    td::Text::print(this->window, s.substr(last), {.font=this->capsFont, .y=y, .size=130, .align=td::Text::Align::CENTER, .color=sf::Color::Black});
    this->window->display();
}


// Render the level text, number of fails, and a button to go back to the Main Menu
// Drawn when state is PLAYING
void Game::drawHUD() {
    std::stringstream ss;
    // Print the current level number
    ss << "LEVEL: " << this->map_index+1;
    td::Text::print(this->window, ss.str(),
                    {.font=this->capsFont, .x=(int)(this->window->getSize().x * 0.02), .y=-5, .size=60, .align=td::Text::Align::LEFT});

    // Print the number of fails
    ss.str(std::string());
    ss << "FAILS: " << this->numDeaths;
    td::Text::print(this->window, ss.str(),
                    {.font=this->capsFont, .x=(int)(this->window->getSize().x * 0.98), .y=-5, .size=60, .align=td::Text::Align::RIGHT});

    // Draw the "MENU" button
    this->hudMenu.drawMenu();
    this->hudMenu.onMouseOver();
}
