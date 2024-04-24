#pragma once
#include "GameObject.h"
#include "Utilities.h"
#include <list>
//world space

//global object hash
class GameObjectManger {
	//global object hash table
	//4093 prime value
	std::list<GameObject> hash_array[4096];
	GameObject create_game_object();
	void insert_game_object(GameObject);// hash_array[uid % 4093]
	void delete_game_object(int64_t uid);

	//trigger module pointer list

};

//quadtree
struct Quad {
	std::list<uint64_t> game_object_list;

	Quad* a;
	Quad* b;
	Quad* c;
	Quad* d;

	std::list<uint64_t> return_owned_objects();
	std::list<uint64_t> return_all_owned_objects();
};

class Quadtree {
	Quad top_level_quad;

	//return a list of ids for all objects in surrounding quads and their children
	//search can be expanded with the level property to higher level quads
	std::list<uint64_t> return_all_nearby(uint64_t parent_uid, int quad_check_level = 0);
};