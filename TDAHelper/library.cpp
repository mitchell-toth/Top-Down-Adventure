#include "library.hpp"

/* General */

// A simple test method to see if the Engine is linked correctly
void td::test() {
    std::cout << "Hello, World!" << std::endl;
}


/* Text */

// Helper to print text to the screen. Takes a window target, a text string, and a configuration object.
void td::Text::print(sf::RenderTarget* target, const std::string &s, td::Text::Config config) {
    // Set up the text, size, and color
    sf::Text* text = &config.text;
    text->setString(s);
    text->setCharacterSize(config.size);
    text->setFillColor(config.color);

    // Set horizontal alignment
    if (config.align == td::Text::Align::LEFT) {  // Align left
        text->setPosition((float)config.x, (float)config.y);
    }
    else if (config.align == td::Text::Align::CENTER) {  // Center text horizontally. Ignore the x passed in
        text->setPosition(
                (float)((target->getSize().x * 0.5) - (text->getLocalBounds().width * 0.5)), (float)config.y);
    }
    else {  // Align right
        text->setPosition((float)config.x - text->getLocalBounds().width, (float)config.y);
    }

    // Draw text to target window
    target->draw(*text);
}


/* Shapes */

// Construct and return a rectangle
sf::RectangleShape td::Shapes::rect(float x, float y, float width, float height, sf::Color color) {
    sf::RectangleShape rect;
    rect.setPosition(sf::Vector2f(x, y));
    rect.setSize(sf::Vector2f(width, height));
    rect.setFillColor(color);
    return rect;
}

// Construct and return a line
sf::VertexArray td::Shapes::line(float x1, float y1, float x2, float y2, sf::Color color) {
    sf::VertexArray l(sf::LinesStrip, 2);
    l[0].position = sf::Vector2f(x1, y1);
    l[0].color  = sf::Color(color);
    l[1].position = sf::Vector2f(x2, y2);
    l[1].color  = sf::Color(color);
    return l;
}
