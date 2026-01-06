#include "Game.h"
#include "Resources.h"
#include <filesystem>
#include <iostream>
#include "Map.h"
#include "Mario.h"
#include "Physics.h"
#include<SFML/Audio.hpp>

Map map(1.0f);
Camera camera(20.0f);
Mario mario ;
std::vector<Object*> objects{};
sf::Music music{};

sf::Font font{};
sf::Text coinsTxt(font, "Score", 24);
void Begin(const sf::Window& window) {

    std::filesystem::path resourcePath = "resources/";

    int depthLimit = 3;
    while (!std::filesystem::exists(resourcePath) && depthLimit > 0) {
        resourcePath = ".." / resourcePath;
        depthLimit--;
    }


    if (!std::filesystem::exists(resourcePath)) {
        std::cout << "FATAL ERROR: Could not find resources folder. Searched up to: "
            << std::filesystem::absolute(resourcePath) << std::endl;
        return;
    }


    for (const auto& entry : std::filesystem::directory_iterator(resourcePath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".png" || ext == ".jpg" || ext == ".gif") {
                std::string key = entry.path().filename().string();
                if (Resources::textures[key].loadFromFile(entry.path().string())) {
                    std::cout << "LOADED: " << key << std::endl;
                }
            }
        }
    }

    std::filesystem::path soundPath = resourcePath / "sounds/";
    if (std::filesystem::exists(soundPath)) {
        for (const auto& entry : std::filesystem::directory_iterator(soundPath)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".ogg" || ext == ".wav") {
                    std::string key = entry.path().filename().string();
                    if (Resources::sounds[key].loadFromFile(entry.path().string())) {
                        std::cout << "LOADED SOUND: " << key << std::endl;
                    }
                }
            }
        }
    }


    music.openFromFile((soundPath / "music.ogg").string());
    music.setLooping(true);
    music.setVolume(25);
    music.play();

    font.openFromFile((resourcePath/"font.ttf").string());
    coinsTxt.setFillColor(sf::Color::White);
    coinsTxt.setOutlineColor(sf::Color::Black);
    coinsTxt.setOutlineThickness(1.0f);
    coinsTxt.setScale(sf::Vector2f(0.1f,0.1f));

    Physics::Init();

    sf::Image image;
    if (image.loadFromFile((resourcePath / "map.png").string())) {
        mario.position = map.CreateFromImg(image, objects);
    }
    else {
        std::cout << "ERROR: Could not load map.png from " << resourcePath << std::endl;
    }

    mario.Begin();
    for (auto& object : objects) {
        object->Begin();
    }
}

void update(float dTime) {
    std::vector<Object*> toDeleteThisFrame;

    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->toDestroy) {
            toDeleteThisFrame.push_back(objects[i]); 
            objects.erase(objects.begin() + i);
            i--;
        }
    }

    Physics::Update(dTime);
    mario.Update(dTime);
    camera.position = mario.position;

    for (auto& object : objects) {
        object->Update(dTime);
    }

    for (Object* obj : toDeleteThisFrame) {
        delete obj;
    }
}
void Render(Renderer& renderer) {
    renderer.Draw(Resources::textures["sky.png"], camera.position, camera.GetViewSize());
    for (auto& object : objects) {
        object->Render(renderer);
    }
    map.Draw(renderer);
    mario.Draw(renderer);
    Physics::DebugDraw(renderer);
}

void RenderUI(Renderer& renderer) {
    coinsTxt.setPosition(-camera.GetViewSize() / 2.0f + sf::Vector2f(2.0f, 1.0f));
    coinsTxt.setString("Score: "+ std::to_string(mario.GetCoins()));
    renderer.target.draw(coinsTxt);
}