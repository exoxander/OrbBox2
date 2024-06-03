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
void MouseFollowModule::set_position_to_mouse(GameObjectManager* _game_manager) {
	ivector mouse_position = ivector(_game_manager->internal_pge->GetMouseX(), _game_manager->internal_pge->GetMouseY());
	_game_manager->get_game_object(get_parent_uid())->screen_position = mouse_position;
};

void DrawQuadLineage::draw_all_quad_parents(GameObjectManager* _game_manager) {
	olc::PixelGameEngine* local_pge = _game_manager->internal_pge;
	//void DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, olc::Pixel p = olc::WHITE)
	Quad* current_quad = _game_manager->get_object_container(get_parent_uid())->tree_owner;

	//loop until top level (0) reached
	do {
		//draw rect using center and manhattan radius
	}while (current_quad->level > 0);
}