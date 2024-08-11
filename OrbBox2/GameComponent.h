#pragma once
#include "Vectors.h"
class GameObject;

class GameComponent {
protected:
	uint8_t id;
	GameObject* parent;
public:
};

class DisplayComponent : public GameComponent {
protected:
public:
	virtual void Draw();
};

class SimpleSpriteComponent : public DisplayComponent {
protected:
	olc::Sprite simple_sprite = olc::Sprite("../default.png");
	//default ../default.png
public:
	void Draw() override;
};