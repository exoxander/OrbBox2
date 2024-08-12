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
	virtual void on_frame() {};
	virtual void on_step() {};
	virtual void after_step() {};
};

class DisplayComponent : public GameComponent {
protected:
public:
	DisplayComponent(uint8_t _id, GameObject* _pnt) : GameComponent(_id, _pnt) {};
	virtual void draw() {};
};

class PhysicsComponent : public GameComponent {
protected:
	fvector position;
	fvector velocity;
	float mass;

public:

};

class SimpleSpriteComponent : public DisplayComponent {
protected:
	olc::Sprite simple_sprite;

	//max dimension 256x256px
	uint8_t width;
	uint8_t height;
public:
	SimpleSpriteComponent(uint8_t _id, GameObject* _pnt,uint8_t _w, uint8_t _h, const char* _img = nullptr): DisplayComponent(_id, _pnt) {
		if (_img == nullptr) {
			_img = "../default.png";
		}
		simple_sprite = olc::Sprite(_img);
		width = _w;
		height = _h;
	}
	const char* get_name() { return "simple_sprite_component"; };
	void draw() override;
	void on_frame() override { draw(); }
};

class FollowMouseComponent : public GameComponent {
public:
	FollowMouseComponent(uint8_t _id, GameObject* _pnt) : GameComponent(_id, _pnt){}

	const char* get_name() { return "follow_mouse_component"; };
	void move();
	void on_frame() override { move(); }
};

class OnClickDebugDraw : public GameComponent {
protected:
	bool is_active;
public:
	OnClickDebugDraw(uint8_t _id, GameObject* _pnt) : GameComponent(_id, _pnt) {}
	void on_frame() override;
};