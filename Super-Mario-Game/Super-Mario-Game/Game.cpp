#include "Game.h"
#include "Resources.h"
#include <filesystem>
#include <iostream>
#include "Map.h"
#include "Mario.h"
#include "Physics.h"
#include<SFML/Audio.hpp>

#include<filesystem>
Map map(1.0f);
Camera camera(20.0f);
Mario mario ;
std::vector<Object*> objects{};

sf::Music music{};

void Begin(const sf::Window& window) {
    std::filesystem::path resourcePath = "../resources/";

    if (std::filesystem::exists(resourcePath)) {
        for (const auto& entry : std::filesystem::directory_iterator(resourcePath)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".png" || ext == ".jpg"|| ext==".gif") {

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
    std::filesystem::path soundPath = "../resources/sounds/";
    if (std::filesystem::exists(soundPath)) {
        for (const auto& entry : std::filesystem::directory_iterator(soundPath)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".ogg" || ext == ".wav" ){

                    std::string key = entry.path().filename().string();

                    if (Resources::sounds[key].loadFromFile(entry.path().string())) {
                        std::cout << "LOADED: " << key << " from " << entry.path() << std::endl;
                    }
                }
            }
        }
    }
    else {
        std::cout << std::filesystem::absolute(soundPath) << std::endl;
    }
    music.openFromFile("../resources/sounds/music.ogg");
    music.setLooping(true);
    music.setVolume(25);
    music.play();

    Physics::Init();

    sf::Image image;
    image.loadFromFile("../resources/map.png");
    mario.position = map.CreateFromImg(image,objects);
    mario.Begin();
    for (auto& object : objects) {
        object->Begin();
    }

}

void update(float dTime) {
    Physics::Update(dTime);
    mario.Update(dTime);
    camera.position = mario.position;
    for (auto& object : objects) {
        object->Update(dTime);
    }

}

void Render(Renderer& renderer) {
    renderer.Draw(Resources::textures["sky.png"], camera.position, camera.GetViewSize());
    for (auto& object : objects) {
        object->Render(renderer);
    }
    map.Draw(renderer);
    mario.Draw(renderer);
    //Physics::DebugDraw(renderer);
}