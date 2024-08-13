#pragma once
#include "GameComponent.h"

//forward declare
class GameManager;
class Quad;
enum ObjectState{alive,dead,ghost};
//alive: normal, send and recieve interactions
//dead: marked to be destroyed, no interactions
//ghost: can only recieve interactions and change internal state;

class GameObject {
private:
	uint64_t id;
protected:
public:
	const char* name;
	ivector screen_position;
	uint64_t get_id() { return id; };
	GameManager* game_manager;
	std::vector<GameComponent*> object_components;
	ObjectState state;
	Quad* tree_owner;

	GameObject(uint64_t _id, GameManager* _mgr, const char* _name = nullptr, ObjectState _state = ObjectState::alive) {
		id = _id;
		game_manager = _mgr;
		name = _name == nullptr ? "default_object" : _name;
		screen_position = ivector();
		object_components = std::vector<GameComponent* >();
		state = _state;
		tree_owner = nullptr;
	};

	GameComponent* get_component_by_name(const char* _name) { return nullptr; };
	void insert_component(GameComponent* _comp) { object_components.push_back(_comp); }
};