//#include "GameObject.h"

//return the first component which matches the given name
/*
GameComponent* GameObject::get_component_by_name(const char* _name) {
	std::vector<GameComponent*>::iterator it = object_components.begin();
	std::vector<GameComponent*>::iterator end = object_components.end();
	while (it != end) {
		GameComponent* gc = *it;
		if (gc->get_name() == _name) {
			return gc;
		}
		std::advance(it, 1);
	}

	return nullptr;
}*/