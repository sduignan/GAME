#pragma once

#include "scene_object.h"

#define BALL		0
#define TAIL		1
#define FOOT		2

class wolf
{
public:
	scene_object parts[3];
	mat4 base_model, proj, view;
	int M_loc, P_loc, V_loc;
	
	vec3 wolf_feet[4];

	vec3 wolf_tail_loc;
		
	wolf(char** mesh_name, char** tex_name);

	void draw();
	
	void set_mat_locs(int in_M_loc, int in_P_loc, int in_V_loc);
	void update_mats(bool M, bool P, bool V);
};