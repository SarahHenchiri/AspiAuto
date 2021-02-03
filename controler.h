#ifndef CONTROLER_H_
#define CONTROLER_H_

#include <stdio.h>
#include <stdbool.h>

enum direction {NONE, RIGHT, LEFT};

typedef struct wheels{
	float left;
	float right;
} wheels;

typedef struct sens_obs{	// capteur d'obstacle (bord_gauche, milieu_gauche, milieu_droit, bord_droit)
	float edge_l;
	float middle_l;
	float middle_r;
	float edge_r;
} sens_obs;			


/**
 * Put opposite speed on wheels to do a U-turn according to the last move
 * @return wheels speed
 */
wheels u_turn();

/**
 * Change speed of wheels to lead the robot to its base
 * @return wheels speed
 */
wheels go_base();


/**
 * Determine in which side the obstacle is
 * @return RIGHT if the obstacle is in the right
 * 			LEFT if the obstacle is in the left
 *		 	else NONE
 */
enum direction analyse_obs(sens_obs obs);

/*
 * Decrease wheels speed if they exceed 25m/s
 * @return wheels speed
 */
wheels limit(wheels wh);

/*
 * Change wheels speed to avoid an obstacle
 * @param  the side in which the obstacle was detect
 * @return wheels speed
 */
wheels avoid_obs(enum direction type_obs);

/*
 * @param  two float
 * @return the minimum beetween them
 */
float min(float x, float y);

/*
 * Reduce wheels speed depending on the distance of obstacle
 * @param obstacle's sensors
 * @return wheels speed
 */
wheels speed(sens_obs obs);

/**
 * Analyse sensors of the robot to control its trajectory
 * @return wheels speed
 */
wheels controler(bool gap, sens_obs obs, bool contact, bool end);

#endif