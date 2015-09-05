/*
GAME!

WASD to turn, up to go in the direction you're facing, space to attack
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include "Antons_maths_funcs.h"
#include "obj_parser.h"
#include <fstream>
#include <irrKlang/irrKlang.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define STBI_ASSERT(x)

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

#include "scene_object.h"
#include "red.h"
#include "wolf.h"
#include "pack_member.h"
#include "pack.h"

using namespace irrklang;

// sound engine
ISoundEngine* engine;
// fade the sound (song)?
bool fade = false;
float fade_factor = 1.0;

// dimensions of the window drawing surface
int gl_width = 600;
int gl_height = 600;

bool reset = false;

bool show_start_screen = true;
bool show_level_screen = false;
bool show_lose_screen = false;
bool show_win_screen = false;

bool close_window = false;

//Camera variables
float V_trans_x = 0.0;
float V_trans_y = 0.0;
float V_trans_z = 0.0;
float V_rot_x = 0.0;
float V_rot_y = 0.0;
float V_rot_z = 0.0;


GLfloat ground_points[] = {
	 250.0f, 0.0f, 250.0f,
	 250.0f, 0.0f, -250.0f,
	-250.0f, 0.0f, -250.0f,
	-250.0f, 0.0f, -250.0f,
	-250.0f, 0.0f, 250.0f,
	 250.0f, 0.0f, 250.0f
};

GLfloat ground_normals[] = {
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};


GLfloat ground_texcoords[] = {
	0.0f,	 0.0f,
	500.0f,  0.0f,
	500.0f,  500.0f,
	500.0f,  500.0f,
	0.0f,	 500.0f,
	0.0f,	 0.0f
};

GLfloat fullscreen_points[] = {
	 1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f
};

GLfloat fullscreen_normals[] = {
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};


GLfloat fullscreen_texcoords[] = {
	1.0f,	1.0f,
	1.0f,	0.0f,
	0.0f,	0.0f,
	0.0f,	0.0f,
	0.0f,	1.0f,
	1.0f,	1.0f
};

int grid_size = 10;
int maze_grid_size;
char maze_in_use = 'd';	//d-Default, s-Small, l-Large

int s_maze_grid[33][33];
int m_maze_grid[63][63];
int l_maze_grid[93][93];
int grid[10][10] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
				    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
				    {1, 2, 0, 0, 0, 0, 0, 0, 0, 1},
				    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
				    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
				    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
				    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
				    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
				    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
				    {1, 0, 0, 0, 0, 0, 0, 0, -1, 1}};

int red_row = 2;
int red_col = 1;

float axe_rot_1 = 0.0;
float axe_rot_2 = 0.0;
float axe_rot_3 = 0.0;

float hand_offset = 0.0;

bool hit = false;

vec3 reds_location = vec3 (0.0, -0.75, -2);
vec3 move_offset = vec3 (0.0, 0.0, 0.0);
bool turn_red = false;
char turn_red_dir = 'S';
bool move_red = false;
int red_lives = 3;
bool red_win = false;

pack gang(3);

vec3 wolf_loc = vec3 (-2.0, -0.75, -4);
bool move_wolf = false;

//vec3 shroom_locs[] = {vec3 (0.0, -0.75, 0.0), vec3 (1.0, -0.75, -0.5), vec3 (1.5, -0.75, 1.75), vec3 (-1, -0.75, -1.0)};
//int num_shrooms = 4;

int num_shrooms = 150;
vec3 shroom_locs[150];


char* reds_meshes[] = { "Meshes/red_face.obj",
						"Meshes/red_hood.obj",
						"Meshes/red_cloak.obj",
						"Meshes/red_hand.obj",
						"Meshes/axe_head.obj",
						"Meshes/axe_handle.obj"};
char* reds_texs[] = {"Textures/red_face.png",
					 "Textures/red_hood.png",
					 "Textures/red_cloak.png",
					 "Textures/red_hand.png",
					 "Textures/axe_head.png",
					 "Textures/axe_handle.png"};

char* wolf_meshes[] ={  "Meshes/wolf_ball.obj",
						"Meshes/wolf_tail.obj",
						"Meshes/wolf_foot.obj"};

char* wolf_texs[] ={"Textures/wolf_ball.png",
					"Textures/wolf_tail.png",
					"Textures/wolf_foot.png"};



void load_maze(char size){	
	red_lives = 3;
	red_win = false;

	std::cout << "\nINFUNC\n";
	char* filename = "";
	if(size=='s'){
		maze_grid_size=33;
		filename = "Mazes/small1.txt";
	}
	if(size=='m'){
		maze_grid_size=63;
		filename = "Mazes/med1.txt";
	}
	if(size=='l'){
		maze_grid_size=93;
		filename = "Mazes/big1.txt";
	}
	char ch;
	int col = 0;
	int row = 0;
	std::fstream fin(filename, std::fstream::in);
	while (fin >> ch) {
		int val = ch - '0';
		if(val==5)
			val = -1;
		if(size=='s')
			s_maze_grid[col][row] = val;
		if(size=='m')
			m_maze_grid[col][row] = val;
		if(size=='l')
			l_maze_grid[col][row] = val;
		row++;
		if(row==maze_grid_size){
			row = 0;
			col++;
		}
	}
}


//keyboard control
void My_Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		close_window = true;
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS){
		if(show_start_screen){
			show_level_screen = true;
			show_start_screen = false;
		}
		if(show_win_screen){
			show_level_screen = true;
			show_win_screen = false;
		}
		if(show_lose_screen){
			show_level_screen = true;
			show_lose_screen = false;
		}
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS){
		V_trans_x += 0.05;
		turn_red = true;
		turn_red_dir = 'W';
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS){
		V_trans_x -= 0.05;
		turn_red = true;
		turn_red_dir = 'E';
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS){
		V_trans_y -= 0.05;
		turn_red = true;
		turn_red_dir = 'N';
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS){
		if(show_level_screen){
			load_maze('s');
			maze_in_use = 's'; 
			show_level_screen = false;
			fade = true;
		}
		else{
			V_trans_y += 0.05;
			turn_red = true;
			turn_red_dir = 'S';
		}
	}
	if (key == GLFW_KEY_Q && action == GLFW_REPEAT){
		V_trans_z += 0.05;
	}
	if (key == GLFW_KEY_E && action == GLFW_REPEAT){
		V_trans_z -= 0.05;
	}

	if (key == GLFW_KEY_K && action == GLFW_REPEAT){
		V_rot_x += 2;
	}
	if (key == GLFW_KEY_I && action == GLFW_REPEAT){
		V_rot_x -= 2;
	}
	if (key == GLFW_KEY_J && action == GLFW_REPEAT){
		V_rot_y += 2;
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS){
		if(show_level_screen){
			load_maze('l');
			maze_in_use = 'l'; 
			show_level_screen = false;
			fade = true;
		}
		V_rot_y -= 2;
	}
	if (key == GLFW_KEY_U && action == GLFW_REPEAT){
		V_rot_z += 2;
	}
	if (key == GLFW_KEY_O && action == GLFW_REPEAT){
		V_rot_z -= 2;
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS){
		if(show_level_screen){
			load_maze('m');
			maze_in_use = 'm'; 
			show_level_screen = false;
			fade = true;
		}
	}


	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
		hit = true;
		move_wolf = true;
		if(maze_in_use=='d'){
			if((turn_red_dir=='N')&&(red_row-1>0)&&(grid[red_row-1][red_col]==3)){
				gang.kill_wolf(red_row-1, red_col);
				grid[red_row-1][red_col] = 0;
			}
			if(turn_red_dir=='S'&&(red_row+1<grid_size)&&(grid[red_row+1][red_col]==3)){
				gang.kill_wolf(red_row+1, red_col);
				grid[red_row+1][red_col] = 0;
			}
			if((turn_red_dir=='E')&&(red_col+1<grid_size)&&(grid[red_row][red_col+1]==3)){
				gang.kill_wolf(red_row, red_col+1);
				grid[red_row][red_col+1] = 0;
			}
			if((turn_red_dir=='W')&&(red_col-1>0)&&(grid[red_row][red_col-1]==3)){
				gang.kill_wolf(red_row, red_col-1);
				grid[red_row][red_col-1] = 0;
			}
		}
		if(maze_in_use=='s'){
			if((turn_red_dir=='N')&&(red_row-1>0)&&(s_maze_grid[red_row-1][red_col]==3)){
				gang.kill_wolf(red_row-1, red_col);
				s_maze_grid[red_row-1][red_col] = 0;
			}
			if(turn_red_dir=='S'&&(red_row+1<maze_grid_size)&&(s_maze_grid[red_row+1][red_col]==3)){
				gang.kill_wolf(red_row+1, red_col);
				s_maze_grid[red_row+1][red_col] = 0;
			}
			if((turn_red_dir=='E')&&(red_col+1<maze_grid_size)&&(s_maze_grid[red_row][red_col+1]==3)){
				gang.kill_wolf(red_row, red_col+1);
				s_maze_grid[red_row][red_col+1] = 0;
			}
			if((turn_red_dir=='W')&&(red_col-1>0)&&(s_maze_grid[red_row][red_col-1]==3)){
				gang.kill_wolf(red_row, red_col-1);
				s_maze_grid[red_row][red_col-1] = 0;
			}
		}
		if(maze_in_use=='m'){
			if((turn_red_dir=='N')&&(red_row-1>0)&&(m_maze_grid[red_row-1][red_col]==3)){
				gang.kill_wolf(red_row-1, red_col);
				m_maze_grid[red_row-1][red_col] = 0;
			}
			if(turn_red_dir=='S'&&(red_row+1<maze_grid_size)&&(m_maze_grid[red_row+1][red_col]==3)){
				gang.kill_wolf(red_row+1, red_col);
				m_maze_grid[red_row+1][red_col] = 0;
			}
			if((turn_red_dir=='E')&&(red_col+1<maze_grid_size)&&(m_maze_grid[red_row][red_col+1]==3)){
				gang.kill_wolf(red_row, red_col+1);
				m_maze_grid[red_row][red_col+1] = 0;
			}
			if((turn_red_dir=='W')&&(red_col-1>0)&&(m_maze_grid[red_row][red_col-1]==3)){
				gang.kill_wolf(red_row, red_col-1);
				m_maze_grid[red_row][red_col-1] = 0;
			}
		}
		if(maze_in_use=='l'){
			if((turn_red_dir=='N')&&(red_row-1>0)&&(l_maze_grid[red_row-1][red_col]==3)){
				gang.kill_wolf(red_row-1, red_col);
				l_maze_grid[red_row-1][red_col] = 0;
			}
			if(turn_red_dir=='S'&&(red_row+1<maze_grid_size)&&(l_maze_grid[red_row+1][red_col]==3)){
				gang.kill_wolf(red_row+1, red_col);
				l_maze_grid[red_row+1][red_col] = 0;
			}
			if((turn_red_dir=='E')&&(red_col+1<maze_grid_size)&&(l_maze_grid[red_row][red_col+1]==3)){
				gang.kill_wolf(red_row, red_col+1);
				l_maze_grid[red_row][red_col+1] = 0;
			}
			if((turn_red_dir=='W')&&(red_col-1>0)&&(l_maze_grid[red_row][red_col-1]==3)){
				gang.kill_wolf(red_row, red_col-1);
				l_maze_grid[red_row][red_col-1] = 0;
			}
		}
	}

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS){
		std::cout << "\n\nGRID:\n" << V_trans_x;
		for(int i=0; i<maze_grid_size; i++){
			std::cout << "\n\t";
			for(int j=0; j<maze_grid_size; j++){
				if(maze_in_use=='s')
					std::cout << s_maze_grid[i][j];
				if(maze_in_use=='m')
					std::cout << m_maze_grid[i][j];
				if(maze_in_use=='l')
					std::cout << l_maze_grid[i][j];
			}
		}
		std::cout << "\n";
	}
	
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS){
		load_maze('s');
		maze_in_use = 's';
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS){
		if(!move_red){
			if(maze_in_use=='d'){
				if((turn_red_dir=='N')&&(red_row-1>=0)&&(grid[red_row-1][red_col]<=0)){
					if(grid[red_row-1][red_col]==-1)
						red_win = true;
					grid[red_row-1][red_col]=2;
					grid[red_row][red_col]=0;
					red_row--;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(0.0, 0.0, 10.0);
				}
				if(turn_red_dir=='S'&&(red_row+1<grid_size)&&(grid[red_row+1][red_col]<=0)){
					if(grid[red_row+1][red_col]==-1)
						red_win=true;
					grid[red_row+1][red_col]=2;
					grid[red_row][red_col]=0;
					red_row++;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(0.0, 0.0, -10.0);
				}
				if((turn_red_dir=='E')&&(red_col+1<grid_size)&&(grid[red_row][red_col+1]<=0)){
					if(grid[red_row][red_col+1]==-1)
						red_win=true;
					grid[red_row][red_col+1]=2;
					grid[red_row][red_col]=0;
					red_col++;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(-10.0, 0.0, 0.0);
				}
				if((turn_red_dir=='W')&&(red_col-1>=0)&&(grid[red_row][red_col-1]<=0)){
					if(grid[red_row][red_col-1]==-1)
						red_win = true;
					grid[red_row][red_col-1]=2;
					grid[red_row][red_col]=0;
					red_col--;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(10.0, 0.0, 0.0);
				}
			}
			if(maze_in_use=='s'){
				if((turn_red_dir=='N')&&(red_row-1>=0)&&(s_maze_grid[red_row-1][red_col]<=0)){
					if(s_maze_grid[red_row-1][red_col]==-1)
						red_win = true;
					s_maze_grid[red_row-1][red_col]=2;
					s_maze_grid[red_row][red_col]=0;
					red_row--;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(0.0, 0.0, 10.0);
				}
				if(turn_red_dir=='S'&&(red_row+1<maze_grid_size)&&(s_maze_grid[red_row+1][red_col]<=0)){
					if(s_maze_grid[red_row+1][red_col]==-1)
						red_win=true;
					s_maze_grid[red_row+1][red_col]=2;
					s_maze_grid[red_row][red_col]=0;
					red_row++;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(0.0, 0.0, -10.0);
				}
				if((turn_red_dir=='E')&&(red_col+1<maze_grid_size)&&(s_maze_grid[red_row][red_col+1]<=0)){
					if(s_maze_grid[red_row][red_col+1]==-1)
						red_win=true;
					s_maze_grid[red_row][red_col+1]=2;
					s_maze_grid[red_row][red_col]=0;
					red_col++;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(-10.0, 0.0, 0.0);
				}
				if((turn_red_dir=='W')&&(red_col-1>=0)&&(s_maze_grid[red_row][red_col-1]<=0)){
					if(s_maze_grid[red_row][red_col-1]==-1)
						red_win = true;
					s_maze_grid[red_row][red_col-1]=2;
					s_maze_grid[red_row][red_col]=0;
					red_col--;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(10.0, 0.0, 0.0);
				}
			}
			if(maze_in_use=='m'){
				if((turn_red_dir=='N')&&(red_row-1>=0)&&(m_maze_grid[red_row-1][red_col]<=0)){
					if(m_maze_grid[red_row-1][red_col]==-1)
						red_win = true;
					m_maze_grid[red_row-1][red_col]=2;
					m_maze_grid[red_row][red_col]=0;
					red_row--;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(0.0, 0.0, 10.0);
				}
				if(turn_red_dir=='S'&&(red_row+1<maze_grid_size)&&(m_maze_grid[red_row+1][red_col]<=0)){
					if(m_maze_grid[red_row+1][red_col]==-1)
						red_win=true;
					m_maze_grid[red_row+1][red_col]=2;
					m_maze_grid[red_row][red_col]=0;
					red_row++;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(0.0, 0.0, -10.0);
				}
				if((turn_red_dir=='E')&&(red_col+1<maze_grid_size)&&(m_maze_grid[red_row][red_col+1]<=0)){
					if(m_maze_grid[red_row][red_col+1]==-1)
						red_win=true;
					m_maze_grid[red_row][red_col+1]=2;
					m_maze_grid[red_row][red_col]=0;
					red_col++;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(-10.0, 0.0, 0.0);
				}
				if((turn_red_dir=='W')&&(red_col-1>=0)&&(m_maze_grid[red_row][red_col-1]<=0)){
					if(m_maze_grid[red_row][red_col-1]==-1)
						red_win = true;
					m_maze_grid[red_row][red_col-1]=2;
					m_maze_grid[red_row][red_col]=0;
					red_col--;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(10.0, 0.0, 0.0);
				}
			}
			if(maze_in_use=='l'){
				if((turn_red_dir=='N')&&(red_row-1>=0)&&(l_maze_grid[red_row-1][red_col]<=0)){
					if(l_maze_grid[red_row-1][red_col]==-1)
						red_win = true;
					l_maze_grid[red_row-1][red_col]=2;
					l_maze_grid[red_row][red_col]=0;
					red_row--;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(0.0, 0.0, 10.0);
				}
				if(turn_red_dir=='S'&&(red_row+1<maze_grid_size)&&(l_maze_grid[red_row+1][red_col]<=0)){
					if(l_maze_grid[red_row+1][red_col]==-1)
						red_win=true;
					l_maze_grid[red_row+1][red_col]=2;
					l_maze_grid[red_row][red_col]=0;
					red_row++;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(0.0, 0.0, -10.0);
				}
				if((turn_red_dir=='E')&&(red_col+1<maze_grid_size)&&(l_maze_grid[red_row][red_col+1]<=0)){
					if(l_maze_grid[red_row][red_col+1]==-1)
						red_win=true;
					l_maze_grid[red_row][red_col+1]=2;
					l_maze_grid[red_row][red_col]=0;
					red_col++;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(-10.0, 0.0, 0.0);
				}
				if((turn_red_dir=='W')&&(red_col-1>=0)&&(l_maze_grid[red_row][red_col-1]<=0)){
					if(l_maze_grid[red_row][red_col-1]==-1)
						red_win = true;
					l_maze_grid[red_row][red_col-1]=2;
					l_maze_grid[red_row][red_col]=0;
					red_col--;
					move_red = true;
					move_wolf = true;
					move_offset = vec3(10.0, 0.0, 0.0);
				}
			}
		}
	}
}


//
// copy a shader from a plain text file into a character array
bool parse_file_into_str (const char* file_name, char* shader_str, int max_len) {
	FILE* file = fopen (file_name , "r");
	int current_len = 0;
	char line[2048];

	shader_str[0] = '\0'; /* reset string */
	if (!file) {
		fprintf (stderr, "ERROR: opening file for reading: %s\n", file_name);
		return false;
	}
	strcpy (line, ""); /* remember to clean up before using for first time! */
	while (!feof (file)) {
		if (NULL != fgets (line, 2048, file)) {
			current_len += strlen (line); /* +1 for \n at end */
			if (current_len >= max_len) {
				fprintf (stderr, 
					"ERROR: shader length is longer than string buffer length %i\n",
					max_len
				);
			}
			strcat (shader_str, line);
		}
	}
	if (EOF == fclose (file)) { /* probably unnecesssary validation */
		fprintf (stderr, "ERROR: closing file from reading %s\n", file_name);
		return false;
	}
	return true;
}


int main () {
	GLFWwindow* window = NULL;
	const GLubyte* renderer;
	const GLubyte* version;
	GLuint shader_programme;
	GLuint simple_shader;

	// start the sound engine with default parameters
	engine = createIrrKlangDevice();

	if (!engine)
	{
		printf("Could not startup engine\n");
		return 0; // error starting up the engine
	}

	engine->setSoundVolume(0.6);

	ISoundSource* music = engine->addSoundSourceFromFile("Sound/lilred.mp3");
	ISoundSource* wolf_attack = engine->addSoundSourceFromFile("Sound/growl1.wav");
	ISoundSource* scream = engine->addSoundSourceFromFile("Sound/WilhelmScream.mp3");


	ISound* backing_track = engine->play2D(music, true, false, true);

	gang.voice(engine);

	//generate shroom locations
	for(int i=0; i<num_shrooms; i++){
		float shroom_x = (rand()%2500)*0.1;
		float shroom_z = (rand()%2500)*0.1;
		shroom_locs[i] = vec3 (shroom_x, -0.75, shroom_z);
	}

	//
	// Start OpenGL using helper libraries
	// --------------------------------------------------------------------------
	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWmonitor* mon = glfwGetPrimaryMonitor ();
	const GLFWvidmode* vmode = glfwGetVideoMode (mon);
	window = glfwCreateWindow (
	  vmode->width, vmode->height, "Extended GL Init", mon, NULL
	);
	gl_width = vmode->width;
	gl_height = vmode->height;
	if (!window) {
		fprintf (stderr, "ERROR: opening OS window\n");
		return 1;
	}
	glfwMakeContextCurrent (window);

	// Tell the window where to find its key callback function
	glfwSetKeyCallback(window, My_Key_Callback);

	glewExperimental = GL_TRUE;
	glewInit ();

	/* get version info */
	renderer = glGetString (GL_RENDERER); /* get renderer string */
	version = glGetString (GL_VERSION); /* version as a string */
	printf ("Renderer: %s\n", renderer);
	printf ("OpenGL version supported %s\n", version);

	//
	// Set up vertex buffers and vertex array objects
	// --------------------------------------------------------------------------

	
	scene_object ground(6, ground_points, ground_normals, ground_texcoords, "Textures/dirt.png");
	scene_object shroom("Meshes/mushroom.obj", "Textures/shroom.png");
	red lil_red(reds_meshes, reds_texs);
	scene_object tree("Meshes/tree.obj", "Textures/tree.png");
	wolf big_bad(wolf_meshes, wolf_texs);
	scene_object win_screen(6, fullscreen_points, fullscreen_normals, fullscreen_texcoords, "Textures/win_screen.png");
	scene_object lose_screen(6, fullscreen_points, fullscreen_normals, fullscreen_texcoords, "Textures/lose_screen.png");
	scene_object start_screen(6, fullscreen_points, fullscreen_normals, fullscreen_texcoords, "Textures/open_screen.png");
	scene_object level_screen(6, fullscreen_points, fullscreen_normals, fullscreen_texcoords, "Textures/level_select.png");
	scene_object granmas_house("Meshes/house.obj", "Textures/house.png");
	scene_object heart("Meshes/heart.obj", "Textures/heart.png");
	

	//
	// Load shaders from files
	// --------------------------------------------------------------------------
	{
		char* simple_vertex_shader_str;
		char* simple_fragment_shader_str;

		// allocate some memory to store shader strings
		simple_vertex_shader_str = (char*)malloc (81920);
		simple_fragment_shader_str = (char*)malloc (81920);
		// load shader strings from text files
		assert (parse_file_into_str ("simple_tex.vert", simple_vertex_shader_str, 81920));
		assert (parse_file_into_str ("simple_tex.frag", simple_fragment_shader_str, 81920));
		GLuint simple_vs, simple_fs;
		simple_vs = glCreateShader (GL_VERTEX_SHADER);
		simple_fs = glCreateShader (GL_FRAGMENT_SHADER);
		glShaderSource (simple_vs, 1, (const char**)&simple_vertex_shader_str, NULL);
		glShaderSource (simple_fs, 1, (const char**)&simple_fragment_shader_str, NULL);
		// free memory
		free (simple_vertex_shader_str);
		free (simple_fragment_shader_str);
		glCompileShader (simple_vs);
		glCompileShader (simple_fs);
		simple_shader = glCreateProgram ();
		glAttachShader (simple_shader, simple_fs);
		glAttachShader (simple_shader, simple_vs);
		glLinkProgram (simple_shader);
	}

	
	{
		char* vertex_shader_str;
		char* fragment_shader_str;

		// allocate some memory to store shader strings
		vertex_shader_str = (char*)malloc (81920);
		fragment_shader_str = (char*)malloc (81920);
		// load shader strings from text files
		assert (parse_file_into_str ("tex.vert", vertex_shader_str, 81920));
		assert (parse_file_into_str ("tex.frag", fragment_shader_str, 81920));
		GLuint vs, fs;
		vs = glCreateShader (GL_VERTEX_SHADER);
		fs = glCreateShader (GL_FRAGMENT_SHADER);
		glShaderSource (vs, 1, (const char**)&vertex_shader_str, NULL);
		glShaderSource (fs, 1, (const char**)&fragment_shader_str, NULL);
		// free memory
		free (vertex_shader_str);
		free (fragment_shader_str);
		glCompileShader (vs);
		glCompileShader (fs);
		shader_programme = glCreateProgram ();
		glAttachShader (shader_programme, fs);
		glAttachShader (shader_programme, vs);
		glLinkProgram (shader_programme);
	}

	//simple shader stuff

	glUseProgram (simple_shader);
	// location of "P" in vertex shader
	int simple_P_loc = glGetUniformLocation (simple_shader, "P");	
	assert (simple_P_loc > -1);
	// location of "V" in vertex shader
	int simple_V_loc = glGetUniformLocation (simple_shader, "V");
	assert (simple_V_loc > -1);
	// location of "M" in vertex shader
	int simple_M_loc = glGetUniformLocation (simple_shader, "M");
	assert (simple_M_loc > -1);
	
	
	heart.M_loc = simple_M_loc;
	heart.V_loc = simple_V_loc;
	heart.P_loc = simple_P_loc;


	//
	// Create some matrices
	// --------------------------------------------------------------------------
	// a model matrix
	mat4 M = identity_mat4();
	mat4 P = identity_mat4();
	mat4 V = identity_mat4();
	glUseProgram (shader_programme);
	// location of "P" in vertex shader
	int P_loc = glGetUniformLocation (shader_programme, "P");	
	assert (P_loc > -1);
	// location of "V" in vertex shader
	int V_loc = glGetUniformLocation (shader_programme, "V");
	assert (V_loc > -1);
	// location of "M" in vertex shader
	int M_loc = glGetUniformLocation (shader_programme, "M");
	assert (M_loc > -1);

	
	ground.M_loc = M_loc;
	ground.V_loc = V_loc;
	ground.P_loc = P_loc;
	
	shroom.M_loc = M_loc;
	shroom.V_loc = V_loc;
	shroom.P_loc = P_loc;
	
	lil_red.M_loc = M_loc;
	lil_red.V_loc = V_loc;
	lil_red.P_loc = P_loc;
	
	tree.M_loc = M_loc;
	tree.V_loc = V_loc;
	tree.P_loc = P_loc;
	
	big_bad.M_loc = M_loc;
	big_bad.V_loc = V_loc;
	big_bad.P_loc = P_loc;

	
	granmas_house.M_loc = M_loc;
	granmas_house.V_loc = V_loc;
	granmas_house.P_loc = P_loc;	



	//location of textures in shader
	int tex_location = glGetUniformLocation (shader_programme, "_texture");
	assert (tex_location > -1);
	glUniform1i (tex_location, 0);

	// location of "move_spotlight" in frag shader
	int move_spotlight_x_loc = glGetUniformLocation (shader_programme, "move_spotlight_x");
	//assert (move_spotlight_x_loc > -1);
	//glUniform1f(move_spotlight_x_loc, 0.0);
	int move_spotlight_z_loc = glGetUniformLocation (shader_programme, "move_spotlight_z");
	//assert (move_spotlight_z_loc > -1);
	glUniform1f(move_spotlight_z_loc, 0.0);

	// location of "M" in vertex shader
	int spot_mov = glGetUniformLocation (shader_programme, "Light_Mov");
	//assert (spot_mov > -1);

	glUseProgram (shader_programme);
	glUniformMatrix4fv (P_loc, 1, GL_FALSE, P.m);
	glUniformMatrix4fv (V_loc, 1, GL_FALSE, V.m);
	glUniformMatrix4fv (M_loc, 1, GL_FALSE, M.m);

	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CCW); // GL_CCW for counter clock-wise
	
	//
	// Start rendering
	// --------------------------------------------------------------------------
	// tell GL to only draw onto a pixel if the fragment is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor (0.0, 0.0, 0.0, 1.0); //BLACK

	while (!glfwWindowShouldClose (window)) {
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (show_level_screen||show_lose_screen||show_start_screen||show_win_screen) {
			glViewport (((gl_width*0.5)-(gl_height*0.5)), 0, gl_height, gl_height);
			glDisable (GL_CULL_FACE); // cull face
			glUseProgram (simple_shader);
			mat4 id = identity_mat4();
			glUniformMatrix4fv (simple_P_loc, 1, GL_FALSE, id.m);
			glUniformMatrix4fv (simple_V_loc, 1, GL_FALSE, id.m);
			glUniformMatrix4fv (simple_M_loc, 1, GL_FALSE, id.m);

			if(show_level_screen)
				level_screen.draw();
			else{
				if(show_lose_screen)
					lose_screen.draw();
				else{
					if(show_start_screen)
						start_screen.draw();
					else
						win_screen.draw();
				}
			}

		}

		else {
			glEnable (GL_CULL_FACE);
			glUseProgram (shader_programme);

			glUniform1f(move_spotlight_x_loc, reds_location.v[0]);
			glUniform1f(move_spotlight_z_loc, reds_location.v[2]-10);
		
			P =  perspective (66, 1, 0.1, 1000);
			glUniformMatrix4fv (P_loc, 1, GL_FALSE, P.m);
		
			glViewport (((gl_width*0.5)-(gl_height*0.5)), 0, gl_height, gl_height);

			//Moving the virtual camera

			V = translate (identity_mat4(), vec3 (-reds_location.v[0], -reds_location.v[1]-5.0, -reds_location.v[2]-5.0));
			V = rotate_x_deg(V, 45.0);

			if(hit)
				lil_red.hit = hit;
			hit = false;
			if(turn_red)
				lil_red.turn(turn_red_dir);
			turn_red = false;
			if (move_red){
				if(move_offset.v[0]<0.0){
					move_offset.v[0]+=1.0;
				}
				if(move_offset.v[0]>0.0){
					move_offset.v[0]-=1.0;
				}
				if(move_offset.v[2]<0.0){
					move_offset.v[2]+=1.0;
				}
				if(move_offset.v[2]>0.0){
					move_offset.v[2]-=1.0;
				}
				if((move_offset.v[0]==0.0)&&(move_offset.v[2]==0.0)){
					move_red = false;
				}
				if(red_win){
					show_win_screen = true;
					backing_track = engine->play2D(music, true, false, true);
				}
			}
			if(move_wolf){
				move_wolf = false;
				if(maze_in_use=='d'){
					if(gang.move((int *)grid, grid_size, red_row, red_col)){
						red_lives--;
						engine->play2D(wolf_attack);
						if(red_lives<=0){
							engine->play2D(scream);
							show_lose_screen = true;
							backing_track = engine->play2D(music, true, false, true);
						}
					}
				}
				if(maze_in_use=='s'){
					if(gang.move((int *)s_maze_grid, maze_grid_size, red_row, red_col)){
						red_lives--;
						engine->play2D(wolf_attack);
						if(red_lives<=0){
							engine->play2D(scream);
							show_lose_screen = true;
							backing_track = engine->play2D(music, true, false, true);
						}
					}
				}
				if(maze_in_use=='m'){
					if(gang.move((int *)m_maze_grid, maze_grid_size, red_row, red_col)){
						red_lives--;
						engine->play2D(wolf_attack);
						if(red_lives<=0){
							engine->play2D(scream);
							show_lose_screen = true;
							backing_track = engine->play2D(music, true, false, true);
						}
					}
				}
				if(maze_in_use=='l'){
					if(gang.move((int *)l_maze_grid, maze_grid_size, red_row, red_col)){
						red_lives--;
						engine->play2D(wolf_attack);
						if(red_lives<=0){
							engine->play2D(scream);
							show_lose_screen = true;
							backing_track = engine->play2D(music, true, false, true);
						}
					}
				}
				move_wolf = false;
			}

			//Ground
			{
				ground.model = translate (identity_mat4(), vec3 (0.0, -0.75, 0.0));
				ground.proj = P;
				ground.view = V;

				ground.update_mats(true, true, true);
				ground.draw();
			}

			//Drawing Trees
			mat4 tree_base = scale(identity_mat4(), vec3 (.8, .8, .8));

			if(maze_in_use=='d'){
				for(int col=0; col<grid_size; col++){
					for(int row=0; row<grid_size; row++){
						switch(grid[row][col]){
						case 1:
							if((row<(red_row+5))&&(row>(red_row-5))&&(col<(red_col+5))&&(col>(red_col-5))){
								tree.model = rotate_y_deg(tree_base, (row*75.0 + (col*-66.0)));
								tree.model = translate(tree.model, vec3((float(col)-2.0), -0.75, float(row)));
								tree.update_mats(true, false, false);
								tree.draw();
							}
							break;
						case 2:
							reds_location =  vec3((float(col)-2.0), -0.75, float(row));
							reds_location = reds_location + vec3(move_offset.v[0]/10, 0.0, move_offset.v[2]/10);
							lil_red.location = reds_location;
							lil_red.draw();
							break;
						case -1:
							if((row<(red_row+5))&&(row>(red_row-5))&&(col<(red_col+5))&&(col>(red_col-5))){
								granmas_house.model =  translate(identity_mat4(), vec3((float(col)-2.0), -0.75, float(row)));
								granmas_house.update_mats(true, false, false);
								granmas_house.draw();
							}
							break;
						}
					}
				}
			}
			if(maze_in_use=='s'||maze_in_use=='m'||maze_in_use=='l'){
				for(int col=0; col<maze_grid_size; col++){
					for(int row=0; row<maze_grid_size; row++){
						int val =0;
						if(maze_in_use=='s')
							val = s_maze_grid[row][col];
						if(maze_in_use=='m')
							val = m_maze_grid[row][col];
						if(maze_in_use=='l')
							val = l_maze_grid[row][col];
						switch(val){
						case 1:
							if((row<(red_row+5))&&(row>(red_row-5))&&(col<(red_col+5))&&(col>(red_col-5))){
								tree.model = rotate_y_deg(tree_base, (row*75.0 + (col*-66.0)));
								if((row>red_row)&&((col>red_col-2)&&(col<red_col+2))){
									tree.model = scale(tree_base, vec3 (0.5, 0.5, 0.5));
								}
								tree.model = translate(tree.model, vec3((float(col)-2.0), -0.75, float(row)));
								tree.update_mats(true, false, false);
								tree.draw();
							}
							break;
						case 2:
							red_col = col;
							red_row = row;
							reds_location =  vec3((float(col)-2.0), -0.75, float(row));
							reds_location = reds_location + vec3(move_offset.v[0]/10, 0.0, move_offset.v[2]/10);
							lil_red.location = reds_location;
							lil_red.draw();
							break;
						case -1:
							if((row<(red_row+10))&&(row>(red_row-10))&&(col<(red_col+10))&&(col>(red_col-10))){
								granmas_house.model = rotate_y_deg(identity_mat4(), 90);
								granmas_house.model = scale(granmas_house.model, vec3(3.0,3.0,3.0));
								granmas_house.model =  translate(granmas_house.model, vec3((float(col)-2.0), -0.75, float(row)-5.5));
								granmas_house.update_mats(true, false, false);
								glDisable (GL_CULL_FACE); // cull face
								granmas_house.draw();
								glEnable (GL_CULL_FACE); // cull face
							}
							break;
						}
					}
				}
			}
		
			glDisable (GL_CULL_FACE); // cull face
			gang.draw(big_bad);

		
			glEnable (GL_CULL_FACE); // cull face


			//Mushrooms	
			float shroom_2_scale = .66;
			float shroom_3_scale = .5;
			vec3 shroom_size = vec3 (.1, .1, .1);
			mat4 shroom_base = scale(identity_mat4(), shroom_size);
			mat4 shroom_2_base = scale(identity_mat4(), shroom_size*shroom_2_scale);
			mat4 shroom_3_base = scale(identity_mat4(), shroom_size*shroom_3_scale);
			vec3 shroom_offset = vec3 (-2.0, 0.0, 0.5);
			vec3 shroom_2_loc = vec3 (-0.1, 0.0, 0.1);
			vec3 shroom_3_loc = vec3 (-0.075, 0.0, 0.075);
			for(int i=0; i<num_shrooms; i++){
				int row = shroom_locs[i].v[2];
				int col = shroom_locs[i].v[0];
				if((row<(red_row+5))&&(row>(red_row-5))&&(col<(red_col+5))&&(col>(red_col-5))){
					std::cout << "\X: " << col << " Z: " << row << "\t";
					//if(row%3==0||col%2!=0){
						shroom.model = translate (shroom_base, shroom_locs[i]+shroom_offset);
						shroom.update_mats(true, false, false);
						shroom.draw();
					//}
					//if(col%2==0){
						shroom.model = translate (shroom_2_base, shroom_locs[i]+shroom_offset+shroom_2_loc);
						shroom.update_mats(true, false, false);
						shroom.draw();
					//}
					//if(col%2==row%3){
						shroom.model = translate (shroom_3_base, shroom_locs[i]+shroom_offset+shroom_3_loc);
						shroom.update_mats(true, false, false);
						shroom.draw();
					//}
				}

			}

			
			//Heart section
			glUseProgram (simple_shader);
			mat4 id = identity_mat4();
			float scaler = 1.2;
			float increment = ((2*(float)sin (glfwGetTime ()*(4.0/(float)red_lives)))*(2*(float)sin (glfwGetTime ()*(4.0/(float)red_lives))))/12;
			scaler += increment;
			M = scale(identity_mat4(), vec3 (scaler, scaler, scaler));
			M = translate (M, vec3 (0.0, -1.0, -2));
			P =  perspective (66, 1, 0.1, 1000);
			glUniformMatrix4fv (simple_P_loc, 1, GL_FALSE, P.m);
			glUniformMatrix4fv (simple_V_loc, 1, GL_FALSE, id.m);
			glUniformMatrix4fv (simple_M_loc, 1, GL_FALSE, M.m);
			int edge = 	(gl_width*0.5)-(gl_height*0.5);
			if(red_lives>0){
				glViewport (0, 0, edge/3, edge/3);
				heart.draw();
			}
			if(red_lives>1){
				glViewport (edge/3, 0, edge/3, edge/3);
				heart.draw();
			}
			if(red_lives>2){
				glViewport (edge*2/3, 0, edge/3, edge/3);
				heart.draw();
			}
			//End heart section

			//Music section
			if (fade) {
				if (backing_track) {
					backing_track->setVolume(fade_factor);
					fade_factor -= 0.002;
				}
				if (fade_factor < 0.0) {
					fade_factor = 1.0;
					fade = false;
					backing_track->drop();
				}
			}
		}

		// this just updates window events and keyboard input events
		glfwPollEvents ();
		glfwSwapBuffers (window);
		if(close_window){
			glfwDestroyWindow(window);
			if (backing_track) {
				backing_track->drop(); // drop backing track
			}
			engine->drop(); // delete sound engine
		}
	}

	return 0;
}
