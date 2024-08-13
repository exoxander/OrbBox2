#include "GameManager.h"
#include <cmath>

GameObject* GameManager::create_game_object(const char* _title) {

	GameObject new_object = GameObject(id_counter, this, _title);
	id_counter++;
	game_objects.push_back(new_object);

	return &game_objects.back();
}

void GameManager::step() {
	frog.leap(physics_components.begin(), physics_components.end(),game_options.pause_physics);
}

ivector Camera::world_to_screen(fvector _world) {
	ivector result = ivector();
	result.x = static_cast<int>(_world.x + view_offset.x);
	result.y = static_cast<int>(_world.y + view_offset.y);

	return result;
}

fvector Camera::screen_to_world(ivector _screen) {
	fvector result = fvector();
	result.x = static_cast<float>(_screen.x - view_offset.x);
	result.y = static_cast<float>(_screen.y - view_offset.y);
	return result;
}

//==================================[ FROG INTEGRATOR ]====================================
void FrogIntegrator::leap(std::list<PhysicsComponent*>::iterator _iterator, std::list<PhysicsComponent*>::iterator _end,bool _pause, float _dt) {
	std::list<PhysicsComponent*>::iterator matrix_outer = _iterator;
	std::list<PhysicsComponent*>::iterator matrix_inner = _iterator;
	float G = .000001;
	//index [a,b] = a + b*size

	if (!_pause) {
		//iterate over all registered physics objects
	//physics
	//compute distance and forces for each interaction pair only once
		while (matrix_outer != _end) {
			fvector gravity_force = fvector(0, 0);
			matrix_inner = matrix_outer;
			PhysicsComponent* m1 = *matrix_outer;

			while (matrix_inner != _end) {
				float distance = 0;
				PhysicsComponent* m2 = *matrix_inner;

				//check for valid interaction pair
				if (matrix_outer != matrix_inner && (m1->get_parent()->state == ObjectState::alive && m2->get_parent()->state == ObjectState::alive)) {
					//direction and distance
					gravity_force = (m2->position - m1->position);
					distance = gravity_force.distance();
					gravity_force.normalize();
					//acceleration force = G*(m1*m2)/r^2
					gravity_force *= (G * (m1->mass * m2->mass) / (distance * distance));

					//force applied to current object
					m1->accumulator += gravity_force;
					//inverse force applied to other body
					m2->accumulator += (gravity_force * -1);
				}
				std::advance(matrix_inner, 1);
			}
			std::advance(matrix_outer, 1);
		}


		//leapfrog integration
		//https://en.wikipedia.org/wiki/Leapfrog_integration

		while (_iterator != _end) {
			PhysicsComponent* p = *_iterator;
			if (!p->is_static) {
				p->accumulator /= p->mass;// F/m=a
				fvector v_half = fvector();
				//kick-drift-kick leap frog
				// v[i+.5] = v[i] + a[i]*(dt/2)
				v_half = p->velocity + (p->acceleration * (_dt / 2));
				// x[i+1] = x[i] + v[i+.5]*dt
				p->position = p->position + (v_half * _dt);
				// v[i+1] = v[i+.5]+a[i+1]*(dt/2)
				p->velocity = v_half + (p->accumulator * (_dt / 2));
				//move acceleration values and reset accumulator for next iteration
				p->acceleration = (p->accumulator + fvector());

				p->accumulator = fvector();

				//set parent screen position from new world position
				p->get_parent()->screen_position = p->get_parent()->game_manager->view.world_to_screen(p->position);
			}
			else {
				p->position = p->get_parent()->game_manager->view.screen_to_world(p->get_parent()->screen_position);
			}
			std::advance(_iterator, 1);
		}
	}
}