/*
 * Wall.cpp: A class that constructs a wall constructed from Block objects
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 15, 2023
 */

#include "Wall.h"

//Constructor
Wall::Wall(void)
{
	comp_len = 0;
	components = NULL;
	initialized = false;
}

//Destructor
Wall::~Wall(void)
{
	//Free allocated memory
	if (initialized)
	{
		delete[] components;
		components = NULL;
	}
}

//Initialize member data and define geometry
bool
Wall::Initialize(void)
{
	//Allocate components memory (Block objects)
	comp_len = 19;
	components = new Block[comp_len];

	//Construct wall as multiple instances of blocks
	components[0].SetPosition(-45.0f, 0.0f, 0.0f);
	components[0].SetScale(0.5f, 90.0f, 3.0f);

	components[1].SetPosition(0.0f, -45.0f, 0.0f);
	components[1].SetScale(90.0f, 0.5f, 3.0f);

	components[2].SetPosition(0.0f, 45.0f, 0.0f);
	components[2].SetScale(90.0f, 0.5f, 3.0f);

	components[3].SetPosition(45.0f, -26.25f, 0.0f);
	components[3].SetScale(0.5f, 37.5f, 3.0f);

	components[4].SetPosition(45.0f, 26.25f, 0.0f);
	components[4].SetScale(0.5f, 37.5f, 3.0f);

	components[5].SetPosition(-45.0f, -45.0f, 0.0f);
	components[5].SetScale(2.0f, 2.0f, 4.0f);

	components[6].SetPosition(45.0f, -45.0f, 0.0f);
	components[6].SetScale(2.0f, 2.0f, 4.0);

	components[7].SetPosition(-45.0f, 45.0f, 0.0f);
	components[7].SetScale(2.0f, 2.0f, 4.0f);

	components[8].SetPosition(45.0f, 45.0f, 0.0f);
	components[8].SetScale(2.0f, 2.0f, 4.0f);

	components[9].SetPosition(45.0f, -7.5f, 0.0f);
	components[9].SetScale(2.0f, 2.0f, 4.0f);

	components[10].SetPosition(45.0f, 7.5f, 0.0f);
	components[10].SetScale(2.0f, 2.0f, 4.0f);

	components[11].SetPosition(-15.0f, -45.0f, 0.0f);
	components[11].SetScale(2.0f, 2.0f, 4.0f);

	components[12].SetPosition(15.0f, -45.0f, 0.0f);
	components[12].SetScale(2.0f, 2.0f, 4.0f);

	components[13].SetPosition(-15.0f, 45.0f, 0.0f);
	components[13].SetScale(2.0f, 2.0f, 4.0f);

	components[14].SetPosition(15.0f, 45.0f, 0.0f);
	components[14].SetScale(2.0f, 2.0f, 4.0f);

	components[15].SetPosition(-45.0f, -15.0f, 0.0f);
	components[15].SetScale(2.0f, 2.0f, 4.0f);

	components[16].SetPosition(-45.0f, 15.0f, 0.0f);
	components[16].SetScale(2.0f, 2.0f, 4.0f);

	components[17].SetPosition(45.0f, -20.0f, 0.0f);
	components[17].SetScale(2.0f, 2.0f, 4.0f);

	components[18].SetPosition(45.0f, 20.0f, 0.0f);
	components[18].SetScale(2.0f, 2.0f, 4.0f);

	//Initialize components
	for (int i = 0; i < comp_len; ++i)
	{
		components[i].Initialize();
	}
	
	//Initialization complete
	initialized = true;
	return true;
}

//Render geometry to screen
void
Wall::Draw(void)
{
	for (int i = 0; i < comp_len; ++i)
	{
		components[i].Draw();
	}
}