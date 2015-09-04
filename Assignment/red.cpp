#include "red.h"

red::red(char** mesh_name, char** tex_name){
	
	red_dir = 'S';
	turn_angle = 0.0;
	turn_increment = 9.0;
	angle_turned = 0;

	location = vec3 (0.0, 0.0, 0.0);

	axe_rot_1 = 0.0;
	axe_rot_2 = 0.0;
	axe_rot_3 = 0.0;

	hand_offset = 0.0;

	hit = false;
	
	scene_object face(mesh_name[FACE], tex_name[FACE]);
	scene_object hood(mesh_name[HOOD], tex_name[HOOD]);
	scene_object cloak(mesh_name[CLOAK], tex_name[CLOAK]);
	scene_object hand(mesh_name[HAND], tex_name[HAND]);
	scene_object axe_head(mesh_name[AXE_HEAD], tex_name[AXE_HEAD]);
	scene_object axe_handle(mesh_name[AXE_HANDLE], tex_name[AXE_HANDLE]);

	parts[FACE] = face;
	parts[HOOD] = hood;
	parts[CLOAK] = cloak;
	parts[HAND] = hand;
	parts[AXE_HEAD] = axe_head;
	parts[AXE_HANDLE] = axe_handle;
}



void red::set_mat_locs(int in_M_loc, int in_P_loc, int in_V_loc){
	M_loc = in_M_loc;
	P_loc = in_P_loc;
	V_loc = in_V_loc;
}

void red::update_mats(bool M, bool P, bool V){
	if(M)
		glUniformMatrix4fv (M_loc, 1, GL_FALSE, base_model.m);
	if(P)
		glUniformMatrix4fv (P_loc, 1, GL_FALSE, proj.m);
	if(V)
		glUniformMatrix4fv (V_loc, 1, GL_FALSE, view.m);
}

void red::turn(char dir){
	if(turn_angle!=angle_turned)
		return;
	if ((red_dir=='S'&&dir=='E')||
		(red_dir=='E'&&dir=='N')||
		(red_dir=='N'&&dir=='W')||
		(red_dir=='W'&&dir=='S'))
		turn_angle += 90.0;
	if ((red_dir=='S'&&dir=='W')||
		(red_dir=='E'&&dir=='S')||
		(red_dir=='N'&&dir=='E')||
		(red_dir=='W'&&dir=='N'))
		turn_angle += -90.0;
	if ((red_dir=='S'&&dir=='N')||
		(red_dir=='W'&&dir=='E'))
		turn_angle += 180.0;
	if ((red_dir=='N'&&dir=='S')||
		(red_dir=='E'&&dir=='W'))
		turn_angle += -180.0;
	red_dir = dir;
}

void red::draw(){
	base_model = identity_mat4();
	if (turn_angle<angle_turned){
		angle_turned -= turn_increment;
	}
	if(turn_angle>angle_turned){
		angle_turned += turn_increment;
	}

	base_model = rotate_y_deg(base_model, angle_turned);

	base_model = translate(base_model, location);

	glUniformMatrix4fv (M_loc, 1, GL_FALSE, base_model.m);

	parts[FACE].draw();
	parts[HOOD].draw();
	parts[CLOAK].draw();


	//Draw axe
	offset_model[0] = scale(identity_mat4(), vec3 (.132, .132, .132));
	offset_model[0] = scale(offset_model[0], vec3 (0.7, 0.7, 0.7));
	offset_model[0] = translate(offset_model[0], vec3(-0.125, 0.0, 0.0));
		
	offset_model[0] = rotate_x_deg(offset_model[0], 90+axe_rot_1);
	offset_model[0] = rotate_z_deg(offset_model[0], axe_rot_1);
	offset_model[0] = rotate_x_deg(offset_model[0], axe_rot_2);
	offset_model[0] = rotate_y_deg(offset_model[0], axe_rot_3);
	if(hit){
		if(axe_rot_1 > -90){
			axe_rot_1 -= 4;
			hand_offset += 0.12;
		}
		else{
			if(axe_rot_2 < 90)
				axe_rot_2 += 6;
			else{
				if(axe_rot_3 > -90){
					hand_offset -= 0.06;
					axe_rot_3 -= 2;
				}
				else{
					hit = false;
					axe_rot_1 = 0;
					axe_rot_2 = 0;
					axe_rot_3 = 0;
					hand_offset = 0.0;
				}

			}
		}
	}

	offset_model[0] = translate(offset_model[0], vec3(0.125, 0.0, 0.0));
	vec3 axe_loc (-0.175, 0.56, 0.63);
	offset_model[0] = translate (offset_model[0], axe_loc);

	offset_model[0] = base_model*offset_model[0];

	glUniformMatrix4fv (M_loc, 1, GL_FALSE, offset_model[0].m);
	
	parts[AXE_HEAD].draw();
	parts[AXE_HANDLE].draw();

	//hand 1
	offset_model[1] = translate(identity_mat4(), vec3(hand_offset, 0.0, 0.0));
	offset_model[1] = offset_model[0]*offset_model[1];
	
	glUniformMatrix4fv (M_loc, 1, GL_FALSE, offset_model[1].m);
	parts[HAND].draw();

	//second hand
	offset_model[2] = translate(identity_mat4(), vec3(5, 0.0, 0.0));
	offset_model[2] = offset_model[0]*offset_model[2];
	
	glUniformMatrix4fv (M_loc, 1, GL_FALSE, offset_model[2].m);
	parts[HAND].draw();

}