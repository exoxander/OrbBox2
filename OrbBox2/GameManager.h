#pragma once
#include "olcPixelGameEngine.h"
#include "GameObject.h"

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
	GameObject* get_object(uint64_t _input);
};

class GameManager {
public:
	PGE* olc_pge;
};