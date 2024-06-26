#pragma once
#include "GameObject.cpp"
#include "Utilities.h"
#include <list>
#include <queue>
#include "olcPixelGameEngine.h"

//world space
struct GameObjectContainer {
	GameObject item;

	//link back to the quad tree
	//ignore these for static objects
	Quad* tree_owner;
	std::list<GameObjectContainer*>::iterator owner_list_iterator;

	GameObjectContainer();
};

//quadtree filters

class ObjectFilter {
public:
	ObjectFilter();
	virtual bool filter(GameObjectContainer* _item) { return true; }
};

class DistanceFilter : public ObjectFilter {
private:
	fvector ref_point;
	float radius;
public:
	DistanceFilter(fvector _point = fvector(), float _radius = 0) { ref_point = _point; radius = _radius; }
	bool filter(GameObjectContainer* _item);
};


//QuadTree
enum quad_pos{a,b,c,d};

/* https://www.youtube.com/watch?v=wXF3HIhnUOg&list=WL&index=11&t=1172s */
class Quad {
public:
	//depth in the tree starting at 0 for world quad
	uint8_t level;
	float manhattan_radius;
	fvector center;

	Quad* parent;
	quad_pos position;

	Quad* children[4] = {nullptr,nullptr,nullptr,nullptr};

	//owned items list
	std::list<GameObjectContainer*> game_object_list;

	//list<t,allocator>::splice
	//return immediate children
	std::list<std::list<GameObjectContainer*>::iterator> return_owned_objects();
	//returns all children, searched recursively
	std::list<std::list<GameObjectContainer*>::iterator> return_all_owned_objects();

	bool check_inside(fvector _world_coordinant);
	void create_children(uint8_t _level_limit);
	Quad();
	Quad(uint8_t _level, fvector _center, float _manhattan_radius, quad_pos _pos = a);
	~Quad();
};



class QuadTree {
private:
	//god quad, owns the game world
	Quad top_level_quad;
	//dimensions of top level quad, meters, recommend keeping as power of 2 for nice division
	float top_level_quad_size = 1024;
	//boundy below which to stop generating more quads, meters
	float bottom_level_quad_boundry = 8;
	//computed INCLUSIVE limit for depth based on quad boundry
	int tree_level_limit = 0;

public:
	QuadTree(float _top_size, float _bottom_boundry) { top_level_quad_size = _top_size; bottom_level_quad_boundry = _bottom_boundry; }
	void insert_object(GameObjectContainer* _container);
	void remove_object(Quad* _tree_quad, std::list<GameObjectContainer*>::iterator _container_itr);
	void move_object();
	void generate_tree();

	//get which quad this coordinant falls into, optionally starting from a known point inside the tree
	Quad* get_inside(fvector _world_coordinant, Quad* _start_point = nullptr, bool _allow_up_search = true);
	Quad* get_top_level_quad() { return &top_level_quad; }

	//traverses around the tree and returns the given sibling / cousin to the start quad
	Quad* get_quad_relative(quad_pos _start, quad_pos _end, int _level);

	//functions to be pointed into get_and_splice_objects as the descriminator during copying

	

	//return a list of containers for all objects in surrounding quads and their children (surrounding meaning the containing and 8 adjacent quads)
	//search radius property is used to determine the the highest level of the tree searched, radius of zero means search the whole world
	
	//recursive, returns a spliced list of all objects contained within a quad heiarchy, optional filter with a descriminator function and info
	std::list<GameObjectContainer*> get_and_splice_objects(Quad* _input_quad, ObjectFilter* _filter = &ObjectFilter());

	//determines which quads to search using get_and_splace_objects() and returns their ouputs
	std::list<GameObjectContainer*> return_in_radius(GameObjectContainer* _inquisitor, int _search_radius);
};


//global object hash
class GameObjectManager {
private:
	const int hash_array_size = 4096;
	const int prime_hash_value = 4093;
	// 4094 and 4095 inaccessable to insert, reserve for engine items?
	//big prime, 65,521 (65,536)

	//recycle the ids of dead objects, preferentially use these vs generating new uids
	std::queue<uint64_t> id_recycle_queue;
	uint64_t next_uid_counter;
	float internal_tick_rate = (1.0 / 64.0);
	bool is_new_frame = false;

public:
	void new_frame_ready() { is_new_frame = true; };
	olc::PixelGameEngine* internal_pge;
	uint64_t hash(uint64_t _uid) { return _uid % prime_hash_value; }

	//look into replacing with with a red-black tree structure, O[log2(n)] vs O[n]
	std::list<GameObjectContainer>* object_hash_array = new std::list<GameObjectContainer>[hash_array_size];
	QuadTree world_tree = QuadTree(1024, 10);

	GameObject create_game_object();
	void insert_new_game_object(GameObject _object, bool _is_static);

	GameObject* get_game_object(uint64_t _uid);
	GameObjectContainer* get_object_container(uint64_t _uid);

	void destroy_game_object(uint64_t _uid);
	void destroy_game_object(GameObject* _object);


	//on frame trigger list
	std::list<TriggerModule*> on_frame_check;
	//on step trigger list
	std::list<TriggerModule*> on_step_check;
	//on step after trigger list
	std::list<TriggerModule*> on_step_after_check;

};
