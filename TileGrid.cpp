#include "tileGrid.h"

Tile::Tile(Texture& texture, Vector2f position, bool solidity, bool opacity, bool isWinTile) : GameSprite(texture, position), solidity(solidity), opacity(opacity), isWinTile(isWinTile) {}

Tile::Tile() : GameSprite() {}

bool Tile::isSolid() {
	return solidity;
}

bool Tile::isOpaque() {
	return opacity;
}

bool Tile::isWin() {
	return isWinTile;
}

Door::Door(Texture& texture, Vector2f position) : Tile(texture, position, true, true, false), isOpen(false) {}
Door::Door() {};

TileGrid::TileGrid(vector<vector<Tile>> tileGridMatrix, vector<Door> doors) : GameSprite(), tileGridMatrix(tileGridMatrix), doors(doors) {
	N = tileGridMatrix.size();
	M = tileGridMatrix[0].size();
	setPosition(Vector2f((1664 - M * Tile::SIZE) / 2 + 128, (1080 - N * Tile::SIZE) / 2));
	setTextureRect(IntRect(0, 0, M * Tile::SIZE, N * Tile::SIZE));
}

TileGrid::TileGrid() : GameSprite() {}

int TileGrid::getN() {
	return N;
}

int TileGrid::getM() {
	return M;
}


Tile& TileGrid::getTile(int r, int c) {
	for (Door& door : doors) {
		if (door.getPosition() == Vector2f(c * Tile::SIZE, r * Tile::SIZE) + Vector2f((1664 - M * Tile::SIZE) / 2 + 128, (1080 - N * Tile::SIZE) / 2) && !door.isOpen) {
			return door;
		}
	}
	return tileGridMatrix[r][c];
}

bool TileGrid::isDoor(int r, int c) {
	for (Door& door : doors) {
		if (door.getPosition() == Vector2f(c * Tile::SIZE, r * Tile::SIZE) + Vector2f((1664 - M * Tile::SIZE) / 2 + 128, (1080 - N * Tile::SIZE) / 2)) {
			return true;
		}
	}
	return false;
}


Door& TileGrid::getDoor(int r, int c) {
	for (Door& door : doors) {
		if (door.getPosition() == Vector2f(c * Tile::SIZE, r * Tile::SIZE) + Vector2f((1664 - M * Tile::SIZE) / 2 + 128, (1080 - N * Tile::SIZE) / 2)) {
			return door;
		}
	}
}

vector<Door>& TileGrid::getDoors() {
	return doors;
}

void TileGrid::draw(RenderWindow& renderWindow) {
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < M; c++) {
			renderWindow.draw(getTile(r, c));
		}
	}
}

bool TileGrid::isSolidTile(GameSprite& gameSprite) {
	int minRow = (gameSprite.getY() - gameSprite.getOrigin().y - getY()) / Tile::SIZE;
	int minCol = (gameSprite.getX() - gameSprite.getOrigin().x - getX()) / Tile::SIZE;
	int maxRow = (gameSprite.getY() - gameSprite.getOrigin().y + gameSprite.getHeight() - getY() - 1) / Tile::SIZE;
	int maxCol = (gameSprite.getX() - gameSprite.getOrigin().x + gameSprite.getWidth() - getX() - 1) / Tile::SIZE;
	return getTile(minRow, minCol).isSolid() || getTile(maxRow, minCol).isSolid() || getTile(minRow, maxCol).isSolid() || getTile(maxRow, maxCol).isSolid();
}

TileGridFactory::TileGridFactory(TextureFactory& textureFactory) : textureFactory(textureFactory) {}

TileGridFactory::TileGridFactory() : textureFactory(textureFactory) {}

TileGrid TileGridFactory::getTileGrid(vector<vector<int>> arr) {
	int N = arr.size();
	int M = arr[0].size();
	vector<vector<Tile>> tileGridMatrix(N, vector<Tile>(M));
	vector<Door> doors;
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < M; c++) {
			// TEMP
			if (arr[r][c] == 0 || arr[r][c] == 5) {
				tileGridMatrix[r][c] = Tile(textureFactory.getTexture("FloorTile"), Vector2f(c * Tile::SIZE, r * Tile::SIZE), false, false, false);
				if (arr[r][c] == 5) {
					doors.push_back(Door(textureFactory.getTexture("DoorTile"), Vector2f(c * Tile::SIZE, r * Tile::SIZE)));
					doors.back().movePosition(Vector2f((1664 - M * Tile::SIZE) / 2 + 128, (1080 - N * Tile::SIZE) / 2));
				}
			}
			else if (arr[r][c] == 1) {
				tileGridMatrix[r][c] = Tile(textureFactory.getTexture("WallTile"), Vector2f(c * Tile::SIZE, r * Tile::SIZE), true, true, false);
			}
			else if (arr[r][c] == 2) {
				tileGridMatrix[r][c] = Tile(textureFactory.getTexture("GlassTile"), Vector2f(c * Tile::SIZE, r * Tile::SIZE), true, false, false);
			}
			else if (arr[r][c] == 9) {
				tileGridMatrix[r][c] = Tile(textureFactory.getTexture("WinTile"), Vector2f(c * Tile::SIZE, r * Tile::SIZE), false, false, true);
			}
			tileGridMatrix[r][c].movePosition(Vector2f((1664 - M * Tile::SIZE) / 2 + 128, (1080 - N * Tile::SIZE) / 2));
		}
	}
	return TileGrid(tileGridMatrix, doors);
}