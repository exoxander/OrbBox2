#pragma once
#include "GameObject.h"
#include "Utilities.h"
#include <list>
//world space

//global object hash
class GameObjectManger {
	//global object hash table
	//4093 prime value
	std::list<GameObject> hash_array[4096];// 4094 and 4095 inaccessable to insert, reserve for engine items?
	GameObject create_game_object();
	void insert_game_object(GameObject);// hash_array[uid % 4093]
	void delete_game_object(int64_t uid);

	//on frame trigger list
	//on step trigger list
	//on step after trigger list

};

//quadtree
struct Quad {
	std::list<uint64_t> game_object_list;
	int level;//depth in the tree starting at 0 for world quad

	Quad* a;
	Quad* b;
	Quad* c;
	Quad* d;

	std::list<uint64_t> return_owned_objects();//return immediate children
	std::list<uint64_t> return_all_owned_objects();//returns all children, searched recursively
};

class Quadtree {
private:
	Quad top_level_quad;//god quad, owns the game world

	//dimensions of top level quad, meters
	float top_level_quad_size = 1000;
	//boundy below which to stop generating more quads, meters
	float bottom_level_quad_boundry = 10;
public:
	void generate_tree();

	//return a list of ids for all objects in surrounding quads and their children (surrounding meaning the containing and 8 adjacent quads)
	//search radius property is used to determine the the highest level of the tree searched, radius of zero means search the whole world
	//size of top level quads to search at (god quad size / (2 ^ quad level)) >= return_radius

	/* MATHEMATICS
	(godqsize / (2^quadlevel)) >= retrradi

	godqsize >= retrradi * (2^quadlevel)

	(godqsize / retrradi) >= 2^quadlevel

	log2(godqsize / retrradi) >= quadlevel
	*/
	std::list<uint64_t> return_all_nearby(uint64_t parent_uid, int return_radius = 0);
};