/*
 * Tree.cpp: A class that constructs a forest of tree objects, varying their parameters
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 18, 2023
 */

#include "Forest.h"
#include <cstdlib>
#include <math.h>
#include <ctime>

//Constructor
Forest::Forest(void)
{
	comp_len = 0;
	components = NULL;
	initialized = false;
}

//Destructor
Forest::~Forest(void)
{
	//Free allocated memory
	if (initialized)
	{
		delete[] components;
		components = NULL;
	}
}

//Define a "patch" of trees parameters to render
void
Forest::SetPatch(float x_min, float x_max, float y_min, float y_max, int number_trees)
{
	comp_len = number_trees;
	perimeter_x_min = x_min;
	perimeter_x_max = x_max;
	perimeter_y_min = y_min;
	perimeter_y_max = y_max;
	patch_set = true;
}

//Initialize member variables and compute geometry
bool
Forest::Initialize(void)
{
	//Allocate components (trees) to render in forest
	components = new Tree[comp_len];

	srand(time(NULL));	//Seed rand() function

	//Generate trees with randomly generated attributes
	for (int i = 0; i < comp_len; ++i)
	{
		//Compute random attributes in defined ranges
		float r_height =fmod(rand(), (H_MAX - H_MIN)) + H_MIN;
		float r_radius =fmod(rand(), (R_MAX - R_MIN)) + R_MIN;
		float r_trunk =fmod(rand(), (T_MAX - T_MIN)) + T_MIN;
		float r_x = fmod(rand(), (perimeter_x_min - perimeter_x_max)) + perimeter_x_min;
		float r_y = fmod(rand(), (perimeter_y_min - perimeter_y_max)) + perimeter_y_min;
		float r_z = 0;

		//Set attributes
		components[i].SetParameters(r_height, r_radius, r_trunk);
		components[i].SetPosition(r_x, r_y, r_z);
	}

	//Initialize contained trees
	for (int i = 0; i < comp_len; ++i)
	{
		components[i].Initialize();
	}

	//Complete initialization
	initialized = true;
	return true;
}

//Render objects to screen
void
Forest::Draw(void)
{
	//Call Draw() method of contained trees
	for (int i = 0; i < comp_len; ++i)
	{
		components[i].Draw();
	}
}