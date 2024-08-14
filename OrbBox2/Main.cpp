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
		//test_object->insert_component(new SimpleSpriteComponent(0, test_object,64,64));
		test_object->insert_component(new BoxDrawComponent(0, test_object, 64, 32));
		test_object->insert_component(new FollowMouseComponent(1, test_object));
		test_object->insert_component(new OnMouseDownDebugDraw(2, test_object));

		//test button(s)
		GameObject* physics_toggle = game_manager.create_game_object("physics_toggle_button");
		physics_toggle->screen_position = ivector(100, 100);
		physics_toggle->insert_component(new BoxDrawComponent(0, physics_toggle, 64, 32));

		//this is convoluted, streamline in structure or with helpers later
		ButtonComponent* btn_comp = new ButtonComponent(1, physics_toggle, 64, 32, nullptr);
		ButtonAction* btn_act = new TogglePhysicsAction(btn_comp);
		btn_comp->set_action(btn_act);
		physics_toggle->insert_component(btn_comp);

		//mass objects
		GameObject* m1 = game_manager.create_game_object("mass_1");
		GameObject* m2 = game_manager.create_game_object("mass_2");
		GameObject* m3 = game_manager.create_game_object("mass_3");
		GameObject* m4 = game_manager.create_game_object("mass_4");

		m1->insert_component(new DisplayComponent(0, m1,5));
		m1->insert_component(new PhysicsComponent(1, m1, 100000000, fvector(200, 200),fvector(0,5)));
		m1->object_components[1]->on_create();

		m2->insert_component(new DisplayComponent(0, m2,10));
		m2->insert_component(new PhysicsComponent(1, m2, 1000000000, fvector(800, 400),fvector(0,-.5)));
		m2->object_components[1]->on_create();

		m3->insert_component(new DisplayComponent(0, m3,10));
		m3->insert_component(new PhysicsComponent(1, m3, 1000000000, fvector(150, 200), fvector(1,0)));
		m3->object_components[1]->on_create();

		m4->insert_component(new DisplayComponent(0, m4,5));
		m4->insert_component(new PhysicsComponent(1, m4, 10000000, fvector(850, 400), fvector(0, -3)));
		m4->object_components[1]->on_create();

		return true;
	}

	//=============================[ UPDATE ]============================
	bool OnUserUpdate(float fElapsedTime) override
	{

		fAccumulator += fElapsedTime;
		if (fAccumulator >= fRate) {
			fAccumulator = 0;
			//move camera
			game_manager.view.move_view(game_manager.olc_pge);
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
					std::advance(component_iterator, 1);
				}
				std::advance(object_iterator, 1);
			}

			//on step
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

//==============================[ Game Manager Camera ]================================
void Camera::move_view(PGE* _pge_ptr) {
	if (_pge_ptr->GetKey(olc::UP).bHeld) {
		view_offset.y += offset_move_speed;
	}
	else if (_pge_ptr->GetKey(olc::DOWN).bHeld) {
		view_offset.y -= offset_move_speed;
	}

	if (_pge_ptr->GetKey(olc::LEFT).bHeld) {
		view_offset.x += offset_move_speed;
	}
	else if (_pge_ptr->GetKey(olc::RIGHT).bHeld) {
		view_offset.x -= offset_move_speed;
	}
}
//================================[ Game Components ]==================================

void DisplayComponent::draw() {
	PGE* olc_pge = parent->game_manager->olc_pge;
	olc_pge->DrawCircle(parent->screen_position.x, parent->screen_position.y, meta, olc::CYAN);
	if (parent->game_manager->game_options.object_name_debug_draw) {
		olc_pge->DrawString(parent->screen_position.x, parent->screen_position.y, parent->name, olc::WHITE);
	}
}

void BoxDrawComponent::draw() {
	PGE* olc_pge = parent->game_manager->olc_pge;
	olc_pge->DrawRect(parent->screen_position.x - (width/2), parent->screen_position.y - (height/2), width, height);
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
	if (olc_pge->GetMouse(0).bPressed) {

		//gross, but effective
		DisplayComponent* d = dynamic_cast<DisplayComponent*>(parent->get_component_by_name("simple_sprite_component"));
		if(d != nullptr) {			
			if (!is_active) {
				is_active = true;
				d->show = true;
			}
			else {
				is_active = false;
				d->show = false;
			}
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

GameComponent* GameObject::get_component_by_name(const char* _name) {
	std::vector<GameComponent*>::iterator it = object_components.begin();
	std::vector<GameComponent*>::iterator end = object_components.end();
	while (it != end) {
		GameComponent* gc = *it;
		if (gc->get_name() == _name) {
			return gc;
		}
		std::advance(it, 1);
	}

	return nullptr;
}

void TogglePhysicsAction::act() {
	if (is_active) {
		btn_owner->get_parent()->game_manager->game_options.pause_physics = false;
	}
	else {
		btn_owner->get_parent()->game_manager->game_options.pause_physics = true;
	}
	is_active = !is_active;
}

bool ButtonComponent::check_clicked() {
	PGE* olc_pge = parent->game_manager->olc_pge;
	if (olc_pge->GetMouse(0).bPressed) {
		int half_w = width / 2;
		int half_h = height / 2;
		ivector mouse_pos = ivector(olc_pge->GetMouseX(), olc_pge->GetMouseY());

		//if in x range
		if (mouse_pos.x > parent->screen_position.x - half_w && mouse_pos.x < parent->screen_position.x + half_w) {
			//std::cout << "click in x range" << std::endl;
			//if in y range
			if (mouse_pos.y > parent->screen_position.y - half_h && mouse_pos.y < parent->screen_position.y + half_h) {
				//std::cout << "button click registered" << std::endl;
				return true;
			}
		}
	}
	return false;
}