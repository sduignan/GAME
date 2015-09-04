#include "wolf.h"

wolf::wolf(char** mesh_name, char** tex_name){
	wolf_feet[0] = vec3 (-0.4, 0.0,  0.25);
	wolf_feet[1] = vec3 (-0.4, 0.0, -0.25);
	wolf_feet[2] = vec3 ( 0.3, 0.0,  0.3);
	wolf_feet[3] = vec3 ( 0.3, 0.0, -0.3);

	wolf_tail_loc = vec3 (0.5, 0.4, 0.0);
		
	scene_object ball(mesh_name[BALL], tex_name[BALL]);
	scene_object tail(mesh_name[TAIL], tex_name[TAIL]);
	scene_object foot(mesh_name[FOOT], tex_name[FOOT]);

	parts[BALL] = ball;
	parts[TAIL] = tail;
	parts[FOOT] = foot;
}
void wolf::set_mat_locs(int in_M_loc, int in_P_loc, int in_V_loc){
	M_loc = in_M_loc;
	P_loc = in_P_loc;
	V_loc = in_V_loc;
}

void wolf::update_mats(bool M, bool P, bool V){
	if(M)
		glUniformMatrix4fv (M_loc, 1, GL_FALSE, base_model.m);
	if(P)
		glUniformMatrix4fv (P_loc, 1, GL_FALSE, proj.m);
	if(V)
		glUniformMatrix4fv (V_loc, 1, GL_FALSE, view.m);
}

void wolf::draw(){
	glUniformMatrix4fv (M_loc, 1, GL_FALSE, base_model.m);

	parts[BALL].draw();

	mat4 offset = translate (identity_mat4(), wolf_tail_loc);
	offset = offset*base_model;

	glUniformMatrix4fv (M_loc, 1, GL_FALSE, offset.m);

	parts[TAIL].draw();

	for(int i=0; i<4; i++){
		offset = translate (identity_mat4(), wolf_feet[i]);
		offset = offset*base_model;

		glUniformMatrix4fv (M_loc, 1, GL_FALSE, offset.m);

		parts[FOOT].draw();
	}
}