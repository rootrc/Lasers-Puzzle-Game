#include "LaserObjects.h"

LaserObject::LaserObject(Texture& texture, Vector2f position, bool isSource, int colorId) : GameSprite(texture, position), isSource(isSource), colorId(colorId) {}

LaserObject::LaserObject() : GameSprite(), isSource(0) {}

bool& LaserObject::isASource() {
	return isSource;
}

int& LaserObject::getColorId() {
	return colorId;
}

void LaserObject::setColorId(int i) {
	colorId = i;
}

Generator::Generator(Texture& texture, Vector2f position, int direction, int colorId) : LaserObject(texture, position, true, colorId) {
	switch (direction) {
	case 0:
		movePosition(Vector2f(16, 13));
		setOrigin(16, 15);
		break;
	case 1:
		movePosition(Vector2f(19, 16));
		setOrigin(17, 16);
		break;
	case 2:
		movePosition(Vector2f(16, 19));
		setOrigin(16, 17);
		break;
	case 3:
		movePosition(Vector2f(13, 16));
		setOrigin(15, 16);
		break;
	}
}

Receiver::Receiver(Texture& texture, Vector2f position, int direction, int colorId) : LaserObject(texture, position, false, colorId) {
	switch (direction) {
	case 0:
		movePosition(Vector2f(16, 3));
		setOrigin(16, 11);
		break;
	case 1:
		movePosition(Vector2f(29, 16));
		setOrigin(21, 16);
		break;
	case 2:
		movePosition(Vector2f(16, 29));
		setOrigin(16, 21);
		break;
	case 3:
		movePosition(Vector2f(3, 16));
		setOrigin(11, 16);
		break;
	}
}

LaserObjectManager::LaserObjectManager(Vector2f size, vector<vector<LaserObject>> laserObjects) : GameSprite(), size(size), laserObjects(laserObjects) {
	for (int r = 0; r < this->laserObjects.size(); r++) {
		for (int c = 0; c < this->laserObjects[r].size(); c++) {
			this->laserObjects[r][c].movePosition(Vector2f((1664 - size.x) / 2 + 128, (1080 - size.y) / 2));
			if (this->laserObjects[r][c].isASource()) {
				sourceLaserObjects.push_back(this->laserObjects[r][c]);
			}
		}
	}
}

LaserObjectManager::LaserObjectManager() : GameSprite() {
	setPosition(Vector2f((1664 - size.x) / 2 + 128, (1080 - size.y) / 2));
	setTextureRect(IntRect(0, 0, size.x, size.y));
}

void LaserObjectManager::addLaserObject(int r, int c, LaserObject laserObject) {
	laserObjects[r][c] = laserObject;
	laserObjects[r][c].movePosition(Vector2f((1664 - size.x) / 2 + 128, (1080 - size.y) / 2));
}

vector<LaserObject> LaserObjectManager::getSourceLaserObjects() {
	return sourceLaserObjects;
}

bool LaserObjectManager::containLaserObject(int r, int c) {
	return laserObjects[r][c].getOrigin() != Vector2f(0, 0);
}

LaserObject& LaserObjectManager::getLaserObject(int r, int c) {
	return laserObjects[r][c];
}

void LaserObjectManager::draw(RenderWindow& renderWindow) {
	for (int r = 0; r < laserObjects.size(); r++) {
		for (int c = 0; c < laserObjects[r].size(); c++) {
			renderWindow.draw(laserObjects[r][c]);
		}
	}
}

LaserObjectFactory::LaserObjectFactory(TextureFactory& textureFactory) : textureFactory(textureFactory) {}

Generator LaserObjectFactory::getGenerator(int r, int c, int direction, int color) {
	return Generator(textureFactory.getTexture("generator" + to_string(color) + to_string(direction)), Vector2f(c * Tile::SIZE, r * Tile::SIZE), direction, color);
}

Receiver LaserObjectFactory::getReceiver(int r, int c, int direction, int color) {
	return Receiver(textureFactory.getTexture("receiver" + to_string(color) + to_string(direction)), Vector2f(c * Tile::SIZE, r * Tile::SIZE), direction, color);
}
