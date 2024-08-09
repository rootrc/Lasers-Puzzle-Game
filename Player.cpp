#include "Player.h"

Player::Player(Texture& texture, Vector2f position) : GameSprite(texture, position) {}

Player::Player() {}

bool Player::collides(Tile& tile) {
	if (!tile.isSolid()) {
		return false;
	}
	return getX() < tile.getX() + tile.getWidth() && getX() + getWidth()> tile.getX() &&  getY() < tile.getY() + tile.getHeight() && getY() + getHeight() > tile.getY();
}

void Player::handleCollision(TileGrid& tileGrid, Vector2f& movement) {
	Vector2f minP = getPosition() - Vector2f((1664 - tileGrid.getM() * Tile::SIZE) / 2 + 128, (1080 - tileGrid.getN() * Tile::SIZE) / 2);
	Vector2f maxP = getPosition() + Vector2f(Tile::SIZE - 1, Tile::SIZE - 1) - Vector2f((1664 - tileGrid.getM() * Tile::SIZE) / 2 + 128, (1080 - tileGrid.getN() * Tile::SIZE) / 2);
	int rows[2] = {minP.y / Tile::SIZE, maxP.y / Tile::SIZE};
	int cols[2] = {minP.x / Tile::SIZE, maxP.x / Tile::SIZE};
	for (int& r: rows) {
		for (int& c: cols) {
			if (collides(tileGrid.getTile(r, c))) {
				handleCollision(tileGrid.getTile(r, c), movement);
				//return;
			}
		}
	}
}

void Player::handleCollision(Tile& tile, Vector2f& movement) {
	if (getX() < tile.getX() && getX() + getWidth() - tile.getX() <= movement.x && (movement.x > 0)) {
		setX(tile.getX() - getWidth());
		movement.x = 0;
	}
	if (getX() - tile.getX() - tile.getWidth() >= movement.x && tile.getX() + tile.getWidth() < getX() + getWidth() && (movement.x < 0)) {
		setX(tile.getX() + tile.getWidth());
		movement.x = 0;
	}
	if (getY() < tile.getY() && getY() + getHeight() - tile.getY() <= movement.y && (movement.y > 0)) {
		setY(tile.getY() - getHeight());
		movement.y = 0;
	}
	if (getY() - tile.getY() - tile.getHeight() >= movement.y && tile.getY() + tile.getHeight() < getY() + getHeight() && (movement.y < 0)) {
		setY(tile.getY() + tile.getHeight());
		movement.y = 0;
	}
}


void Player::update(Event& event) {
	if (event.type == Event::KeyPressed) {
		if (event.key.code == Keyboard::W) {
			movingUp = true;
		}
		if (event.key.code == Keyboard::S) {
			movingDown = true;
		}
		if (event.key.code == Keyboard::A) {
			movingLeft = true;
		}
		if (event.key.code == Keyboard::D) {
			movingRight = true;
		}
		if (event.key.code == Keyboard::LShift || event.key.code == Keyboard::RShift) {
			running = true;
		}
	}
	else if (event.type == Event::KeyReleased) {
		if (event.key.code == Keyboard::W) {
			movingUp = false;
		}
		if (event.key.code == Keyboard::S) {
			movingDown = false;
		}
		if (event.key.code == Keyboard::A) {
			movingLeft = false;
		}
		if (event.key.code == Keyboard::D) {
			movingRight = false;
		}
		if (event.key.code == Keyboard::LShift || event.key.code == Keyboard::RShift) {
			running = false;
		}
	}
}

void Player::update(TileGrid& tileGrid) {
	Vector2f movement(0, 0);
	if (movingUp) {
		movement.y -= 0.5;
	}
	if (movingDown) {
		movement.y += 0.5;
	}
	if (movingRight) {
		movement.x += 0.5;
	}
	if (movingLeft) {
		movement.x -= 0.5;
	}
	if (running) {
		movement.x *= 2;
		movement.y *= 2;
	}
	if (movement.x && movement.y) {
		movement.x *= sqrt(0.5);
		movement.y *= sqrt(0.5);
	}
	movePosition(movement);
	handleCollision(tileGrid, movement);
}

bool Player::isWin(TileGrid& tileGrid) {
	Vector2f minP = getPosition() - Vector2f((1664 - tileGrid.getM() * Tile::SIZE) / 2 + 128, (1080 - tileGrid.getN() * Tile::SIZE) / 2);
	Vector2f maxP = getPosition() + Vector2f(Tile::SIZE - 1, Tile::SIZE - 1) - Vector2f((1664 - tileGrid.getM() * Tile::SIZE) / 2 + 128, (1080 - tileGrid.getN() * Tile::SIZE) / 2);
	int rows[2] = {minP.y / Tile::SIZE, maxP.y / Tile::SIZE};
	int cols[2] = {minP.x / Tile::SIZE, maxP.x / Tile::SIZE};
	for (int& r: rows) {
		for (int& c: cols) {
			if (tileGrid.getTile(r, c).isWin()) {
				return true;
			}
		}
	}
	return false;
}