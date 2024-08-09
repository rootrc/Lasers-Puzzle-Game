#include "main.h"
#include "TextureFactory.h"

#ifndef GameSprite_H
#define GameSprite_H
class GameSprite : public Sprite {
private:
	vector<IntRect> spriteRects;
	int frameCnt;
public:
	GameSprite(Texture& texture, Vector2f position);
	GameSprite(Texture& texture, int frameCnt, Vector2f position);
	GameSprite();

	IntRect& getFrameRect();
	void setFrame(int idx);
	void nextFrame();

	bool isMouseOver(Vector2f& mousePos);

	float getX();
	float getY();
	void setX(float x);
	void setY(float y);
	void movePosition(Vector2f dv);
	void moveX(float dx);
	void moveY(float dy);
	float getWidth();
	float getHeight();
};
#endif
