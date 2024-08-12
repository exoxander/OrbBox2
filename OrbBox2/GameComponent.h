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

struct Animation {
	bool loop;
	int frames;
	std::vector<olc::Sprite> sprite_set;
};
class AnimatedComponent : public DisplayComponent {
protected:
public:
	const char* get_name() { return "animated_component"; }
};

class PhysicsComponent : public GameComponent {
protected:
	fvector position;
	fvector velocity;
	float mass;
public:
	const char* get_name() { return "physics_component"; }

	//register physics object with the game manager
	void register_component();
};

class ColliderComponent : public GameComponent {
protected:
public:
	const char* get_name() { return "collider_component"; }

	//register physics object with the game manager
	void register_component();
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
	void on_frame() override;
};

class OnMouseDownDebugDraw : public GameComponent {
protected:
	bool is_active;
public:
	OnMouseDownDebugDraw(uint8_t _id, GameObject* _pnt) : GameComponent(_id, _pnt) {}
	void on_frame() override;
};