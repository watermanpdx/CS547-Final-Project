/*
 * Block.cpp: A class that draws a deformable cube with brick-texture
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 11, 2023
 */

#include "Block.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>

using namespace std;

//Constructor
Block::Block(void)
{
	display_list = 0;
	texture_obj = NULL;
	initialized = false;
}

//Destructor
Block::~Block(void)
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
Block::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

//Set block position
void
Block::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

//Initialize member data and define geometry
bool
Block::Initialize(void)
{
	//Load texture data from file
	ubyte* image_data;
	int	    image_height, image_width;

	char* texture_file = "brick.tga";
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

	//Cube vertices
	coord3D vertices[8] = { {-0.5f, -0.5f, 0.0f},
							{ 0.5f, -0.5f, 0.0f},
							{-0.5f,  0.5f, 0.0f},
							{ 0.5f,  0.5f, 0.0f},
							{-0.5f, -0.5f, 1.0f},
							{ 0.5f, -0.5f, 1.0f},
							{-0.5f,  0.5f, 1.0f},
							{ 0.5f,  0.5f, 1.0f} };

	glBegin(GL_QUADS);

		//Front
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
		glTexCoord2f(1.0f * texture_scale * scale.x, 0.0f);
		glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
		glTexCoord2f(1.0f * texture_scale * scale.x, 1.0f * texture_scale * scale.z);
		glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
		glTexCoord2f(0.0f, 1.0f * texture_scale * scale.z);
		glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
		//Back
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
		glTexCoord2f(1.0f * texture_scale * scale.x, 0.0f);
		glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
		glTexCoord2f(1.0f * texture_scale * scale.x, 1.0f * texture_scale * scale.z);
		glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
		glTexCoord2f(0.0f, 1.0f * texture_scale * scale.z);
		glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
		//Right
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
		glTexCoord2f(1.0f * texture_scale * scale.y, 0.0f);
		glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
		glTexCoord2f(1.0f * texture_scale * scale.y, 1.0f * texture_scale * scale.z);
		glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
		glTexCoord2f(0.0f, 1.0f * texture_scale * scale.z);
		glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
		//Left
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
		glTexCoord2f(1.0f * texture_scale * scale.y, 0.0f);
		glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
		glTexCoord2f(1.0f * texture_scale * scale.y, 1.0f * texture_scale * scale.z);
		glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
		glTexCoord2f(0.0f, 1.0f * texture_scale * scale.z);
		glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
		//Top
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
		glTexCoord2f(1.0f * texture_scale * scale.x, 0.0f);
		glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
		glTexCoord2f(1.0f * texture_scale * scale.x, 1.0f * texture_scale * scale.y);
		glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
		glTexCoord2f(0.0f, 1.0f * texture_scale * scale.y);
		glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
		//Bottom
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
		glTexCoord2f(1.0f * texture_scale * scale.x, 0.0f);
		glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
		glTexCoord2f(1.0f * texture_scale * scale.x, 1.0f * texture_scale * scale.y);
		glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
		glTexCoord2f(0.0f, 1.0f * texture_scale * scale.y);
		glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEndList();

	//End initialization
	initialized = true;
	return true;
}

//Render geometry to screen
void
Block::Draw(void)
{
	glPushMatrix();
		//Apply transformations and draw to screen
		glTranslatef(position.x, position.y, position.z);
		glScalef(scale.x, scale.y, scale.z);
		glCallList(display_list);
	glPopMatrix();
}