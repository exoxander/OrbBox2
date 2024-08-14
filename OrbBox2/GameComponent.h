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
	GameObject* get_parent() { return parent; };
	virtual const char* get_name() { return "default_component"; };

	virtual void on_create() {};
	virtual void on_frame() {};
	virtual void on_step() {};
	virtual void after_step() {};
};

class DisplayComponent : public GameComponent {
protected:
	int meta;
public:
	DisplayComponent(uint8_t _id, GameObject* _pnt, int _meta = 10, bool _show = true) : GameComponent(_id, _pnt) { meta = _meta; show = _show; };
	bool show;
	virtual void draw();
	void on_frame() { if(show) draw(); };
};

class ButtonComponent;
class ButtonAction {
protected:
	bool is_active;
	ButtonComponent* btn_owner;
public:
	ButtonAction(ButtonComponent* _owner, bool _active = false) { btn_owner = _owner; is_active = _active; };
	virtual void act() {};
};

class TogglePhysicsAction : public ButtonAction {
protected:
public:
	TogglePhysicsAction(ButtonComponent* _owner, bool _active = false) : ButtonAction(_owner, _active) {};
	void act() override;
};

class ButtonComponent : public GameComponent {
protected:
	int width;
	int height;
	ButtonAction* action;
public:
	ButtonComponent(uint8_t _id, GameObject* _pnt, int _width, int _height, ButtonAction* _action) :GameComponent(_id, _pnt) {
		width = _width;
		height = _height;
		action = _action;
	}
	void set_action(ButtonAction* _action) { action = _action; };
	bool check_clicked();
	void on_frame() override { if (check_clicked()) { action->act(); }; };

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
	
public:
	bool is_static;
	fvector accumulator;
	fvector acceleration;
	fvector position;
	fvector velocity;
	float mass;

	PhysicsComponent(uint8_t _id, GameObject* _pnt, float _mass = 10, fvector _pos = fvector(), fvector _vel = fvector()) : GameComponent(_id, _pnt){
		mass = _mass;
		position = _pos;
		velocity = _vel;
		acceleration = fvector();
		accumulator = fvector();
		is_static = false;
	}
	const char* get_name() { return "physics_component"; }

	//register physics object with the game manager
	void register_component();
	void on_create() { register_component(); }
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
	//void on_frame() override { if(show) draw(); }
};

class BoxDrawComponent : public DisplayComponent {
protected:
	int width;
	int height;
public:
	BoxDrawComponent(uint8_t _id, GameObject* _pnt, int _width, int _height) : DisplayComponent(_id, _pnt) {
		width = _width;
		height = _height;
	}
	void draw() override;
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
	OnMouseDownDebugDraw(uint8_t _id, GameObject* _pnt) : GameComponent(_id, _pnt) { is_active = true; }
	void on_frame() override;
};