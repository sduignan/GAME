#include "pack_member.h"

pack_member::pack_member(){
	for(int i=0; i<4; i++){
		foot_offsets[i] = vec3(0.0, 0.0, 0.0);
		foot_rotations[i] = 0.0;
	}

	row = 0;
	col = 0;

	offset = vec3 (0.0, 0.0, 0.0);
	
	next = NULL;

	facing_dir = 'W';
	
	turn_angle = 0.0;
	turn_increment = 9.0;
	angle_turned = 0;
}

pack_member::pack_member(int _row, int _col){
	for(int i=0; i<4; i++){
		foot_offsets[i] = vec3(0.0, 0.0, 0.0);
		foot_rotations[i] = 0.0;
	}

	row = _row;
	col = _col;

	offset = vec3 (0.0, 0.0, 0.0);
		
	next = NULL;
	
	facing_dir = 'W';
	
	turn_angle = 0.0;
	turn_increment = 9.0;
	angle_turned = 0;
}

void pack_member::turn(char dir){
	if(dir==facing_dir)
		return;
	if(turn_angle!=angle_turned)
		return;
	if ((facing_dir=='S'&&dir=='E')||
		(facing_dir=='E'&&dir=='N')||
		(facing_dir=='N'&&dir=='W')||
		(facing_dir=='W'&&dir=='S'))
		turn_angle += 90.0;
	if ((facing_dir=='S'&&dir=='W')||
		(facing_dir=='E'&&dir=='S')||
		(facing_dir=='N'&&dir=='E')||
		(facing_dir=='W'&&dir=='N'))
		turn_angle += -90.0;
	if ((facing_dir=='S'&&dir=='N')||
		(facing_dir=='W'&&dir=='E'))
		turn_angle += 180.0;
	if ((facing_dir=='N'&&dir=='S')||
		(facing_dir=='E'&&dir=='W'))
		turn_angle += -180.0;
	facing_dir = dir;
}

bool pack_member::move(int *grid, int size, int red_row, int red_col){
	bool kill_red = false;
	if((offset.v[0]!=0.0)||(offset.v[2]!=0.0))
		return kill_red;
	if(red_row>row){
		turn('S');
		if(grid[(row+1)*size+col]==0){
			grid[(row+1)*size+col] = 3;
			grid[row*size+col] = 0;
			row++;
			offset = vec3 (0.0, 0.0, -10.0);
			return kill_red;
		}
		else{
			if(grid[(row+1)*size+col]==2){
				kill_red = true;
			}
		}
	}
	if(red_row<row){
		turn('N');
		if(grid[(row-1)*size+col]==0){
			grid[(row-1)*size+col] = 3;
			grid[row*size+col] = 0;
			row--;
			offset = vec3 (0.0, 0.0, 10.0);
			return kill_red;
		}
		else{
			if(grid[(row-1)*size+col]==2){
				kill_red = true;
			}
		}
	}
	if(red_col>col){
		turn('E');
		if(grid[row*size+col+1]==0){
			grid[row*size+col+1] = 3;
			grid[row*size+col] = 0;
			col++;
			offset = vec3 (-10.0, 0.0, 0.0);
			return kill_red;
		}
		else{
			if(grid[row*size+col+1]==2){
				kill_red = true;
			}
		}
	}
	if(red_col<col){
		turn('W');
		if(grid[row*size+col-1]==0){
			grid[row*size+col-1] = 3;
			grid[row*size+col] = 0;
			col--;
			offset = vec3 (10.0, 0.0, 0.0);
			return kill_red;
		}
		else{
			if(grid[row*size+col-1]==2){
				kill_red = true;
			}
		}
	}
	return kill_red;
}


bool pack_member::in_scope(int red_row, int red_col){
	if((abs(red_row-row)<5)&&(abs(red_col-col)<5)){
		return true;
	}
	return false;
}

vec3 pack_member::get_loc(){
	
	if(offset.v[0]<0.0){
		offset.v[0]+=1.0;
	}
	if(offset.v[0]>0.0){
		offset.v[0]-=1.0;
	}
	if(offset.v[2]<0.0){
		offset.v[2]+=1.0;
	}
	if(offset.v[2]>0.0){
		offset.v[2]-=1.0;
	}

	vec3 loc = vec3((float(col)-2.0), -0.75, float(row));
	loc = loc + vec3(offset.v[0]/10, 0.0, offset.v[2]/10);

	return loc;
}

