#include "Game.h"
#include "Resources.h"
#include <filesystem>
#include <iostream>

sf::Texture texture;

void Begin(const sf::Window& window) {
    std::filesystem::path resourcePath = "../resources/";

    if (std::filesystem::exists(resourcePath)) {
        for (const auto& entry : std::filesystem::directory_iterator(resourcePath)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".png" || ext == ".jpg") {

                    std::string key = entry.path().filename().string();

                    if (Resources::textures[key].loadFromFile(entry.path().string())) {
                        std::cout << "LOADED: " << key << " from " << entry.path() << std::endl;
                    }
                }
            }
        }
    }
    else {
        std::cout << std::filesystem::absolute(resourcePath) << std::endl;
    }
}

void update(float dTime) {

}

void render(Renderer& renderer) {
	renderer.Draw(Resources::textures["block.png"], sf::Vector2f(), sf::Vector2f(2, 2));

}