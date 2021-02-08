#include "controler.h"



enum direction last_move = RIGHT;		// save the last move for u-turn

void init_lastmove(enum direction move){
	last_move=move;
}
// For CUnit test
wheels init_wheels(float left, float right){
	wheels wh;
	wh.left = left;
	wh.right = right;
	return wh;
}

// For debugging purpose in main
sens_obs init_sens_obs(float el, float ml, float mr, float er){
	sens_obs obs;
	obs.edge_l = el;
	obs.middle_l = ml;
	obs.middle_r = mr;
	obs.edge_r = er;
	return obs;
}


wheels u_turn(){ 
	wheels wh;
	if(last_move == LEFT){
		wh.right = 25.0;
		wh.left = -25.0;
		last_move = RIGHT;
	}
	else {
		wh.right = -25.0;
		wh.left = 25.0;
		last_move = LEFT;
	}
	return wh;
}


wheels go_base(){
	wheels wh;
	wh.left = 0.0;
	wh.right = 0.0;
	return wh; 
}


enum direction analyse_obs(sens_obs obs){
	int r = 0;
	int l = 0;

	// Analyse the 4 sensors
	if (obs.edge_l <= 20.0){
		l += 1;
	}
	if(obs.middle_l <= 20.0){
		l += 1;
	}
	if(obs.middle_r <= 20.0){
		r += 1;
	}
	if(obs.edge_r <= 20.0){
		r += 1;
	}
	

	if(l==2){
		return LEFT;
	}
	else if(r==2){ 
		return RIGHT;
	}

	return NONE;
}


wheels limit(wheels wh){
	if(wh.left > 25.0){
		wh.left = 25.0;
	}
	if(wh.right > 25.0){
		wh.right = 25.0;
	}
	if(wh.left < -25.0){
		wh.left = -25.0;
	}
	if(wh.right < -25.0){
		wh.right = -25.0;
	}
	return wh;
}


wheels avoid_obs(enum direction type_obs){
	wheels wh;
	if (type_obs == RIGHT){
		wh.right = 15.0;
		wh.left = -15.0;
	}
	else {
		wh.right = -15.0;
		wh.left = 15.0;
	}
	return wh;
}

float min(float x, float y){
	if(x<y){
		return x;
	}
	else{
		return y;
	}
}

wheels speed(sens_obs obs){
	wheels wh;
	wh.left = 25;
	wh.right = 25;

	if (obs.edge_l <= 0.0 || obs.middle_l <= 0.0 || obs.edge_r <= 0.0 || obs.middle_r <= 0.0){
		wh.left = 0;
		wh.right = 0;
	}

	else if (obs.edge_l <= 20.0 || obs.middle_l <= 20.0 || obs.edge_r <= 20.0 || obs.middle_r <= 20.0){
		wh.left = min(min(obs.edge_l, obs.middle_l), min(obs.edge_r, obs.middle_r));
		wh.right = min(min(obs.edge_l, obs.middle_l), min(obs.edge_r, obs.middle_r));
	}

	
	return wh;
}


wheels controler(bool gap, sens_obs obs, bool contact, bool end, int* cnt_contact){
	wheels wh = speed(obs);
	
	if(gap){
		wh = u_turn();
		*cnt_contact = 0;
	}
	else if(end){
		wh = go_base();
		*cnt_contact = 0;
	}
	else if(contact){
		*cnt_contact = (*cnt_contact) + 1;
		enum direction type_obs = analyse_obs(obs);
		if(*cnt_contact < 3 && type_obs != NONE){
			wh = avoid_obs(type_obs);
		}
		else if(*cnt_contact==3){
			wh = u_turn();
			*cnt_contact = 0;

		}
	}
	else{
		*cnt_contact = 0;
	}

	return limit(wh);
}

