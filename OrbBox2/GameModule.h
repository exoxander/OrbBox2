#pragma once
#include "Vectors.h"
class GameModule
{
private:
	uint64_t parent_uid;
	int16_t uid;

public:
	int16_t get_uid() { return uid; };
	uint64_t get_parent_uid() { return parent_uid; }
};

//---------------------------------< Trigger Modules >----------------------------------------
class TriggerModule : GameModule {
protected:
	bool is_triggered;
	bool allow_reset;

public:
	void reset();
	void trigger();
	virtual bool check_trigger();
};

class KeyInputTrigger : TriggerModule {
	// check if a key is active and what type of activation it is
protected:
	enum key_input_type {pressed,down,held};
	bool key_down;
public:
};

//---------------------------------< Colliders >-----------------------------------------
class ColliderModule : TriggerModule {
protected:
	enum collider_type { none, simple_circle, mesh };
	collider_type type;
	bool is_pure_trigger;

public:
	bool check_trigger() override;
};

class CircleCollider : ColliderModule{
protected:
	float radius;

public:
	CircleCollider(float _radius = 1, bool _is_pure_trigger = false);
	// { radius = _radius; is_trigger = _is_trigger; type = collider_type::simple_circle; }

};

class MeshCollider : ColliderModule {
protected:
	//mesh structure
public:
};

//---------------------------------< Physics Body >-------------------------------------------
class PhysicsBody : GameModule {
public:
	float mass;
	fvector velocity;
	ColliderModule collider;
};

//---------------------------------< Sprite and Animation >-----------------------------------
//https://github.com/OneLoneCoder/olcPixelGameEngine/wiki/TUTORIAL---Sprites
class Sprite : GameModule {

};
struct Animation {
	//list of sprites
	float duration;//in seconds
	bool allow_interrupt;
};
class AnimationController : GameModule {
	//list of animations
	//list of associated animation triggers
};

