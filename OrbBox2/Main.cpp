#define OLC_PGE_APPLICATION
#include "world.cpp"

//https://github.com/OneLoneCoder/olcPixelGameEngine/wiki/Multiple-File-Projects

//https://github.com/OneLoneCoder/olcPixelGameEngine/wiki/olc::PixelGameEngine-Documentation




class PGE : public olc::PixelGameEngine {
public:
	PGE()
	{
		sAppName = "OrbBox2";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
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

//===========================================< TESTING JUNK >=====================================

//on every activation call the function and data via the parent-game manager link
void MouseFollowModule::always_activate() {
	set_position_to_mouse(get_parent_ptr()->get_manager_ptr());
}

//every activation set the parent objects screen position to that of the mouse cursor
void MouseFollowModule::set_position_to_mouse(GameObjectManager* _game_manager) {
	ivector mouse_position = ivector(_game_manager->internal_pge->GetMouseX(), _game_manager->internal_pge->GetMouseY());
	_game_manager->get_game_object(get_parent_ptr()->get_uid())->screen_position = mouse_position;
};

//call the draw quad function
void DrawQuadLineage::always_activate() {
	draw_all_quad_parents(get_parent_ptr()->get_manager_ptr());
};

//draw squares showing the quad tree lineage path for the applicable object
void DrawQuadLineage::draw_all_quad_parents(GameObjectManager* _game_manager) {
	olc::PixelGameEngine* local_pge = _game_manager->internal_pge;
	Quad* current_quad = _game_manager->get_object_container(get_parent_ptr()->get_uid())->tree_owner;
	int norm_radius = 0;
	ivector norm_coord = ivector();

	//check if outside world, draw red square showing world if true
	if (current_quad == nullptr) {
		current_quad = _game_manager->world_tree.get_top_level_quad();
		norm_radius = static_cast<int>(current_quad->manhattan_radius);
		norm_coord = ivector(current_quad->center);
		_game_manager->internal_pge->DrawRect(norm_coord.x - norm_radius, norm_coord.y - norm_radius, 2 * norm_radius, 2 * norm_radius, olc::RED);
	}
	else {
		while (current_quad != nullptr) {
			//void DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, olc::Pixel p = olc::WHITE)
			//draw rect using center and manhattan radius
			norm_radius = static_cast<int>(current_quad->manhattan_radius);
			norm_coord = ivector(current_quad->center);
			_game_manager->internal_pge->DrawRect(norm_coord.x - norm_radius, norm_coord.y - norm_radius, 2 * norm_radius, 2 * norm_radius);
			current_quad = current_quad->parent;
		}
	}
	}