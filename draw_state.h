#ifndef DRAW_STATE_H
#define DRAW_STATE_H

#include "position.h"
#include "velocity.h"
#include <vector>

struct tank_state
	{
	position start_pos;
	position pos;
	velocity vel;
	float heading;
	};

struct bullet_state
	{
	position start_pos;
	position pos;
	velocity vel;
	};

struct building_state
	{
	position top_right;
	position bottom_left;
	int health;
	};

struct explosion_state
	{
	position center;
	float r;  //radius
	float dr; //delta-radius
	};

struct draw_state
	{
	unsigned int framestamp;//tick-time
	std::vector<tank_state> tanks;
	std::vector<bullet_state> bullets;
	std::vector<building_state> buildings;
	std::vector<explosion_state> explosions;
	void clear()
		{
		tanks.clear();
		bullets.clear();
		buildings.clear();
		explosions.clear();
		}
	};

#endif
