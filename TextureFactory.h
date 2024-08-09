#include "main.h"

#ifndef SpriteSheetFactory_H
#define SpriteSheetFactory_H
class TextureFactory {
private:
	unordered_map<string, Texture> memo;
public:
	Texture& getTexture(string imageName);
};
#endif