#include "Map.h"
#include "Resources.h"
#include "Physics.h"
#include <box2d/box2d.h>
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

sf::Vector2f Map::CreateFromImg(const sf::Image& img) {
    grid.clear();

    auto width = img.getSize().x;
    auto height = img.getSize().y;
    sf::Vector2f marioPos{};

    grid = std::vector(width, std::vector<int>(height, 0));

    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            sf::Color color = img.getPixel({ x, y });

            if (color == sf::Color::Black || color == sf::Color::Green) {
                grid[x][y] = 1;

                b2BodyDef bodyDef = b2DefaultBodyDef();
                bodyDef.position = { cellSize * x + cellSize / 2.0f, cellSize * y + cellSize / 2.0f };
                b2BodyId bodyId = b2CreateBody(Physics::world, &bodyDef);
                b2Polygon box = b2MakeBox(cellSize / 2.0f, cellSize / 2.0f);
                b2ShapeDef shapeDef = b2DefaultShapeDef();
                b2CreatePolygonShape(bodyId, &shapeDef, &box);

            }
            else if (color == sf::Color::Red) {
                marioPos = sf::Vector2f(cellSize * x + cellSize / 2.0f, cellSize * y + cellSize / 2.0f);
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