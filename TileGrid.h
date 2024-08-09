#include "main.h"
#include "GameSprite.h"

#ifndef Tile_H
#define Tile_H
class Tile : public GameSprite {
	bool solidity;
	bool opacity;
	bool isWinTile;
public:
	const static int SIZE = 32;

	Tile(Texture& texture, Vector2f position, bool solidity, bool opacity, bool isWinTile);
	Tile();

	bool isSolid();
	bool isOpaque();
	bool isWin();
};
#endif 

#ifndef Door_H
#define Door_H
class Door : public Tile {
public:
	bool isOpen;
	Door(Texture& texture, Vector2f position);
	Door();
};
#endif 

#ifndef TileGrid_H
#define TileGrid_H
class TileGrid : public GameSprite {
private:
	int N, M;
	vector<vector<Tile>> tileGridMatrix;
	vector<Door> doors;
public:
	TileGrid(vector<vector<Tile>> tileGridMatrix, vector<Door> doors);
	TileGrid();

	int getN();
	int getM();
	Tile& getTile(int r, int c);
	vector<Door>& getDoors();
	bool isDoor(int r, int c);
	Door& getDoor(int r, int c);
	void draw(RenderWindow& renderWindow);
	bool isSolidTile(GameSprite& gameSprite);
};
#endif

#ifndef TileGridFactory_H
#define TileGridFactory_H
class TileGridFactory {
private:
	TextureFactory& textureFactory;
public:
	TileGridFactory(TextureFactory& spriteSheetFactory);
	TileGridFactory();

	TileGrid getTileGrid(vector<vector<int>> arr);
};
#endif