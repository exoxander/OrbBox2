#pragma once
#include <vector>
#include "GameModule.cpp"
#include "Vectors.h"
#include "list"

class GameObject
{
private:
	uint64_t uid;
	std::vector<GameModule> modules;
	std::vector<TriggerModule> triggers;

public:
	int16_t meta;
	ivector screen_position;
	fvector world_position;

	GameObject(uint64_t _uid) { uid = _uid; };
	void add_module(GameModule _module);

	//call destroy on all modules
	~GameObject();

	int16_t get_uid() { return uid; }
	std::vector<GameModule>* get_all_modules();
	std::vector<TriggerModule>* get_all_triggers();

	//only returns the first found, best be specific
	GameModule* get_first_module_by_type(game_module_type type);

	//return all modules of a type, make sure recieved deletes the list after
	std::list<GameModule*> get_all_module_by_type(game_module_type type);
};