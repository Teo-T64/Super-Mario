#include "Map.h"
#include "Resources.h"
#include "Physics.h"
#include"Coin.h"
#include "Enemy.h"
#include <box2d/box2d.h>
#include<iostream>
Map::Map(float cellSize) : cellSize(cellSize),grid() {}


/*void Map::createCheckerBoard(size_t width, size_t height) {

	grid = std::vector(width,std::vector(height,0));

	bool last = 0;
	for (auto& col : grid) {
		for (auto& cell : col) {
			last = cell = !last;
			
		}
		if (width % 2 == 0) {
			last = !last;
		}
	
	}

}*/

sf::Vector2f Map::CreateFromImg(const sf::Image& img, std::vector<Object*>& objects) {
    grid.clear();

    auto width = img.getSize().x;
    auto height = img.getSize().y;
    sf::Vector2f marioPos{};

    grid = std::vector<std::vector<int>>(width, std::vector<int>(height, 0));

    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            sf::Color color = img.getPixel({ x, y });

            if (color == sf::Color::Black || color == sf::Color::Green) {
                grid[x][y] = 1;

                b2BodyDef bodyDef = b2DefaultBodyDef();
                bodyDef.type = b2_staticBody; 
                bodyDef.position = { cellSize * x + cellSize / 2.0f, cellSize * y + cellSize / 2.0f };

                b2BodyId bodyId = b2CreateBody(Physics::world, &bodyDef);

                b2Polygon box = b2MakeBox(cellSize / 2.0f, cellSize / 2.0f);

                b2ShapeDef shapeDef = b2DefaultShapeDef();

                FixtureData* fixtureData = new FixtureData();
                fixtureData->type = FixtureDataType::MapTile;
                fixtureData->mapX = x;
                fixtureData->mapY = y;
                fixtureData->listener = nullptr;

                shapeDef.userData = fixtureData;
                shapeDef.enableContactEvents = true;
                shapeDef.density = 0.0f; 
                shapeDef.material.friction = 0.0f;

                b2CreatePolygonShape(bodyId, &shapeDef, &box);
            }
            else if (color == sf::Color::Red) {
                marioPos = sf::Vector2f(cellSize * x + cellSize / 2.0f, cellSize * y + cellSize / 2.0f);
            }
            else if (color == sf::Color::Yellow) {

                sf::Vector2f pos(cellSize * x + cellSize / 2.0f, cellSize * y + cellSize / 2.0f);
                Coin* coin = new Coin(pos); 
                objects.push_back(coin);

            }
            else if (color == sf::Color::Blue) {
                sf::Vector2f enemyPos(cellSize * x + cellSize / 2.0f,
                    cellSize * y + cellSize / 2.0f);

                Enemy* enemy = new Enemy(enemyPos);
                enemy->position = enemyPos; 
                objects.push_back(enemy);

                std::cout << "Created enemy at: (" << enemyPos.x << ", " << enemyPos.y << ")" << std::endl;
            }
        }
    }
    return marioPos;
}

void Map::Draw(Renderer& renderer) {

	int x = 0;
	for (const auto& col : grid) {
		int y = 0;
		for (const auto& cell : col) {
			if (cell) {
				renderer.Draw(Resources::textures["block.png"],
					sf::Vector2f(cellSize*x+cellSize/2.0f,cellSize*y+cellSize/2.0f),
					sf::Vector2f(cellSize,cellSize)
									
				);
			
			}
		

			y++;
		}
		x++;
	}

}