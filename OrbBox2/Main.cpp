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
		test_object->insert_component(new OnClickDebugDraw(2, test_object));
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
	olc_pge->DrawSprite(parent->screen_position.x - (width / 2), parent->screen_position.y - (height / 2), &simple_sprite, 1);
	if (parent->game_manager->game_options.object_name_debug_draw) {
		olc_pge->DrawString(parent->screen_position.x, parent->screen_position.y - (height / 2), parent->name, olc::WHITE);
	}
}

void FollowMouseComponent::move() {
	PGE* olc_pge = parent->game_manager->olc_pge;
	parent->screen_position = ivector(olc_pge->GetMouseX(), olc_pge->GetMouseY());
}

void OnClickDebugDraw::on_frame() {
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