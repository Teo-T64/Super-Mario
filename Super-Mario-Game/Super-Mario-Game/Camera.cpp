#include "Camera.h"


Camera::Camera(float zoomLvl) :zoomLvl(zoomLvl) {}
sf::Vector2f Camera::GetViewSize()
{
	return viewSize;
}
sf::View Camera::GetView(sf::Vector2u windowSize) {
	float aspect = (float)windowSize.x / (float)windowSize.y;
	sf::Vector2f size;
	if (aspect < 1.0f) {
		viewSize = sf::Vector2f(zoomLvl, zoomLvl / aspect);
	}else viewSize = sf::Vector2f(zoomLvl * aspect, zoomLvl);

	return sf::View(position, viewSize);


}