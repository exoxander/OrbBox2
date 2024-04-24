#pragma once
#include <string.h>
#include "World.h"

/*
* INTEGERS
* BOOLEAN (integers of 0 and 1)
* FLOATS
* STRINGS
* 
<save>
	<save_header>
		[save_name:default_world]
		[allow_overwrite:0]
		<save_settings>
		</save_settings>
	</save_header>
	<game_object_list>
		<game_object>
			[world_position_x:105.884]
			[world_position_y:-345.4355]
			[meta:0]
			<game_object_modules>
				<physics_module>
					[mass:283472.234234]
					[velocity_x:3.234]
					[velocity_y:-23.3444]
						<circle_collider>
							[radius:1.0]
							[is_trigger:0]
						</circle_collider>
				</physics_module>
			</game_object_modules>
		</game_object>
	</game_object_list>
</save>




*/
class MarkupInterface
{
protected:
	//use string snipping between special characters into a switch case for setting all values
	GameObject create_game_object_from_read();
	//check module type and use smaller specialized functions for each?
	GameModule create_game_module_from_read();

	void write_game_object_data(int depth, GameObject& _object);
	void write_game_module_data(int depth, GameModule& _module);
	void write_save_data();
	void read_save_data();
public:
	void markup_write(char* path);
	void markup_read_as_data(char* path);
};

