#include "textureFactory.h"

Texture& TextureFactory::getTexture(string imageName) {
	if (memo.find(imageName) != memo.end()) {
		return memo[imageName];
	}
	Texture texture;
	texture.loadFromFile("assets/" + imageName + ".png");
	return memo[imageName] = texture;
}
