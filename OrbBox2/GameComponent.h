#pragma once
#include "Vectors.h"
class GameObject;

class GameComponent {
protected:
	uint8_t id;
	GameObject* parent;
public:
	GameComponent(uint8_t _id, GameObject* _pnt) {
		id = _id;
		parent = _pnt;
	}
	
	virtual const char* get_name() { return "default_component"; };
	virtual void run() {};
};

class DisplayComponent : public GameComponent {
protected:
public:
	DisplayComponent(uint8_t _id, GameObject* _pnt) : GameComponent(_id, _pnt) {};
	virtual void draw() {};
};

class SimpleSpriteComponent : public DisplayComponent {
protected:
	olc::Sprite simple_sprite;
public:
	SimpleSpriteComponent(uint8_t _id, GameObject* _pnt, const char* _img = nullptr): DisplayComponent(_id, _pnt) {
		if (_img == nullptr) {
			_img = "../default.png";
		}
		simple_sprite = olc::Sprite(_img);
	}
	const char* get_name() { return "simple_sprite_component"; };
	void draw() override;
	void run() override { draw(); }
};

class FollowMouseComponent : public GameComponent {
public:
	FollowMouseComponent(uint8_t _id, GameObject* _pnt) : GameComponent(_id, _pnt){}

	const char* get_name() { return "follow_mouse_component"; };
	void move();
	void run() override { move(); }
};