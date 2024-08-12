#define OLC_PGE_APPLICATION
#include "GameManager.h"
#include <iostream>

//https://github.com/OneLoneCoder/olcPixelGameEngine/wiki/Multiple-File-Projects

//https://github.com/OneLoneCoder/olcPixelGameEngine/wiki/olc::PixelGameEngine-Documentation

//multi file projects
//https://github.com/OneLoneCoder/olcPixelGameEngine/wiki/Multiple-File-Projects




class PGE : public olc::PixelGameEngine {
	GameManager game_manager = GameManager(this);
	float fRate = (1 / 60);
	float fAccumulator = 0;

public:
	PGE()
	{
		sAppName = "OrbBox2";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		GameObject* test_object = game_manager.create_game_object("mouse_test_object");
		test_object->insert_component(std::make_shared<SimpleSpriteComponent>(0, test_object));
		test_object->insert_component(std::make_shared<FollowMouseComponent>(1, test_object));
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		fAccumulator += fElapsedTime;
		if (fAccumulator >= fRate) {
			fAccumulator = 0;

			std::list<GameObject>::iterator object_iterator = game_manager.game_objects.begin();
			std::list<GameObject>::iterator object_end = game_manager.game_objects.end();

			while (object_iterator != object_end) {
				std::vector<std::shared_ptr<GameComponent>>::iterator component_iterator = object_iterator->object_components.begin();
				std::vector<std::shared_ptr<GameComponent>>::iterator component_end = object_iterator->object_components.end();

				while (component_iterator != component_end) {
					component_iterator->get()->run();
					//std::cout << "running: " << object_iterator->name << " | " << component_iterator->get()->get_name() << std::endl;
					std::advance(component_iterator, 1);
				}

				std::advance(object_iterator, 1);
			}
		}
		
		return true;
	}
};

int main()
{
	PGE game;
	if (game.Construct(1280, 720, 1, 1))
		game.Start();

	return 0;
}

//TESTING
void SimpleSpriteComponent::draw(){
	PGE* olc_pge = parent->game_manager->olc_pge;
	olc_pge->DrawSprite(parent->screen_position.x, parent->screen_position.y, &simple_sprite, 1);
}

void FollowMouseComponent::move() {
	PGE* olc_pge = parent->game_manager->olc_pge;
	parent->screen_position = ivector(olc_pge->GetMouseX(), olc_pge->GetMouseY());
}