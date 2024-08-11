#pragma once
#include "GameComponent.h"

class GameObject {
private:
	uint64_t id;
	GameManager* game_manager;
protected:
	std::vector<GameComponent> object_components;
public:
	ivector screen_position;
	uint64_t get_id() { return id; }
	GameManager* get_game_manager() { return game_manager; }
};