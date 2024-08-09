#include "GameSprite.h";

GameSprite::GameSprite(Texture& texture, Vector2f position) : GameSprite(texture, 1, position) {
	frameCnt = 1;
}

GameSprite::GameSprite(Texture& texture, int frameCnt, Vector2f position) : Sprite(texture) {
	this->frameCnt = frameCnt;
	setFrame(0);
	for (int i = 0; i < frameCnt; i++) {
		spriteRects.push_back(IntRect(i * texture.getSize().x / frameCnt, 0, texture.getSize().x / frameCnt, texture.getSize().y));
	}
	setPosition(position);
}

GameSprite::GameSprite() : Sprite() {
	frameCnt = 1;
}

IntRect& GameSprite::getFrameRect() {
	return spriteRects[frameCnt];
}

void GameSprite::setFrame(int idx) {
	frameCnt = idx;
}

void GameSprite::nextFrame() {
	frameCnt++;
}

bool GameSprite::isMouseOver(Vector2f& mousePos) {
	if (getX() < mousePos.x + getOrigin().x && mousePos.x + getOrigin().x < getX() + getWidth() && getY() < mousePos.y + getOrigin().y && mousePos.y + getOrigin().y < getY() + getHeight()) {
		return true;
	}
	return false;
}


float GameSprite::getX() {
	return getPosition().x;
}

float GameSprite::getY() {
	return getPosition().y;
}

void GameSprite::setX(float x) {
	setPosition(Vector2f(x, getY()));
}

void GameSprite::setY(float y) {
	setPosition(Vector2f(getX(), y));
}

void GameSprite::movePosition(Vector2f dv) {
	setPosition(getPosition() + dv);
}

void GameSprite::moveX(float dx) {
	movePosition(Vector2f(dx, 0));
}

void GameSprite::moveY(float dy) {
	movePosition(Vector2f(0, dy));
}

float GameSprite::getWidth() {
	if (frameCnt != 0) {
		return getTextureRect().width / frameCnt;
	}
	return getTextureRect().width;
}

float GameSprite::getHeight() {
	return getTextureRect().height;
}
