#pragma once
#include "GameComponent.h"
//forward declare
class GameManager;


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

	GameObject(uint64_t _id, GameManager* _mgr, const char* _name = nullptr) {
		id = _id;
		game_manager = _mgr;
		name = _name == nullptr ? "default_object" : _name;
		screen_position = ivector();
		object_components = std::vector<GameComponent* >();
	};

	void insert_component(GameComponent* _comp) { object_components.push_back(_comp); }
};