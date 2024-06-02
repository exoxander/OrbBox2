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
		//lets be real
		//if the code goes in here, that means there is 18 quintillion objects in memory
		//the program has been beyond recovery for a long time
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
	//owner list undefined
	//list position undefined

	//set container into hash table
	object_hash_array[_object.get_uid() % prime_hash_value].push_back(new_container);

	//insert triggers into appropriate check lists
	std::vector<TriggerModule>* triggers = _object.get_all_triggers();
	for (int index = 0; index < triggers->size(); index++) {

		//on frame update
		if (triggers->at(index).get_check_type() == on_frame_update) {
			on_frame_check.push_back(&triggers->at(index));
			triggers->at(index).set_list_position(on_frame_check.end());
		}

		//on step
		if (triggers->at(index).get_check_type() == on_step) {
			on_step_check.push_back(&triggers->at(index));
			triggers->at(index).set_list_position(on_step_check.end());
		}

		//on after step
		if (triggers->at(index).get_check_type() == on_step_after) {
			on_step_after_check.push_back(&triggers->at(index));
			triggers->at(index).set_list_position(on_step_after_check.end());
		}
	}
	
	//insert into quad tree if non static
	if (!_is_static) {
		world_tree.insert_object(&new_container);
	}
}

//get an object by its uid
GameObject* GameObjectManger::get_game_object(uint64_t _uid) {
	uint64_t hashed_id = hash(_uid);
	std::list<GameObjectContainer>::iterator list_iterator = object_hash_array[hashed_id].begin();
	std::list<GameObjectContainer>::iterator list_end = object_hash_array[hashed_id].end();

	while (list_iterator != list_end) {
		if (list_iterator->item.get_uid() == _uid) {
			return &list_iterator->item;
		}
		std::advance(list_iterator, 1);
	}

	//not found, likely invalid id, return nullptr
	return nullptr;
}

//destroy the object and reclaim the UID
void GameObjectManger::destroy_game_object(uint64_t _uid) {
	uint64_t hashed_id = hash(_uid);
	std::list<GameObjectContainer>::iterator list_iterator = object_hash_array[hashed_id].begin();
	std::list<GameObjectContainer>::iterator list_end = object_hash_array[hashed_id].end();

	//search
	while (list_iterator != list_end) {
		if (list_iterator->item.get_uid() == _uid) {
			break;
		}
		std::advance(list_iterator, 1);
	}

	//guards for invalid UID
	if (list_iterator == list_end) {
		//throw some error
		return;
	}

	//remove from quad tree if non static
	if (list_iterator->tree_owner != nullptr) {
		world_tree.remove_object(list_iterator->tree_owner, list_iterator->owner_list_iterator);
	}

	//recycle UID
	id_recycle_queue.push(_uid);

	//remove container from hash table
	//item is on stack and should descope? check back later for leaks
	object_hash_array[hashed_id].erase(list_iterator);
}

void GameObjectManger::destroy_game_object(GameObject* _object) {
	//need to use the UID for container access
	destroy_game_object(_object->get_uid());
}

//quad tree

//return a pointer to whichever bottom level quad owns the input coordinant
//requires refactoring, currently recursively checks parents and children until the object is found or the top level quad is hit
Quad* QuadTree::get_inside(fvector _world_coordinant, Quad* _starting_quad, bool _allow_up_search) {
	Quad* start_quad;
	Quad* result = nullptr;

	//if no starting point is supplied start at the top level quad
	if (_starting_quad == nullptr) {
		start_quad = &top_level_quad;
	}
	else {
		start_quad = _starting_quad;
	}

	//check inside starting quad
	if (start_quad->check_inside(_world_coordinant)) {
		//check children
		//if children are null then this is a bottom level quad, return self
		if (start_quad->children[0] == nullptr) {
			return start_quad;
		}
		else {
			for (int quad = 0; quad < 4; quad++) {
				result = get_inside(_world_coordinant, start_quad->children[quad], false);
				if ( result != nullptr) {
					return result;
				}
			}
		}
	}
	else if (start_quad == &top_level_quad) {
		//OBJECT OUT OF WORLD BOUNDS
	}
	else if(_allow_up_search) {
		//check parent, recursive
		result = get_inside(_world_coordinant, start_quad->parent);
	}

	return result;
}