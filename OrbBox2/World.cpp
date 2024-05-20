#include "World.h"

//object hash table

//create a new clean object with a unique 64 bit identifier
GameObject GameObjectManger::create_game_object() {
	uint64_t valid_uid = 0;

	if (next_uid_counter == 0) {
		next_uid_counter++;
		return GameObject(valid_uid);
	}
	else if (next_uid_counter == UINT64_MAX) {
		//throw some error and stop the program
		//if the game has 2^64 objects its probably already borked tho
	}
	else {
		if (id_recycle_queue.empty()) {
			valid_uid = next_uid_counter;
			next_uid_counter++;
		}
		else {
			valid_uid = id_recycle_queue.front();
			id_recycle_queue.pop();
		}
	}

	return GameObject(valid_uid);
};

//add an existing completed object and its trigger modules into the hash table and trigger lists
void GameObjectManger::insert_new_game_object(GameObject _object, bool _is_static) {
	GameObjectContainer new_container = GameObjectContainer();

	new_container.item = _object;

	//set container into hash table
	object_hash_array[_object.get_uid() % prime_hash_value].push_back(new_container);

	//insert triggers into appropriate lists
	std::vector<TriggerModule>* triggers = _object.get_all_triggers();
	for (int item = 0; item < triggers->size(); item++) {

		//on frame update
		if (triggers->at(item).get_check_type() == on_frame_update) {
			on_frame_check.push_back(&triggers->at(item));
			triggers->at(item).set_list_position(on_frame_check.end());
		}

		//on step
		if (triggers->at(item).get_check_type() == on_step) {
			on_frame_check.push_back(&triggers->at(item));
			triggers->at(item).set_list_position(on_step_check.end());
		}

		//on after step
		if (triggers->at(item).get_check_type() == on_after_step) {
			on_frame_check.push_back(&triggers->at(item));
			triggers->at(item).set_list_position(after_on_step_check.end());
		}
	}
	
	//insert into quad tree if non static
	if (!_is_static) {
		world_tree.insert_object(&new_container);
	}
}

//get an object by its uid
GameObject* GameObjectManger::get_game_object(uint64_t _uid) {
	std::list<GameObjectContainer>::iterator list_iterator = object_hash_array[_uid % prime_hash_value].begin();
	std::list<GameObjectContainer>::iterator list_end = object_hash_array[_uid % prime_hash_value].end();

	while (list_iterator != list_end) {
		if (list_iterator->item.get_uid() == _uid) {
			return &list_iterator->item;
		}
		std::advance(list_iterator, 1);
	}

	return nullptr;
}

//destroy the object and reclaim the UID
void GameObjectManger::destroy_game_object(uint64_t _uid) {
	std::list<GameObjectContainer>::iterator list_iterator = object_hash_array[_uid % prime_hash_value].begin();
	std::list<GameObjectContainer>::iterator list_end = object_hash_array[_uid % prime_hash_value].end();

	while (list_iterator != list_end) {
		if (list_iterator->item.get_uid() == _uid) {
			break;
		}
		std::advance(list_iterator, 1);
	}

	//remove from quad tree
	world_tree.remove_object(list_iterator->quad_owner_list, list_iterator->owner_list_iterator);

	//recycle UID
	id_recycle_queue.push(_uid);

	//remove container from hash table, check for possible memory leaks
	object_hash_array[_uid % prime_hash_value].erase(list_iterator);
}

void GameObjectManger::destroy_game_object(GameObject* _object) {
	//need to use the UID for container access
	destroy_game_object(_object->get_uid());
}

//quad tree