#include "main.h"
#include "main.h"
#include "tileGrid.h"
#include "laserObjects.h"

#ifndef Laser_H
#define Laser_H
class Laser : public Shape {
private:
	Vector2f p1, p2;

	Vector2f m_direction;
	float m_thickness;
public:
	static Color RED, BLUE, GREEN;
	Laser(Vector2f point1, Vector2f point2, Color& color);
	Vector2f& getP1();
	Vector2f& getP2();
	void setP2(Vector2f p);
	bool operator==(const Laser& other);

	void setThickness(float thickness);
	float getThickness();
	float getDirection();

	float getLength() const;
	size_t getPointCount() const;
	Vector2f getPoint(size_t index) const;
};
#endif

#ifndef LaserSystem_H
#define LaserSystem_H
class LaserSystem : public GameSprite {
private:
	static const int floatPrecision = 1e6;

	TextureFactory& textureFactory;
	TileGrid tileGrid;
	Vector2f size;

	vector<Laser> originalLasers;
	vector<Laser> lasers;
	vector<LaserObject> connectors;

	unordered_set<int> poweredPoints;
	vector<Laser> poweredLasers;

	static Color& getColorFromId(int colorId);
	static int hashPoint(Vector2f p);

	bool isIntersecting(Laser& laser1, Laser& laser2);
	Vector2f getIntersectionPoint(Laser& laser1, Laser& laser2);
	bool isIntersectingWithTile(Laser& laser, TileGrid& tileGrid);
	Vector2f getIntersectionWithTile(Laser& laser, TileGrid& tileGrid);


	void addLaser(Laser newLaser, TileGrid& tileGrid);
	void removePoint(Vector2f p, TileGrid& tileGrid);
	void removeLaser(Laser laser, TileGrid& tileGrid);
	void updateLasers(Laser& laser1, Laser& laser2, TileGrid& tileGrid);
	void updateIntersectionPoint(Laser& laser, Vector2f oldIntersectionPoint, Vector2f& p, TileGrid& tileGrid);
	void setPoweredLasers(LaserObjectManager& laserObjectManager);

public:
	LaserSystem(Vector2f size, TextureFactory& textureFactory, TileGrid tileGrid);
	LaserSystem();
	static float getDistance(Vector2f p1, Vector2f p2);

	void mouseAddConnectors(Vector2f& mousePos, vector<LaserObject>& connected, LaserObjectManager& laserObjectManager, TileGrid& tileGrid);
	void addConnector(Vector2f pos, LaserObject connector, vector<LaserObject>& connectedLaserObjects, TileGrid& tileGrid);
	void mouseRemoveConnectors(Vector2f& mousePos, LaserObjectManager& laserObjectManager, TileGrid& tileGrid);
	void removeConnector(int idx, TileGrid& tileGrid);

	vector<LaserObject>& getConnectors();

	void draw(RenderWindow& renderWindow, LaserObjectManager& laserObjectManager, TileGrid& tileGrid);
};
#endif