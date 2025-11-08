/*
 * Block.cpp: A class that draws an archway
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 28, 2023
 */

#include "Arch.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>
#include <cmath>

//Constructor
Arch::Arch(void)
{
	display_list = 0;
	texture_obj = NULL;
	initialized = false;
}

//Destructive
Arch::~Arch(void)
{
	//Free allocated memory
	if (initialized)
	{
		glDeleteLists(display_list, 1);
		glDeleteTextures(1, &texture_obj);
	}
}

//Set block scale
void
Arch::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

//Set block position
void
Arch::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

//Initialize member data and define geometry
bool
Arch::Initialize(void)
{
	//Load texture data from file
	ubyte* image_data;
	int	    image_height, image_width;

	char* texture_file = "jasmine.tga";
	if (!(image_data = (ubyte*)tga_load(texture_file, &image_width,
		&image_height, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "Block::Initialize: Couldn't load %s\n", texture_file);
		return false;
	}

	//Apply texture
	glGenTextures(1, &texture_obj);
	glBindTexture(GL_TEXTURE_2D, texture_obj);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Construct mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_width, image_height,
		GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Build cube geometry
	display_list = glGenLists(1);
	glNewList(display_list, GL_COMPILE);
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj);

	//Compute cartesian coordinates along sweep-arcs
	coordArc* arc_inner = new coordArc[INNER_FACES + 1];
	for (int i = 0; i <= INNER_FACES; ++i)
	{
		arc_inner[i].u = (float)i / (INNER_FACES);
		arc_inner[i].x = cos(2 * PI * arc_inner[i].u);
		arc_inner[i].y = sin(2 * PI * arc_inner[i].u);
	}
	coordArc* arc_outer = new coordArc[OUTER_FACES + 1];
	for (int i = 0; i <= OUTER_FACES; ++i)
	{
		arc_outer[i].u = (float)i / (OUTER_FACES);
		arc_outer[i].x = cos(2 * PI * arc_outer[i].u);
		arc_outer[i].y = sin(2 * PI * arc_outer[i].u);
	}
	coord3D v1, v2, v3, v4, norm;
	float slice = (height - radius) / LEG_DIVS;

	glBegin(GL_QUADS);

		//Arch top	
		for (int i = 0; i < OUTER_FACES / 2; ++i)
		{
			for (int j = 0; j < INNER_FACES; ++j)
			{
				//Compute face vertices
				v1 = { thickness * arc_inner[j].x,
						arc_outer[i].x * (thickness * arc_inner[j].y + radius),
						arc_outer[i].y * (thickness * arc_inner[j].y + radius) + height - radius };
				v2 = { thickness * arc_inner[j + 1].x,
						arc_outer[i].x * (thickness * arc_inner[j + 1].y + radius),
						arc_outer[i].y * (thickness * arc_inner[j + 1].y + radius) + height - radius };
				v3 = { thickness * arc_inner[j + 1].x,
						arc_outer[i + 1].x * (thickness * arc_inner[j + 1].y + radius),
						arc_outer[i + 1].y * (thickness * arc_inner[j + 1].y + radius) + height - radius };
				v4 = { thickness * arc_inner[j].x,
						arc_outer[i + 1].x * (thickness * arc_inner[j].y + radius),
						arc_outer[i + 1].y * (thickness * arc_inner[j].y + radius) + height - radius };

				Normal(&v1, &v2, &v3, &norm);
				glNormal3f(norm.x, norm.y, norm.z);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(v1.x, v1.y, v1.z);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(v2.x, v2.y, v2.z);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(v3.x, v3.y, v3.z);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(v4.x, v4.y, v4.z);
			}
		}

		//Arch legs
		for (int i = 0; i < LEG_DIVS; ++i)
		{
			for (int j = 0; j < INNER_FACES; ++j)
			{
				//Compute face vertices
				v1 = { thickness * arc_inner[j].x,
						thickness * arc_inner[j].y + radius,
						slice* i };
				v2 = { thickness * arc_inner[j + 1].x,
						thickness * arc_inner[j + 1].y + radius,
						slice* i };
				v3 = { thickness * arc_inner[j + 1].x,
						thickness * arc_inner[j + 1].y + radius,
						slice* (i + 1) };
				v4 = { thickness * arc_inner[j].x,
						thickness * arc_inner[j].y + radius,
						slice* (i + 1) };

				//Right leg
				Normal(&v1, &v2, &v3, &norm);
				glNormal3f(norm.x, norm.y, norm.z);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(v1.x, v1.y, v1.z);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(v2.x, v2.y, v2.z);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(v3.x, v3.y, v3.z);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(v4.x, v4.y, v4.z);
				//Left leg
				Normal(&v1, &v2, &v3, &norm);
				glNormal3f(norm.x, norm.y, norm.z);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(v1.x, v1.y - 2 * radius, v1.z);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(v2.x, v2.y - 2 * radius, v2.z);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(v3.x, v3.y - 2 * radius, v3.z);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(v4.x, v4.y - 2 * radius, v4.z);
			}
		}

		//Free up allocated memory
		delete[] arc_inner;
		arc_inner = NULL;
		delete[] arc_outer;
		arc_outer = NULL;

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEndList();

	//End initialization
	initialized = true;
	return true;
}

//Render geometry to screen
void
Arch::Draw(void)
{
	glPushMatrix();
		//Apply transformations and draw to screen
		glTranslatef(position.x, position.y, position.z);
		glScalef(scale.x, scale.y, scale.z);
		glCallList(display_list);
	glPopMatrix();
}