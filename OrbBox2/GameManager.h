#pragma once
#include "olcPixelGameEngine.h"
#include "GameObject.h"
//forward declare
class PGE;

class Quad {
private:
public:
	Quad* children[4] = {nullptr, nullptr, nullptr, nullptr};
	std::list<GameObject> owned_objects;
};

class QuadTree {
private:
public:
};

class HashTable {
private:
	const int array_size = 4096;
	const int hash_value = 4093;
	// 4094 and 4095 inaccessable to insert, reserve for engine items?
	//big prime, 65,521 (65,536)
	std::list<GameObject*>* object_table;
	//object_table == new std::list<GameObject*>[array_size];
	std::list<GameObject*>::iterator hash(uint64_t input) { return object_table[input % array_size].begin(); };
public:
	//GameObject* get_object(uint64_t _input);
};

class FrogIntegrator {
private:
public:
	void leap(std::list<PhysicsComponent*>::iterator _iterator, std::list<PhysicsComponent*>::iterator _end, bool _pause, float _dt = (1.0f / 60.0f));
};

class Camera {
private:
	fvector view_offset;
	float view_scale;
	float offset_move_speed;
public:
	Camera() {
		view_offset = fvector();
		view_scale = 1;
		offset_move_speed = 5;
	}
	ivector world_to_screen(fvector _world);
	fvector screen_to_world(ivector _screen);
	void move_view(PGE* _pge_ptr);
};

//game options,held in struct
struct Options {
	bool object_name_debug_draw = false;
	bool pause_physics = false;
};

class GameManager {
private:
	uint64_t id_counter;
public:
	PGE* olc_pge;
	GameObject* create_game_object(const char* _title = nullptr);
	Options game_options;
	std::list<PhysicsComponent*> physics_components;
	Camera view;
	FrogIntegrator frog;

	//temp
	std::list<GameObject> game_objects;
	GameManager(PGE* _pge) {
		olc_pge = _pge;
		game_objects = std::list<GameObject>();
		physics_components = std::list<PhysicsComponent*>();
		id_counter = 0;
		//debug options
		game_options = Options();
		view = Camera();
		frog = FrogIntegrator();
	}

	void step();
};