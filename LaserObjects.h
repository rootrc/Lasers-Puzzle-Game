#include "main.h"
#include "textureFactory.h"
#include "tileGrid.h"

#ifndef LaserObject_H
#define LaserObject_H
class LaserObject : public GameSprite {
	bool isSource;
	int colorId;
public:
	LaserObject(Texture& texture, Vector2f position, bool isSource, int colorId);
	LaserObject();
	bool& isASource();
	int& getColorId();
	void setColorId(int i);
};
#endif

#ifndef Generator_H
#define Generator_H

class Generator: public LaserObject {
private:
public:
	Generator(Texture& texture, Vector2f position, int direction, int color);
};
#endif

//#ifndef Door_H
//#define Door_H
//
//class Door : public LaserObject {
//private:
//public:
//	Door(Texture& texture, int r, int c);
//};
//#endif

#ifndef Receiver_H
#define Receiver_H

class Receiver : public LaserObject {
private:
public:
	Receiver(Texture& texture, Vector2f position, int direction, int color);
};
#endif

#ifndef LaserObjectManager_H
#define LaserObjectManager_H
class LaserObjectManager : public GameSprite {
private:
	Vector2f size;
	vector<vector<LaserObject>> laserObjects;
	vector<LaserObject> sourceLaserObjects;
public:
	LaserObjectManager(Vector2f size, vector<vector<LaserObject>> laserObjects);
	LaserObjectManager();

	void addLaserObject(int r, int c, LaserObject laserObject);

	vector<LaserObject> getSourceLaserObjects();

	bool containLaserObject(int r, int c);
	LaserObject& getLaserObject(int r, int c);

	void draw(RenderWindow& renderWindow);
};
#endif 

#ifndef LaserObjectFactory_H
#define LaserObjectFactory_H
class LaserObjectFactory {
private:
	TextureFactory& textureFactory;
public:
	LaserObjectFactory(TextureFactory& textureFactory);

	Generator getGenerator(int r, int c, int direction, int color);
	Receiver getReceiver(int r, int c, int direction, int color);
};
#endif 