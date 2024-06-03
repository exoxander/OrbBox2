#include "GameModule.h"
//forward declarations
class GameObjectManager;
class Quad;
struct GameObjectContainer;
class PixelGameEngine;

//game modules
void GameModule::set_parent(GameObject* _parent_ptr) {
	if (parent_ptr == nullptr) {
		parent_ptr = _parent_ptr;
	}
}
//trigger modules

//physics module

