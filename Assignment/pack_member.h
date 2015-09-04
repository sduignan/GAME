#pragma once
#include "Antons_maths_funcs.h"

class pack_member
{
public:
	vec3 foot_offsets[4];
	float foot_rotations[4];

	int row;
	int col;
	vec3 offset;
	char facing_dir;
	float turn_angle;
	float turn_increment;
	float angle_turned;
	
	pack_member *next;

	pack_member(int _row, int _col);
	pack_member();

	void turn(char dir);

	bool move(int *grid, int size, int red_row, int red_col);
	bool in_scope(int red_row, int red_col);
	vec3 get_loc();
};
