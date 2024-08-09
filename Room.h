#include "main.h"
#include "tileGrid.h"
#include "laser.h"
#include "laserObjects.h"
#include "player.h"

#ifndef Room_H
#define Room_H
class Room : public GameSprite {
private:
	int totalConnectors;
	TileGrid tileGrid;
	LaserObjectManager laserObjectManager;
	LaserSystem laserSystem;
	Player player;
public:
	Room(Vector2f size, Player player, TileGrid tileGrid, LaserSystem laserSystem, vector<vector<LaserObject>> laserObjects);
	Room();
	void draw(RenderWindow& renderWindow);

	TileGrid& getTileGrid();
	LaserObjectManager& getLaserObjectManager();
	LaserSystem& getLaserSystem();
	Player& getPlayer();

	int getAvailableConnectors();
};
#endif

#ifndef RoomFactory_H
#define RoomFactory_H
class RoomFactory {
private:
	TextureFactory& textureFactory;
	TileGridFactory tileGridFactory;
	LaserObjectFactory laserObjectFactory;

	TileGrid tileGrid;
	LaserObjectManager laserObjectManager;
	LaserSystem laserSystem;
public:
	RoomFactory(TextureFactory& textureFactory);
	Room getRoom(int roomId);
};
#endif