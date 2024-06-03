#include "GameObject.h"
void GameObject::add_module(GameModule _module) {
	object_modules.push_back(_module);
	object_modules.back().set_parent(this);
}

void GameObject::add_trigger(TriggerModule _trigger) {
	object_triggers.push_back(_trigger);
	object_triggers.back().set_parent(this);
}