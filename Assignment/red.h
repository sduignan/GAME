#pragma once

#include "scene_object.h"


#define FACE		0
#define HOOD		1
#define CLOAK		2
#define HAND		3
#define AXE_HEAD	4
#define AXE_HANDLE	5


#define OFFSET_AXE			0
#define OFFSET_HAND1		1
#define OFFSET_HAND2		2


class red
{
public:
	scene_object parts[6];
	mat4 base_model, proj, view;
	int M_loc, P_loc, V_loc;
	mat4 offset_model[3];
	vec3 location;

	char red_dir;
	float turn_angle;
	float turn_increment;
	float angle_turned;
	
	float axe_rot_1;
	float axe_rot_2;
	float axe_rot_3;

	float hand_offset;

	bool hit;
	
	red(char** mesh_name, char** tex_name);

	void turn(char dir);
	void move();

	void draw();
	
	void set_mat_locs(int in_M_loc, int in_P_loc, int in_V_loc);
	void update_mats(bool M, bool P, bool V);
};