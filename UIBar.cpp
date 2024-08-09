#include "uIBar.h"

UIBar::UIBar(TextureFactory& textureFactory, Vector2f position) : GameSprite(textureFactory.getTexture("UIBar"), position) {
	font.loadFromFile("font/Candal.ttf");
	connectorCntText.setFont(font);
	connectorCntText.setCharacterSize(54);
	connectorCntText.setFillColor(Color::Red);
	connectorCntText.setPosition(getPosition() + Vector2f(48, 128));

	connectorSprite = GameSprite(textureFactory.getTexture("connector"), getPosition() + Vector2f(12, 148));
	connectorSprite.setScale(2, 2);

	returnButtonSprite = GameSprite(textureFactory.getTexture("ReturnButton"), getPosition() + Vector2f(16, 16));
	returnButtonSprite.setScale(1.5, 1.5);
}

void UIBar::draw(RenderWindow& renderWindow, int connectorCnt) {
	renderWindow.draw(*this);
	connectorCntText.setString(":" + to_string(connectorCnt));
	renderWindow.draw(connectorCntText);
	renderWindow.draw(connectorSprite);
	renderWindow.draw(returnButtonSprite);
}
