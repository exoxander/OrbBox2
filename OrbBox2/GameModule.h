#pragma once
#include "Vectors.h"
enum game_module_type{
	undefined,basic_trigger,key_input_trigger,
	collider_trigger,circle_collider_trigger,mesh_collider_trigger,
	physics_body,
	sprite,animation_controller
};

class GameModule
{
private:
	uint64_t parent_uid;
	int16_t uid;

public:
	int16_t get_uid() { return uid; };
	uint64_t get_parent_uid() { return parent_uid; }
	virtual game_module_type get_module_type_name() { return game_module_type::undefined; }
};

//---------------------------------< Trigger Modules >----------------------------------------
enum trigger_check_type {on_frame_update, on_step, on_step_after, other};
//triggers get sent into check queues for processing in world
//check only on frame update, check on every game step, check on step *after* the other set, other special one in case

class TriggerModule : GameModule {
protected:
	bool is_triggered;
	bool allow_reset;
	trigger_check_type check_type;

public:
	game_module_type get_module_type_name() { return game_module_type::basic_trigger; }
	void reset();
	void trigger();
	trigger_check_type get_check_type() { return check_type; }
	virtual bool check_trigger();
};

class KeyInputTrigger : TriggerModule {
	// check if a key is active and what type of activation it is
protected:
	enum key_input_type {pressed,down,held};
	bool key_down;
public:
	game_module_type get_module_type_name() { return game_module_type::key_input_trigger; }
};

//---------------------------------< Colliders >-----------------------------------------
class ColliderModule : TriggerModule {
protected:
	enum collider_type { none, simple_circle, mesh };
	collider_type type;
	bool is_pure_trigger;

public:
	game_module_type get_module_type_name() { return game_module_type::collider_trigger; }
	bool check_trigger();
};

class CircleCollider : ColliderModule{
protected:
	float radius;

public:
	game_module_type get_module_type_name() { return game_module_type::circle_collider_trigger; }
	CircleCollider(float _radius = 1, bool _is_pure_trigger = false);
	// { radius = _radius; is_trigger = _is_trigger; type = collider_type::simple_circle; }

};

class MeshCollider : ColliderModule {
protected:
	//mesh structure
public:
	game_module_type get_module_type_name() { return game_module_type::mesh_collider_trigger; }
};

//---------------------------------< Physics Body >-------------------------------------------
class PhysicsBody : GameModule {
public:
	game_module_type get_module_type_name() { return game_module_type::physics_body; }
	float mass;
	fvector velocity;
	ColliderModule collider;
};

//---------------------------------< Sprite and Animation >-----------------------------------
//https://github.com/OneLoneCoder/olcPixelGameEngine/wiki/TUTORIAL---Sprites
class Sprite : GameModule {
public:
	game_module_type get_module_type_name() { return game_module_type::sprite; }
};
struct Animation {
	//list of sprites
	float duration;//in seconds
	bool allow_interrupt;
};
class AnimationController : GameModule {
	//list of animations (containing lists of sprites)
	//list of associated animation triggers
public:
	game_module_type get_module_type_name() { return game_module_type::animation_controller; }
};

