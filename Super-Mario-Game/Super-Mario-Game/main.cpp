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
			if (auto keyEvent = event->getIf<sf::Event::KeyPressed>()) {
				if (keyEvent->code == sf::Keyboard::Key::Escape) {
					paused = !paused;
				}
			}
		}

		update(deltaTime);

		window.clear();
		window.setView(camera.GetView(window.getSize()));

		Render(renderer);
		window.setView(camera.GetUIView());
		RenderUI(renderer);


		window.display();
	}
}



