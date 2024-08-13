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
		test_object->insert_component(new SimpleSpriteComponent(0, test_object,64,64));
		test_object->insert_component(new FollowMouseComponent(1, test_object));
		test_object->insert_component(new OnMouseDownDebugDraw(2, test_object));

		//mass objects
		GameObject* m1 = game_manager.create_game_object("mass_1");
		GameObject* m2 = game_manager.create_game_object("mass_2");
		GameObject* m3 = game_manager.create_game_object("mass_3");

		m1->insert_component(new DisplayComponent(0, m1));
		m1->insert_component(new PhysicsComponent(1, m1, 1000000, fvector(200, 200),fvector(0,0)));
		m1->object_components[1]->on_create();

		m2->insert_component(new DisplayComponent(0, m2));
		m2->insert_component(new PhysicsComponent(1, m2, 100000, fvector(400, 400),fvector(0,-.2)));
		m2->object_components[1]->on_create();

		m3->insert_component(new DisplayComponent(0, m3));
		m3->insert_component(new PhysicsComponent(1, m3, 10000000, fvector(150, 200), fvector(0,0)));
		m3->object_components[1]->on_create();

		return true;
	}

	//=============================[ UPDATE ]============================
	bool OnUserUpdate(float fElapsedTime) override
	{

		fAccumulator += fElapsedTime;
		if (fAccumulator >= fRate) {
			fAccumulator = 0;

			//draw background
			for (int x = 0; x < ScreenWidth(); x++)
				for (int y = 0; y < ScreenHeight(); y++)
					Draw(x, y, olc::Pixel(60,60,60));

			//compute all on frame components
			std::list<GameObject>::iterator object_iterator = game_manager.game_objects.begin();
			std::list<GameObject>::iterator object_end = game_manager.game_objects.end();
			while (object_iterator != object_end) {
				std::vector<GameComponent*>::iterator component_iterator = object_iterator->object_components.begin();
				std::vector<GameComponent*>::iterator component_end = object_iterator->object_components.end();

				while (component_iterator != component_end) {
					GameComponent* gc = *component_iterator;
					gc->on_frame();
					//std::cout << "running: " << object_iterator->name << " | " << component_iterator->get()->get_name() << std::endl;
					std::advance(component_iterator, 1);
				}

				std::advance(object_iterator, 1);
			}

			game_manager.step();
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

//================================[ Game Components ]==================================

void DisplayComponent::draw() {
	PGE* olc_pge = parent->game_manager->olc_pge;
	olc_pge->DrawCircle(parent->screen_position.x, parent->screen_position.y, 5, olc::CYAN);
	if (parent->game_manager->game_options.object_name_debug_draw) {
		olc_pge->DrawString(parent->screen_position.x, parent->screen_position.y, parent->name, olc::WHITE);
	}
}

void SimpleSpriteComponent::draw(){
	PGE* olc_pge = parent->game_manager->olc_pge;
	olc_pge->DrawSprite(parent->screen_position.x - (width / 2), parent->screen_position.y - (height / 2), &simple_sprite, 1);
	if (parent->game_manager->game_options.object_name_debug_draw) {
		olc_pge->DrawString(parent->screen_position.x, parent->screen_position.y - (height / 2), parent->name, olc::WHITE);
	}
}

void FollowMouseComponent::on_frame() {
	PGE* olc_pge = parent->game_manager->olc_pge;
	parent->screen_position = ivector(olc_pge->GetMouseX(), olc_pge->GetMouseY());
}

void OnMouseDownDebugDraw::on_frame() {
	PGE* olc_pge = parent->game_manager->olc_pge;
	if (olc_pge->GetMouse(0).bHeld) {
		if (!is_active) {
			is_active = true;
			parent->game_manager->game_options.object_name_debug_draw = true;
		}
	}
	else {
		if (is_active) {
			is_active = false;
			parent->game_manager->game_options.object_name_debug_draw = false;
		}
	}
}

void PhysicsComponent::register_component() {
	if (parent != nullptr) {
		if (parent->game_manager != nullptr) {
			parent->game_manager->physics_components.push_back(this);
		}
	}
}