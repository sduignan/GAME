#pragma once

#include "pack_member.h"
#include "wolf.h"

class pack
{
public:
	pack_member *alpha, *_new;
	int pack_size;
	int max_size;

	pack();
	pack(int _size);
	~pack();

	bool move(int *grid, int size, int red_row, int red_col);
	void draw(wolf big_bad);

	void kill_wolf(int _row, int _col);
};
