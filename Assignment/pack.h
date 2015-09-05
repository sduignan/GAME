#pragma once

#include <irrKlang/irrKlang.h> // for sound
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
using namespace irrklang;

#include "pack_member.h"
#include "wolf.h"

class pack
{
public:
	pack_member *alpha, *_new;
	int pack_size;
	int max_size;

	// sound engine
	ISoundEngine* engine;

	ISoundSource* birth;
	ISoundSource* death;

	pack();
	pack(int _size);
	~pack();

	void voice(ISoundEngine* _engine);
	bool move(int *grid, int size, int red_row, int red_col);
	void draw(wolf big_bad);

	void kill_wolf(int _row, int _col);
};
