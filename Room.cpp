#include "room.h"

Room::Room(Vector2f size, Player player, TileGrid tileGrid, LaserSystem laserSystem, vector<vector<LaserObject>> laserObjects) : player(player), tileGrid(tileGrid), laserSystem(laserSystem), laserObjectManager(LaserObjectManager(size, laserObjects)) {
	totalConnectors = laserSystem.getConnectors().size();
	setPosition(Vector2f((1664 - size.x) / 2 + 128, (1080 - size.y) / 2));
	setTextureRect(IntRect(0, 0, size.x, size.y));
}

Room::Room() : tileGrid(tileGrid), laserSystem(laserSystem), laserObjectManager(laserObjectManager) {}

void Room::draw(RenderWindow& renderWindow) {
	tileGrid.draw(renderWindow);
	laserSystem.draw(renderWindow, laserObjectManager, tileGrid);
	laserObjectManager.draw(renderWindow);
	CircleShape circle;
	circle.setRadius(Tile::SIZE);
	circle.setOrigin(24, 24);
	circle.setPosition(player.getPosition());
	circle.setFillColor(Color(96, 96, 96, 96));
	renderWindow.draw(circle);
	renderWindow.draw(player);
}

TileGrid& Room::getTileGrid() {
	return tileGrid;
}

LaserObjectManager& Room::getLaserObjectManager() {
	return laserObjectManager;
}

LaserSystem& Room::getLaserSystem() {
	return laserSystem;
}

Player& Room::getPlayer() {
	return player;
}

int Room::getAvailableConnectors() {
	return totalConnectors - laserSystem.getConnectors().size();
}

RoomFactory::RoomFactory(TextureFactory& textureFactory) : textureFactory(textureFactory), tileGridFactory(TileGridFactory(textureFactory)), laserObjectFactory(LaserObjectFactory(textureFactory)) {}

Room RoomFactory::getRoom(int levelId) {
	fstream fileStream("Levels/Level" + to_string(levelId) + ".txt", ios::in);
	int N, M;
	fileStream >> N >> M;
	Vector2f size(M * Tile::SIZE, N * Tile::SIZE);
	vector<vector<int>> tileGridMatrix(N, vector<int>(M));
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < M; c++) {
			fileStream >> tileGridMatrix[r][c];
		}
	}
	TileGrid tileGrid = tileGridFactory.getTileGrid(tileGridMatrix);
	vector<vector<LaserObject>> laserObjects(N, vector<LaserObject>(M));

	int playerR, playerC;
	fileStream >> playerR >> playerC;

	LaserSystem laserSystem(size, textureFactory, tileGrid);
	int K;
	fileStream >> K;
	int r, c;
	LaserObject connector = LaserObject(textureFactory.getTexture("connector"), Vector2f(0, 0), true, 0);
	for (int i = 0; i < K; i++) {
		fileStream >> r >> c;
		connector.setPosition(Vector2f(c * Tile::SIZE + Tile::SIZE / 2, r * Tile::SIZE + Tile::SIZE / 2) + Vector2f((1664 - size.x) / 2 + 128, (1080 - size.y) / 2));
		connector.setOrigin(8, 8);
		laserSystem.getConnectors().push_back(connector);
	}
	fileStream >> K;
	int direction, color;
	for (int i = 0; i < K; i++) {
		fileStream >> r >> c >> direction >> color;
		laserObjects[r][c] = laserObjectFactory.getGenerator(r, c, direction, color);
	}

	fileStream >> K;
	for (int i = 0; i < K; i++) {
		fileStream >> r >> c >> direction >> color;
		laserObjects[r][c] = laserObjectFactory.getReceiver(r, c, direction, color);
	}

	fileStream.close();
	return Room(size, Player(textureFactory.getTexture("player"), Vector2f(playerC * Tile::SIZE + 8, playerR * Tile::SIZE + 8) + Vector2f((1664 - size.x) / 2 + 128, (1080 - size.y) / 2)), tileGrid, laserSystem, laserObjects);
}