#include "main.h"
#include "uiBar.h"
#include "tileGrid.h"
#include "room.h"
#include "laser.h"


RenderWindow window(VideoMode(1920, 1080), "Puzzle Game");
//RenderWindow window(VideoMode(1920, 1080), "Puzzle Game", Style::Fullscreen);



//RoomStuff
TextureFactory textureFactory;
UIBar uiBar(textureFactory, Vector2f(1792, 0));
GameSprite MenuBar(textureFactory.getTexture("MenuBar"), Vector2f(0, 0));
RoomFactory roomFactory(textureFactory);
vector<Room> rooms(1);
int levelId;

const int levelCnt = 12;

vector<pair<GameSprite, LaserObject>> clickedLaserObjects;
//MenuStuff

vector<GameSprite> levelButtons(1);
Font levelNumFont;
Text levelText[13];

string gameState = "menu";

void runGame(Event& event, Vector2f& mousePos) {
	Room& room = rooms[levelId];
	int N = room.getTileGrid().getN();
	int M = room.getTileGrid().getM();

	room.getPlayer().update(event);
	if (room.getPlayer().isWin(room.getTileGrid())) {
		gameState = "menu";
		return;
	}
	if (event.type == Event::MouseButtonPressed) {
		if (event.mouseButton.button == Mouse::Left) {
			if (uiBar.returnButtonSprite.isMouseOver(mousePos)) {
				gameState = "menu";
				return;
			}
			for (int r = 1; r < N - 1; r++) {
				for (int c = 1; c < M - 1; c++) {
					if (!room.getLaserObjectManager().getLaserObject(r, c).isMouseOver(mousePos)) {
						continue;
					}
					bool flag1 = true;
					for (int i = 0; i < clickedLaserObjects.size(); i++) {
						if (clickedLaserObjects[i].first.getPosition() == room.getLaserObjectManager().getLaserObject(r, c).getPosition() - room.getLaserObjectManager().getLaserObject(r, c).getOrigin()) {
							clickedLaserObjects.erase(clickedLaserObjects.begin() + i);
							flag1 = false;
							break;
						}
					}
					if (flag1) {
						clickedLaserObjects.push_back({ GameSprite(textureFactory.getTexture("highlightedTile"), room.getLaserObjectManager().getLaserObject(r, c).getPosition() - room.getLaserObjectManager().getLaserObject(r, c).getOrigin()), room.getLaserObjectManager().getLaserObject(r, c) });
					}
					return;
				}
			}
			for (LaserObject connector : room.getLaserSystem().getConnectors()) {
				if (connector.isMouseOver(mousePos)) {
					clickedLaserObjects.push_back({ GameSprite(textureFactory.getTexture("highlightedTile"), connector.getPosition()), connector });
					clickedLaserObjects.back().first.setScale(0.75, 0.75);
					clickedLaserObjects.back().first.setOrigin(16, 16);
					return;
				}
			}
			if (LaserSystem::getDistance(room.getPlayer().getPosition(), mousePos) < Tile::SIZE && room.getAvailableConnectors()) {
				vector<LaserObject> connectedLaserObjects;
				for (pair<GameSprite, LaserObject>& clickedLaserObject : clickedLaserObjects) {
					connectedLaserObjects.push_back(clickedLaserObject.second);
				}
				room.getLaserSystem().mouseAddConnectors(mousePos, connectedLaserObjects, room.getLaserObjectManager(), room.getTileGrid());
				clickedLaserObjects.clear();
			}
		}
		else if (event.mouseButton.button == Mouse::Right) {
			if (LaserSystem::getDistance(room.getPlayer().getPosition(), mousePos) < Tile::SIZE) {
				room.getLaserSystem().mouseRemoveConnectors(mousePos, room.getLaserObjectManager(), room.getTileGrid());
				clickedLaserObjects.clear();
			}
			else {
				clickedLaserObjects.clear();
			}
		}
	}
}

void drawGame(Event& event, Vector2f& mousePos) {
	Room& room = rooms[levelId];

	bool flag = false;

	room.getPlayer().update(room.getTileGrid());

	if (event.type != Event::MouseButtonPressed && clickedLaserObjects.size() && LaserSystem::getDistance(room.getPlayer().getPosition(), mousePos) < Tile::SIZE && room.getAvailableConnectors()) {
		vector<LaserObject> connectedLaserObjects;
		for (pair<GameSprite, LaserObject>& clickedLaserObject : clickedLaserObjects) {
			connectedLaserObjects.push_back(clickedLaserObject.second);
		}
		int temp = room.getLaserSystem().getConnectors().size();
		room.getLaserSystem().mouseAddConnectors(mousePos, connectedLaserObjects, room.getLaserObjectManager(), room.getTileGrid());
		if (temp != room.getLaserSystem().getConnectors().size()) {
			flag = true;
		}
	}

	window.draw(MenuBar);
	uiBar.draw(window, room.getAvailableConnectors());
	room.draw(window);

	for (pair<GameSprite, LaserObject>& connectedLaserObject : clickedLaserObjects) {
		window.draw(connectedLaserObject.first);
	}

	if (flag) {
		room.getLaserSystem().mouseRemoveConnectors(mousePos, room.getLaserObjectManager(), room.getTileGrid());
	}
}

void runMenu(Event& event, Vector2f& mousePos) {
	if (event.type == Event::MouseButtonPressed) {
		if (event.mouseButton.button == Mouse::Left) {
			for (int i = 1; i <= levelCnt; i++) {
				if (levelButtons[i].isMouseOver(mousePos)) {
					levelId = i;
					gameState = "game";
					return;
				}
			}
		}
	}
}

void drawMenu() {
	for (int i = 1; i <= levelCnt; i++) {
		window.draw(levelButtons[i]);
		window.draw(levelText[i]);
	}
}

int main() {
	levelNumFont.loadFromFile("font/Candal.ttf");
	for (int i = 0; i <= 1; i++) {
		for (int j = 1; j <= levelCnt / 2; j++) {
			rooms.push_back(roomFactory.getRoom(i * levelCnt / 2 + j));
			levelButtons.push_back(GameSprite(textureFactory.getTexture("levelButton"), Vector2f(256 * j, 256 + 256 * i)));
			Text& text = levelText[i * levelCnt / 2 + j];
			text.setString(to_string(i * levelCnt / 2 + j));
			text.setFont(levelNumFont);
			text.setCharacterSize(72);
			text.setFillColor(Color::Black);
			text.setPosition(Vector2f(256 * j + (128 - text.getGlobalBounds().width - 18) / 2, 256 + 256 * i + (128 - text.getGlobalBounds().height - 34) / 2));
		}
	}

	while (window.isOpen()) {
		//this_thread::sleep_for(10ms);	
		Event event;
		Vector2f mousePos(Mouse::getPosition(window));
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			if (gameState == "menu") {
				runMenu(event, mousePos);
			}
			else if (gameState == "game") {
				runGame(event, mousePos);
			}

		}
		if (gameState == "menu") {
			window.clear(Color::White);
			drawMenu();
		}
		else if (gameState == "game") {
			window.clear();
			drawGame(event, mousePos);
		}

		window.display();
	}

}