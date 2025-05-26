#include <SFML/Graphics.hpp>

int main() {
    // Create a 800x600 window with title "SFML Test"
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test");

    // Create a circle shape with radius 50
    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(375, 275); // Center-ish

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window on exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear, draw, and display
        window.clear(sf::Color::Black);
        window.draw(circle);
        window.display();
    }

    return 0;
}