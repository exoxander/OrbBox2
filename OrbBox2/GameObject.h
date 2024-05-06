#pragma once
#include <vector>
#include "GameModule.h"
#include "Vectors.h"

class GameObject
{
private:
	uint64_t uid;
	std::vector<GameModule> modules;

public:
	int16_t meta;
	ivector screen_position;
	fvector world_position;

	int16_t get_uid() { return uid; }
	std::vector<GameModule>& get_modules();
	GameModule* get_module_by_type(GameModuleType type);//only returns the first found, best be specific
};