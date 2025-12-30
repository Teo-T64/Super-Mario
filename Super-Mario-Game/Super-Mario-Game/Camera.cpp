#include "Camera.h"


Camera::Camera(float zoomLvl) :zoomLvl(zoomLvl) {}

sf::View Camera::GetView(sf::Vector2u windowSize) {
	float aspect = (float)windowSize.x / (float)windowSize.y;
	sf::Vector2f size;
	if (aspect < 1.0f) {
		size = sf::Vector2f(zoomLvl, zoomLvl / aspect);
	}else size = sf::Vector2f(zoomLvl * aspect, zoomLvl);

	return sf::View(position, size);


}