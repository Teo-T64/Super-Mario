#include "Game.h"
#include "Resources.h"
#include <filesystem>
#include <iostream>
#include "Map.h"
#include "Mario.h"
#include "Physics.h"

Map map(1.0f);
Camera camera(20.0f);
Mario mario;


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

    Physics::Init();

    sf::Image image;
    image.loadFromFile("../resources/map.png");
    mario.position = map.CreateFromImg(image);
    mario.Begin();

}

void update(float dTime) {
    Physics::Update(dTime);
    mario.Update(dTime);
    camera.position = mario.position;


}

void Render(Renderer& renderer) {
    map.Draw(renderer);
    mario.Draw(renderer);
    //Physics::DebugDraw(renderer);
}