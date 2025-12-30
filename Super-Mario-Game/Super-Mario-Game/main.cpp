#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include "Game.h"
#include "Camera.h"
#include "Renderer.h"

int main() {
	sf::RenderWindow window(sf::VideoMode({ 1200,900 }), "Super Mario");
	sf::Clock deltaClock;
	Renderer renderer(window);

	window.setFramerateLimit(60);
	Begin(window);

	while (window.isOpen()) {
		float deltaTime = deltaClock.restart().asSeconds();
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			
			}

		}
		window.setView(camera.GetView(window.getSize()));

		update(deltaTime);

		window.clear();

		Render(renderer);

		window.display();
	}


}

