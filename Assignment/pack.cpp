#include "pack.h"

pack::pack(){
	alpha = NULL;
	pack_size = 0;
	max_size = 5;
	_new = NULL;
}

pack::pack(int _size){
	alpha = NULL;
	pack_size = 0;
	max_size = _size;
	_new = NULL;
}

pack::~pack(){	
	while (alpha) {
		pack_member *p = alpha->next;
		delete alpha;
		alpha = p;
	}
}

bool pack::move(int *grid, int size, int red_row, int red_col){
	pack_member *curr = alpha;
	pack_member *prev = NULL;

	bool kill_red = false;

	while(curr!=NULL){
		if(curr->move(grid, size, red_row, red_col))
			kill_red = true;
		if (curr->in_scope(red_row, red_col)){
			prev = curr;
			curr = curr->next;
		}
		else{
			grid[curr->row*size+curr->col] = 0;
			if(prev){
				prev->next = curr->next;
				//delete curr;
				curr = prev->next;
			}
			else{
				alpha = curr->next;
				//delete curr;
				curr = alpha;
			}
			pack_size--;
		}
	}

	if(pack_size<max_size){
		int chance_row = rand()%11;
		int chance_col = rand()%11;
		if((chance_row==1||chance_row==9||chance_row==0||chance_row==10)||(chance_col==1||chance_col==9||chance_col==0||chance_col==10)){
			if(((red_row-5+chance_row)<size)&&((red_col-5+chance_col)<size)&&((red_row-5+chance_row)>=0)&&((red_col-5+chance_col)>=0)){
				if(grid[(red_row-5+chance_row)*size+(red_col-5+chance_col)]==0){
					_new = new pack_member(red_row-5+chance_row, red_col-5+chance_col);
					_new->next = alpha;
					alpha = _new;
					_new = NULL;
					grid[(red_row-5+chance_row)*size+(red_col-5+chance_col)]=3;
					pack_size++;
				}
			}
		}
	}
	return 	kill_red;
}

void pack::draw(wolf big_bad){
	pack_member *curr = alpha;

	while(curr!=NULL){
		if (curr->turn_angle<curr->angle_turned){
			curr->angle_turned -= curr->turn_increment;
		}
		if(curr->turn_angle>curr->angle_turned){
			curr->angle_turned += curr->turn_increment;
		}
		big_bad.base_model = rotate_y_deg(identity_mat4(), curr->angle_turned);
		big_bad.base_model = translate(big_bad.base_model, curr->get_loc());
		big_bad.draw();
		curr = curr->next;
	}
}

void pack::kill_wolf(int  _row, int _col){
	pack_member *curr = alpha;
	pack_member *prev = NULL;
	
	while (curr!=NULL){
		if((curr->row==_row)&&(curr->col==_col)){
			if(prev!=NULL){
				prev->next=curr->next;		//One to kill is not head
				//delete curr;
			}
			else{
				alpha=curr->next;	//removing head	
				//delete curr;
			}
			pack_size--;
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}