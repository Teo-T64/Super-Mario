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
Mario mario;
std::vector<Object*> objects{};
sf::Music music{};
size_t coinsInLvl{};
bool paused{};
float winTimer = -1.0f;

sf::Font font{};
sf::Text coinsTxt(font, "Score", 24);
sf::RectangleShape backgroundShape(sf::Vector2f(1.0f, 1.0f));

void Restart() {

    for (auto* obj : objects) {
        delete obj;
    }
    objects.clear();

    Physics::Cleanup();
    Physics::Init(); 
    winTimer = -1.0f;
    mario = Mario(); 
    mario.isDead = false;
    paused = false;

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
    coinsInLvl = 0;
    sf::Image image;
    if (image.loadFromFile((resourcePath / "map.png").string())) {
        mario.position = map.CreateFromImg(image, objects);
    }

    mario.Begin();
    for (auto& object : objects) {
        object->Begin();
    }
}
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

    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setOrigin(sf::Vector2f(0.5f, 0.5f));


    Physics::Init();
    sf::Image image;

    if (image.loadFromFile((resourcePath / "map.png").string())) {
        mario.position = map.CreateFromImg(image, objects);
    }
    else {
        std::cout << "ERROR: Could not load map.png from " << resourcePath << std::endl;
    }
    Restart();
}

void update(float dTime) {
    std::vector<Object*> toDeleteThisFrame;

    bool won = (mario.GetCoins() >= coinsInLvl && coinsInLvl > 0);

    if (won && winTimer < 0) {
        winTimer = 1.25f;
    }
    if (winTimer > 0) {
        winTimer -= dTime;
        if (winTimer <= 0) {
            paused = true;
            winTimer = 0;
        }
    }

    if (mario.isDead) {
        paused = true;
    }

    if (paused && (mario.isDead || won)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            paused = false;
            Restart();
            return;
        }
    }

    if (paused)
        return;

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
    //Physics::DebugDraw(renderer);
}

void RenderUI(Renderer& renderer) {
    sf::View uiView = camera.GetUIView();
    renderer.target.setView(uiView);

    sf::Vector2f uiSize = uiView.getSize();
    float halfWidth = uiSize.x / 2.0f;
    float halfHeight = uiSize.y / 2.0f;

 
    coinsTxt.setScale({ 0.1f, 0.1f });
    coinsTxt.setPosition({ -halfWidth + 2.0f, -halfHeight + 2.0f });
    coinsTxt.setString("Score: " + std::to_string(mario.GetCoins()));
    renderer.target.draw(coinsTxt);

    if (paused) {
        backgroundShape.setSize(uiSize);
        backgroundShape.setOrigin(uiSize / 2.0f); 
        backgroundShape.setPosition({ 0.0f, 0.0f });
        renderer.target.draw(backgroundShape);

        sf::Text statusTxt(font);
        statusTxt.setCharacterSize(60);
        statusTxt.setFillColor(sf::Color::White);
        statusTxt.setOutlineColor(sf::Color::Black);
        statusTxt.setOutlineThickness(2.0f);
        statusTxt.setScale({ 0.08f, 0.08f });

        bool won = (mario.GetCoins() >= coinsInLvl && coinsInLvl > 0);

        if (mario.isDead) {
            statusTxt.setString("GAME OVER\nPress SPACE to Restart");
            statusTxt.setFillColor(sf::Color::White);
        }
        else if (won) {
            statusTxt.setString("YOU WIN!\nPress SPACE to Play Again");
            statusTxt.setFillColor(sf::Color::Yellow);
        }
        else {
            statusTxt.setString("PAUSED\nPress ESC to Resume");
            statusTxt.setFillColor(sf::Color::White);
        }

        sf::FloatRect bounds = statusTxt.getLocalBounds();

        statusTxt.setOrigin({ bounds.position.x + bounds.size.x / 2.0f, bounds.position.y });


        statusTxt.setPosition({ 0.0f, -halfHeight + 2.0f });

        renderer.target.draw(statusTxt);
    }

    renderer.target.setView(camera.GetView(renderer.target.getSize()));
}