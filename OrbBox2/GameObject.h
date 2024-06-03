#pragma once
#include <vector>
#include "GameModule.cpp"
#include "Vectors.h"
#include "list"
class GameObjectManager;

class GameObject
{
private:
	uint64_t uid;
	std::vector<GameModule> object_modules;
	std::vector<TriggerModule> object_triggers;
	GameObjectManager* gm_ptr;

public:
	int16_t meta;
	ivector screen_position;
	fvector world_position;

	GameObject(uint64_t _uid, GameObjectManager* _gm_ptr) { uid = _uid; gm_ptr = _gm_ptr; };
	void add_module(GameModule _module);
	void add_trigger(TriggerModule _module);
	GameObjectManager* get_manager_ptr() { return gm_ptr; }
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