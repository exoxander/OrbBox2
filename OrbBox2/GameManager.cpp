#include "GameManager.h"

GameObject* GameManager::create_game_object(const char* _title) {

	GameObject new_object = GameObject(id_counter, this, _title);
	id_counter++;
	game_objects.push_back(new_object);

	return &game_objects.back();
}