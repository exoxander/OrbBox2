#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

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