#pragma once
#include "Vectors.h"

//for some ungodly reason intellisense has an anyurism when theres only one include statement in this file
#include <list>

//forward declare GameObjectManager
class GameObjectManager;
class GameObject;

//enum top_level_type{undef_trigger,trigger};
enum game_module_type{
	undefined,basic_trigger,key_input_trigger,
	collider_trigger,circle_collider_trigger,mesh_collider_trigger,
	physics_body,
	sprite,animation_controller
};


class GameModule{
private:
	GameObject* parent_ptr;
	int16_t meta;

public:
	GameModule(GameObject* _parent_ptr, int16_t _meta = 0) {
		parent_ptr = _parent_ptr;
		meta = _meta;
	};

	int16_t get_meta() { return meta; }
	GameObject* get_parent_ptr() { return parent_ptr; }

	//allows setting the parent, only sets if the existing value is nullptr
	void set_parent(GameObject* _parent_ptr);

	virtual game_module_type get_module_type_name() { return game_module_type::undefined; }
	//virtual top_level_type get_top_level_type_name() { return top_level_type::undefined; }
};

//---------------------------------< Trigger Modules >----------------------------------------
enum trigger_check_type {on_new_frame, on_step, on_after_step, other};
//triggers get sent into check queues for processing in world
//check only on frame update, check on every game step, check on step *after* the other set, other special one in case

class TriggerModule : public GameModule {
protected:
	bool is_triggered;
	bool allow_reset;
	trigger_check_type trigger_type;
	std::list<TriggerModule*>::iterator trigger_list_iterator;

public:
	TriggerModule(GameObject* _parent_ptr, bool _allow_reset, trigger_check_type _trigger_type)
		:GameModule(_parent_ptr) {
		allow_reset = _allow_reset;
		trigger_type = _trigger_type;
	};

	game_module_type get_module_type_name() { return game_module_type::basic_trigger; }
	//top_level_type get_top_level_type_name() { return top_level_type::trigger; }
	void reset();
	void trigger();
	void set_list_position(std::list<TriggerModule*>::iterator _iterator) { trigger_list_iterator = _iterator; }
	trigger_check_type get_check_type() { return trigger_type; }

	//always run this code whenever the appropriate check list is being run through
	virtual void always_activate();

	//specifically check the trigger condition, use inside always activate
	virtual bool check_trigger();

	//on delete, remove self from the trigger lists
	~TriggerModule();
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
class ColliderModule : public TriggerModule {
protected:
	enum collider_type { none, simple_circle, mesh };
	collider_type type;
	bool is_pure_trigger;

public:
	game_module_type get_module_type_name() { return game_module_type::collider_trigger; }
	bool check_trigger();
};

class CircleCollider : public ColliderModule{
protected:
	float radius;

public:
	game_module_type get_module_type_name() { return game_module_type::circle_collider_trigger; }
	CircleCollider(float _radius = 1, bool _is_pure_trigger = false);
	// { radius = _radius; is_trigger = _is_trigger; type = collider_type::simple_circle; }

};

class MeshCollider : public ColliderModule {
protected:
	//mesh structure
public:
	game_module_type get_module_type_name() { return game_module_type::mesh_collider_trigger; }
};

//---------------------------------< Physics Body >-------------------------------------------
class PhysicsBody : public GameModule {
public:
	game_module_type get_module_type_name() { return game_module_type::physics_body; }
	float mass;
	fvector velocity;
	ColliderModule collider;
};

//---------------------------------< Sprite and Animation >-----------------------------------
//https://github.com/OneLoneCoder/olcPixelGameEngine/wiki/TUTORIAL---Sprites
class Sprite : public GameModule {
public:
	game_module_type get_module_type_name() { return game_module_type::sprite; }
};
struct Animation {
	//list of sprites
	float duration;//in seconds
	bool allow_interrupt;
};
class AnimationController : public GameModule {
	//list of animations (containing lists of sprites)
	//list of associated animation triggers
public:
	game_module_type get_module_type_name() { return game_module_type::animation_controller; }
};


//==========================< testing junk >======================================
class MouseFollowModule : public TriggerModule {
public:
	MouseFollowModule(GameObject* _parent_ptr, bool _allow_reset)
		:TriggerModule(_parent_ptr, _allow_reset, on_new_frame) {
		//do nothing special
	}
	void always_activate();
	void set_position_to_mouse(GameObjectManager* _game_manager);
};

class DrawQuadLineage : public TriggerModule {
public:
	DrawQuadLineage(GameObject* _parent_ptr, bool _allow_reset)
		:TriggerModule(_parent_ptr, _allow_reset, on_after_step) {
		//do nothing
	}
	void always_activate();
	void draw_all_quad_parents(GameObjectManager* _game_manager);
};
