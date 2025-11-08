/*
 * Bird.cpp: A class for animated bird object
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 26, 2023
 */

#include "Bird.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>
#include <cmath>

using namespace std;

//Constructor
Bird::Bird(void)
{
	//Initialize member variables
	list_tip = 0;
	list_mid = 0;
	list_base = 0;
	list_tail = 0;
	list_body = 0;
	texture_obj = NULL;
	initialized = false;
}

//Destructor
Bird::~Bird(void)
{
	//Free allocated memory
	if (initialized)
	{
		glDeleteLists(list_tip, 1);
		list_tip = NULL;
		glDeleteLists(list_mid, 1);
		list_mid = NULL;
		glDeleteLists(list_base, 1);
		list_base = NULL;
		glDeleteLists(list_tail, 1);
		list_tail = NULL;
		glDeleteLists(list_body, 1);
		list_body = NULL;
		glDeleteTextures(1, &texture_obj);
		texture_obj = NULL;
	}
}

//Set model scale
void
Bird::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

//Set flapping offset by initializing theta
void
Bird::SetPhaseOffset(float offset)
{
	theta = offset;
}

//Initialize model parameters and construct geometry lists
bool
Bird::Initialize(void)
{
	//Load texture data from file
	ubyte* image_data;
	int	    image_height, image_width;

	char* texture_file = "feathers.tga";
	if (!(image_data = (ubyte*)tga_load(texture_file, &image_width,
		&image_height, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "Bird::Initialize: Couldn't load %s\n", texture_file);
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

	// Shared wing coordinates
	coord3D v_base[4] = {	{ 0.0f, -4.0f, 0.0f },
							{ 0.0f, 2.0f, 0.0f },
							{ 4.0f, 2.5f, 0.0f },
							{ 4.0f, -3.0f, 0.0f } };
	coord3D v_mid[4] = {	{ 0.0f, 2.5f, 0.0f },	// Note: +4.0f units in x from base
							{ 0.0f, -3.0f, 0.0f },
							{ 5.0f, 2.0f, 0.0f },
							{ 5.0f, -2.0f, 0.0f } };

	coord3D v_tip[3] = {	{ 0.0f, 2.0f, 0.0f },	// Note: +5.0f units in x from mid
							{ 0.0f, -2.0f, 0.0f },
							{ 7.0f, -3.0f, 0.0f} };

	// Build wing-base
	list_base = glGenLists(1);
		glNewList(list_base, GL_COMPILE);
		glColor3f(1.0, 1.0, 1.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_obj);

		glBegin(GL_QUADS);

			//Top
			glNormal3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_base[0].x, v_base[0].y, v_base[0].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_base[1].x, v_base[1].y, v_base[1].z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(v_base[2].x, v_base[2].y, v_base[2].z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_base[3].x, v_base[3].y, v_base[3].z);
			//Bottom
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_base[3].x, v_base[3].y, v_base[3].z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(v_base[2].x, v_base[2].y, v_base[2].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_base[1].x, v_base[1].y, v_base[1].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_base[0].x, v_base[0].y, v_base[0].z);
	
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glEndList();

	// Build wing-middle
	list_mid = glGenLists(1);
		glNewList(list_mid, GL_COMPILE);
		glColor3f(1.0, 1.0, 1.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_obj);

		glBegin(GL_QUADS);

			//Top
			glNormal3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_mid[1].x, v_mid[1].y, v_mid[1].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_mid[0].x, v_mid[0].y, v_mid[0].z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(v_mid[2].x, v_mid[2].y, v_mid[2].z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_mid[3].x, v_mid[3].y, v_mid[3].z);
			//Bottom
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_mid[3].x, v_mid[3].y, v_mid[3].z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(v_mid[2].x, v_mid[2].y, v_mid[2].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_mid[0].x, v_mid[0].y, v_mid[0].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_mid[1].x, v_mid[1].y, v_mid[1].z);

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glEndList();

	// Build wing-tip
	list_tip = glGenLists(1);
		glNewList(list_tip, GL_COMPILE);
		glColor3f(1.0, 1.0, 1.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_obj);

		glBegin(GL_TRIANGLES);

			//Top
			glNormal3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_tip[1].x, v_tip[1].y, v_tip[1].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_tip[0].x, v_tip[0].y, v_tip[0].z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_tip[2].x, v_tip[2].y, v_tip[2].z);
			//Bottom
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_tip[2].x, v_tip[2].y, v_tip[2].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_tip[0].x, v_tip[0].y, v_tip[0].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_tip[1].x, v_tip[1].y, v_tip[1].z);

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glEndList();

	// Build tail
	list_tail = glGenLists(1);
		glNewList(list_tail, GL_COMPILE);
		glColor3f(1.0, 1.0, 1.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_obj);

		//Tail vertices
		coord3D v_tail[4] = {	{ 0.0f, 0.0f, 0.0f },
								{ 2.0f, -4.0f, 0.0f },
								{ -2.0f, -4.0f, 0.0f},
								{ 0.0f, -5.5f, 0.0f} };

		glBegin(GL_TRIANGLES);

			glNormal3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_tail[0].x, v_tail[0].y, v_tail[0].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_tail[1].x, v_tail[1].y, v_tail[1].z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_tail[3].x, v_tail[3].y, v_tail[3].z);
		
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_tail[3].x, v_tail[3].y, v_tail[3].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_tail[1].x, v_tail[1].y, v_tail[1].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_tail[0].x, v_tail[0].y, v_tail[0].z);

			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_tail[0].x, v_tail[0].y, v_tail[0].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_tail[2].x, v_tail[2].y, v_tail[2].z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_tail[3].x, v_tail[3].y, v_tail[3].z);

			glNormal3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_tail[3].x, v_tail[3].y, v_tail[3].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_tail[2].x, v_tail[2].y, v_tail[2].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_tail[0].x, v_tail[0].y, v_tail[0].z);

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glEndList();

	// Build body
	list_body = glGenLists(1);
		glNewList(list_body, GL_COMPILE);
		glColor3f(1.0, 1.0, 1.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_obj);

		coord3D norm = { 0.0f, 0.0f, 0.0f };	//Used to store calculated normal vectors
								// Body
		coord3D v_body[13] = {	{ 0.0f, 3.5f, 0.0f },
								{ 2.0f, 2.0f, 0.0f },
								{ 0.0f, 2.0f, 1.5f },
								{ -2.0f, 2.0f, 0.0f },
								{ 0.0f, 2.0f, -1.5f },
								{ 0.0f, -4.0f, 0.0f},
								// Head
								{ 0.0f, 4.5f, 0.0f },
								{ -1.25f, 2.0f, 0.0f },
								{ 1.25f, 2.0f, 0.0f },
								{ 0.0f, 2.0f, 1.5f },
								// Beak
								{0.0f, 5.0f, -1.0f},
								{-0.5f, 3.0f, 0.0f},
								{0.5f, 3.0f, 0.0f} };

		glBegin(GL_TRIANGLES);

			//Body
			Normal(&v_body[1], &v_body[0], &v_body[2], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[1].x, v_body[1].y, v_body[1].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[0].x, v_body[0].y, v_body[0].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[2].x, v_body[2].y, v_body[2].z);

			Normal(&v_body[2], &v_body[0], &v_body[3], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[2].x, v_body[2].y, v_body[2].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[0].x, v_body[0].y, v_body[0].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[3].x, v_body[3].y, v_body[3].z);

			Normal(&v_body[3], &v_body[0], &v_body[4], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[3].x, v_body[3].y, v_body[3].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[0].x, v_body[0].y, v_body[0].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[4].x, v_body[4].y, v_body[4].z);

			Normal(&v_body[4], &v_body[0], &v_body[1], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[4].x, v_body[4].y, v_body[4].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[0].x, v_body[0].y, v_body[0].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[1].x, v_body[1].y, v_body[1].z);

			Normal(&v_body[1], &v_body[2], &v_body[5], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[1].x, v_body[1].y, v_body[1].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[2].x, v_body[2].y, v_body[2].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[5].x, v_body[5].y, v_body[5].z);

			Normal(&v_body[2], &v_body[3], &v_body[5], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[2].x, v_body[2].y, v_body[2].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[3].x, v_body[3].y, v_body[3].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[5].x, v_body[5].y, v_body[5].z);

			Normal(&v_body[3], &v_body[4], &v_body[5], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[3].x, v_body[3].y, v_body[3].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[4].x, v_body[4].y, v_body[4].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[5].x, v_body[5].y, v_body[5].z);

			Normal(&v_body[4], &v_body[1], &v_body[5], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[4].x, v_body[4].y, v_body[4].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[1].x, v_body[1].y, v_body[1].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[5].x, v_body[5].y, v_body[5].z);

			//Head
			Normal(&v_body[7], &v_body[6], &v_body[8], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[7].x, v_body[7].y, v_body[7].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[6].x, v_body[6].y, v_body[6].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[8].x, v_body[8].y, v_body[8].z);

			Normal(&v_body[8], &v_body[6], &v_body[9], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[8].x, v_body[8].y, v_body[8].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[6].x, v_body[6].y, v_body[6].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[9].x, v_body[9].y, v_body[9].z);

			Normal(&v_body[9], &v_body[6], &v_body[7], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[9].x, v_body[9].y, v_body[9].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[6].x, v_body[6].y, v_body[6].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[7].x, v_body[7].y, v_body[7].z);

			Normal(&v_body[7], &v_body[8], &v_body[9], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[7].x, v_body[7].y, v_body[7].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[8].x, v_body[8].y, v_body[8].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[9].x, v_body[9].y, v_body[9].z);

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glEndList();

	// Build beak
	list_beak = glGenLists(1);
		glNewList(list_beak, GL_COMPILE);
		glColor3f(0.6f, 0.53f, 0.18f);	//No texture, set beak color

		glBegin(GL_TRIANGLES);

			Normal(&v_body[6], &v_body[10], &v_body[11], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v_body[6].x, v_body[6].y, v_body[6].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v_body[10].x, v_body[10].y, v_body[10].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v_body[11].x, v_body[11].y, v_body[11].z);

			Normal(&v_body[6], &v_body[12], &v_body[10], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glVertex3f(v_body[6].x, v_body[6].y, v_body[6].z);
			glVertex3f(v_body[12].x, v_body[12].y, v_body[12].z);
			glVertex3f(v_body[10].x, v_body[10].y, v_body[10].z);

			Normal(&v_body[11], &v_body[10], &v_body[12], &norm);
			glNormal3f(norm.x, norm.y, norm.z);
			glVertex3f(v_body[11].x, v_body[11].y, v_body[11].z);
			glVertex3f(v_body[10].x, v_body[10].y, v_body[10].z);
			glVertex3f(v_body[12].x, v_body[12].y, v_body[12].z);

		glEnd();
		glEndList();

	//End initialization
	initialized = true;
	return true;
}

//Update geometry control for animation
void
Bird::Update(float dt)
{
	theta += RATE * dt;
}

//Render geometry to screen
void
Bird::Draw(void)
{
	//Heirarchical model w/ flapping wings and tail
	glPushMatrix();
		//Reorient and scale model from Initialize() definition
		glRotatef(90.0f, 0.0f, 0.0f, -1.0f);
		glScalef(0.15f, 0.15f, 0.15f);
		glScalef(scale.x, scale.y, scale.z);	//User defined scaling

		glPushMatrix();
			// Body
			glCallList(list_body);
			glCallList(list_beak);

			glPushMatrix();
				// Right wing - heirarchy of wing elements
				glRotatef(cos(theta) * RANGE, 0.0f, -1.0f, 0.0f);
				glCallList(list_base);

				glTranslatef(4.0f, 0.0f, 0.0f);
				glRotatef(cos(theta + PHASE) * RANGE, 0.0f, -1.0f, 0.0f);
				glCallList(list_mid);

				glTranslatef(5.0f, 0.0f, 0.0f);
				glRotatef(cos(theta + 2 * PHASE) * RANGE, 0.0f, -1.0f, 0.0f);
				glCallList(list_tip);
			glPopMatrix();

			// Left Wing - heirarchy of wing elements
			glPushMatrix();
				//Reuse right-wing but flip 180deg, and rotation axes
				glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
				glRotatef(cos(theta) * RANGE, 0.0f, 1.0f, 0.0f);
				glCallList(list_base);

				glTranslatef(4.0f, 0.0f, 0.0f);
				glRotatef(cos(theta + PHASE) * RANGE, 0.0f, 1.0f, 0.0f);
				glCallList(list_mid);

				glTranslatef(5.0f, 0.0f, 0.0f);
				glRotatef(cos(theta + 2 * PHASE) * RANGE, 0.0f, 1.0f, 0.0f);
				glCallList(list_tip);
			glPopMatrix();

			//Tail - single element, no heirarchy
			glPushMatrix();
				glTranslatef(0.0f, -2.5f, 0.0f);
				glRotatef(0.5f * cos(theta) * RANGE, 1.0f, 0.0f, 0.0f);
				glCallList(list_tail);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}