#include "main.h"
#include "tileGrid.h"

#ifndef Player_H
#define Player_H
class Player : public GameSprite {
private:
	bool movingUp = false;
	bool movingDown = false;
	bool movingLeft = false;
	bool movingRight = false;
	bool running = false;

	bool collides(Tile& tile);
	void handleCollision(TileGrid& tileGrid, Vector2f& movement);
	void handleCollision(Tile& tile, Vector2f& movement);
public:
	Player(Texture& texture, Vector2f position);
	Player();

	void update(Event& event);
	void update(TileGrid& tileGrid);
	bool isWin(TileGrid& tileGrid);
};
#endif