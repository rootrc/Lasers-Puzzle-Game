#include "main.h"
#include "GameSprite.h"

#ifndef UIBar_H
#define UIBar_H
class UIBar : public GameSprite {
private:
	Font font;
	Text connectorCntText;
	GameSprite connectorSprite;
public:
	GameSprite returnButtonSprite;
	UIBar(TextureFactory& textureFactory, Vector2f position);
	void draw(RenderWindow& renderWindow, int connectorCnt);
};
#endif