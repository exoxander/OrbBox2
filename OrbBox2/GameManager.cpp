#include "GameManager.h"

GameObject* GameManager::create_game_object(const char* _title) {

	GameObject new_object = GameObject(id_counter, this, _title);
	id_counter++;
	game_objects.push_back(new_object);

	return &game_objects.back();
}

void GameManager::step() {
	frog.leap(physics_components.begin(), physics_components.end(), physics_components.size());
}

ivector Camera::world_to_screen(fvector _world) {
	ivector result = ivector();
	result.x = static_cast<int>(_world.x);
	result.y = static_cast<int>(_world.y);

	return result;
}

fvector Camera::screen_to_world(ivector _screen) {
	fvector result = fvector();
	result.x = _screen.x;
	result.y = _screen.y;
	return result;
}

void FrogIntegrator::leap(std::list<PhysicsComponent*>::iterator _iterator, std::list<PhysicsComponent*>::iterator _end,int _matrix_size, float _dt) {
	std::list<PhysicsComponent*>::iterator matrix_outer = _iterator;
	std::list<PhysicsComponent*>::iterator matrix_inner = _iterator;
	//fvector* acceleration_matrix = new fvector[_matrix_size * _matrix_size];
	float G = .000001;
	int row = 0;
	int col = 0;
	//index [a,b] = a + b*size

	//iterate over all registered physics objects
	//physics
	//acceleration matrix	
	//compute half of matrix, diagonal mirror is just inverted values
	while (matrix_outer != _end) {
		fvector gravity_force = fvector(0,0);
		matrix_inner = matrix_outer;
		col = row;
		PhysicsComponent* m1 = *matrix_outer;

		while (matrix_inner != _end) {
			float distance = 0;
			PhysicsComponent* m2 = *matrix_inner;

			if (matrix_outer == matrix_inner || !(m1->get_parent()->state == ObjectState::alive && m2->get_parent()->state == ObjectState::alive)) {
				//vector invalid, done on self or involves a non-living object
			}
			else {
				//direction and distance
				gravity_force = m1->position - m2->position;
				distance = gravity_force.distance();
				//acceleration force = G*(m1*m2)/r^2
				//F = ma
				//a = F/m
				gravity_force *= (G * (m1->mass * m2->mass) / (distance * distance));
				m2->accumulator += gravity_force;
				m1->accumulator += (gravity_force * -1);
			}

			std::advance(matrix_inner, 1);
			col++;
		}
		std::advance(matrix_outer, 1);
		row++;
	}


	//leapfrog integration
	
	
	while (_iterator != _end) {
		PhysicsComponent* p = *_iterator;
		if (!p->is_static) {
			p->accumulator /= p->mass;
			//new pos = curr pos + curr vel * dt + (1/2)*(accel)*(dt*dt)
			p->position = (p->position + (p->velocity * _dt) + (p->acceleration * 0.5f * _dt * _dt));
			//new vel = curr vel + (accel accumulator)*0.5*dt
			p->velocity = (p->velocity + (p->acceleration + p->accumulator) * 0.5f * _dt);

			//move acceleration values and reset accumulator for next iteration
			p->acceleration = p->accumulator;
			p->accumulator = fvector();

			//set parent screen position from new world position
			p->get_parent()->screen_position = p->get_parent()->game_manager->view.world_to_screen(p->position);
		}
		else {
			p->position = p->get_parent()->game_manager->view.screen_to_world(p->get_parent()->screen_position);
		}
		std::advance(_iterator, 1);
	}
	//delete [] acceleration_matrix;
}