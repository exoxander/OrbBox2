#include "World.h"
#include <math.h>

#pragma region discriminators
//quad tree discriminator functions

//check nothing, return everything, get_and_splice_objects() defaults to this
bool check_none(GameObjectContainer* _container, DCFInfo* _info) { return true; }

//return only items that are within the radius of the distance descriminators center point
//the inquisitee is the object being filtered, the _info centerpoint is the reference for the distance search
bool check_inside_range(GameObjectContainer* _inquisitee, DistanceDCF* _info) {
	fvector ref_point = _info->compare_point;
	fvector item_point = _inquisitee->item.world_position;
	float x = ref_point.x - item_point.x;
	float y = ref_point.y - item_point.y;

	if (_info->radius >= sqrt((x * x) + (y * y))) {
		return true;
	}
	return false;
}
#pragma endregion search discriminator functions

#pragma region game_object_manager
//create a new clean object with a unique 64 bit identifier
GameObject GameObjectManager::create_game_object() {
	uint64_t valid_uid = 0;

	if (next_uid_counter == 0) {
		next_uid_counter++;
		return GameObject(valid_uid, this);
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

	return GameObject(valid_uid, this);
};

//add an existing completed object and its trigger modules into the hash table and trigger lists
void GameObjectManager::insert_new_game_object(GameObject _object, bool _is_static) {
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
		if (triggers->at(index).get_check_type() == on_new_frame) {
			on_frame_check.push_back(&triggers->at(index));
			triggers->at(index).set_list_position(on_frame_check.end());
		}

		//on step
		if (triggers->at(index).get_check_type() == on_step) {
			on_step_check.push_back(&triggers->at(index));
			triggers->at(index).set_list_position(on_step_check.end());
		}

		//on after step
		if (triggers->at(index).get_check_type() == on_after_step) {
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
GameObject* GameObjectManager::get_game_object(uint64_t _uid) {
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

//get an objects container by its uid
GameObjectContainer* GameObjectManager::get_object_container(uint64_t _uid) {
	uint64_t hashed_id = hash(_uid);
	std::list<GameObjectContainer>::iterator list_iterator = object_hash_array[hashed_id].begin();
	std::list<GameObjectContainer>::iterator list_end = object_hash_array[hashed_id].end();

	while (list_iterator != list_end) {
		if (list_iterator->item.get_uid() == _uid) {
			return &*list_iterator;
		}
		std::advance(list_iterator, 1);
	}

	return nullptr;
}

//destroy the object and reclaim the UID
void GameObjectManager::destroy_game_object(uint64_t _uid) {
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

void GameObjectManager::destroy_game_object(GameObject* _object) {
	//need to use the UID for container access
	destroy_game_object(_object->get_uid());
}
#pragma endregion all GameObjectManager functions

#pragma region Quad
//quad
Quad::Quad(uint8_t _level, fvector _center, float _manhattan_radius) {
	level = _level;
	center = _center;
	manhattan_radius = _manhattan_radius;
	parent = nullptr;
}

Quad::~Quad() {
	//recursively kill all children
	for (int quad = 0; quad < 4; quad++) {
		delete children[quad];
	}
}

bool Quad::check_inside(fvector _world_coordinant) {
	fvector relative_position = center - _world_coordinant;
	//check x
	if (relative_position.x <= manhattan_radius && relative_position.x > -manhattan_radius) {
		//check y
		if (relative_position.y <= manhattan_radius && relative_position.y > -manhattan_radius) {
			return true;
		}
	}
	return false;
}

void Quad::create_children(uint8_t _level_limit) {

	//clockwise: top left--, top right+-, bottom right++, bottom left-+
	float child_radius = manhattan_radius / 2;
	fvector next_child_center = fvector();

	//a
	next_child_center = center + fvector(-child_radius, -child_radius);
	children[0] = new Quad(level++, next_child_center, child_radius);

	//b
	next_child_center = center + fvector(child_radius, -child_radius);
	children[1] = new Quad(level++, next_child_center, child_radius);

	//c
	next_child_center = center + fvector(child_radius, child_radius);
	children[2] = new Quad(level++, next_child_center, child_radius);

	//d
	next_child_center = center + fvector(-child_radius, child_radius);
	children[3] = new Quad(level++, next_child_center, child_radius);

	//check level limit, create grandchildren if level value allows
	if (level < _level_limit) {
		for (int quad = 0; quad < 4; quad++) {
			children[quad]->create_children(_level_limit);
		}
	}
}
#pragma endregion all Quad functions

#pragma region QuadTree
//quad tree
void QuadTree::generate_tree() {
	top_level_quad = Quad(0, fvector(), top_level_quad_size / 2);
	//topsize / (2^level) = levelsize
	//log2(topsize/minsize) >= level
	tree_level_limit = floor(log2f(top_level_quad_size / bottom_level_quad_boundry));

	//create the universe
	top_level_quad.create_children(tree_level_limit);
}

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
		//up search to parent
		result = get_inside(_world_coordinant, start_quad->parent);
	}

	return result;
}

std::list<GameObjectContainer*> QuadTree::get_and_splice_objects(Quad* _current_quad, bool (*_dcf)(GameObjectContainer* _container, DCFInfo* _dcf_info), DCFInfo* _info) {
	bool (*descriminator_function)(GameObjectContainer * container, DCFInfo* info) = _dcf;

	//watch for descope issues
	std::list<GameObjectContainer*> filtered_list = std::list<GameObjectContainer*>();
	std::list<GameObjectContainer*>::iterator current_object = _current_quad->game_object_list.begin();
	std::list<GameObjectContainer*>::iterator end = _current_quad->game_object_list.end();
	GameObjectContainer* object_copy = nullptr;

	//copy all owned objects into new list, filtering with discriminator function
	if (!_current_quad->game_object_list.empty()) {
		while (current_object != end) {
			//filter function
			if (descriminator_function(*current_object, _info)) {

				//may not be nessicary, but feels better
				object_copy = *current_object;
				filtered_list.push_back(object_copy);
			}

			std::advance(current_object, 1);
		}
	}
	//recurse on children
	if (_current_quad->children[0] != nullptr) {
		for (int quad = 0; quad < 4; quad++) {
			//recurse on children and splice result into filtered list
			filtered_list.splice(filtered_list.end(), get_and_splice_objects(_current_quad->children[quad], _dcf, _info));			
		}
	}
	
	//return new list
	return filtered_list;
}


/* quadtree traversal based on a search radius
searchRadius <= manhattanRadius = (topQuadSize / (2^level))
searchRadius <= (topQuadSize / (2^level)
SR * (2^L) <= TQS
2^l <= TQS / SR
level <= log2(topQuadSize / searchRadius)
*/

std::list<GameObjectContainer*> QuadTree::return_in_radius(GameObjectContainer* _inquisitor, int _search_radius) {
	//determine level to search at
	//navigate to level from inquisitor owner
	//get pointers to immediate siblings and cousins of top level center
	//run get_and_splice_objects() on sublings and cousins
	//splice and return results from get_and_splice_objects() using the range dcf and info
}
#pragma endregion all quad tree functions