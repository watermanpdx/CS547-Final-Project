/*
 * Tree.cpp: A class that draws a parameterizable tree object
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 18, 2023
 */

#include "Tree.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>
#include <cmath>

//Initialize static member variables
int
Tree::instances = 0;
GLubyte
Tree::foliage_list = NULL;
GLubyte
Tree::trunk_list = NULL;

//Constructor
Tree::Tree(void)
{
	++instances;	//Count instances of Tree
}

Tree::~Tree(void)
{
	--instances;	//Decrement Tree
	if (instances == 0)	//Only delete shared allocated memory if last object
	{
		glDeleteLists(foliage_list, 1);
		foliage_list = NULL;
		glDeleteLists(trunk_list, 1);
		trunk_list = NULL;
	}
}

//Set tree parameters
void
Tree::SetParameters(float height, float radius, float trunk_ratio)
{
	this->height = height;
	this->radius = radius;
	this->trunk_ratio = trunk_ratio;
}

//Set tree position
void
Tree::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

//Construct geometry
bool
Tree::Initialize(void)
{
	//Only construct if not initialized for class. If detected, no need to recompute
	if (foliage_list == NULL || trunk_list == NULL)
	{
		//Load texture data from file
		ubyte* image_data;
		int	    image_height, image_width;

		char* texture_file = "pine.tga";
		if (!(image_data = (ubyte*)tga_load(texture_file, &image_width,
			&image_height, TGA_TRUECOLOR_24)))
		{
			fprintf(stderr, "Tree::Initialize: Couldn't load %s\n", texture_file);
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

		//Draw foliage list
		foliage_list = glGenLists(1);
		glNewList(foliage_list, GL_COMPILE);
		glColor3f(1.0, 1.0, 1.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_obj);

		//Compute cartesian coordinates along sweep-arc
		coordArc* arc = new coordArc[FACES + 1];
		for (int i = 0; i <= FACES; ++i)
		{
			arc[i].u = (float)i / (FACES);
			arc[i].x = -cos(2 * PI * arc[i].u);
			arc[i].y = -sin(2 * PI * arc[i].u);
		}

		// Foliage slope (used for normal)
		float h = 1.0f;
		float r = 1.0f;
		float mag = sqrt(h * h + r * r);
		float nh = -r / mag;
		float nr = h / mag;

		glBegin(GL_TRIANGLES);
			// Cone surface and underside foliage
			for (int i = 0; i < FACES; ++i)
			{
				//Surface
				glNormal3f(nr * arc[i].x, nr * arc[i].y, nh);
				glTexCoord2d(0.5f * (arc[i].u + arc[i + 1].x), 1.0f);
				glVertex3d(0.0f, 0.0f, h);
				glTexCoord2d(arc[i].u, 0.0f);
				glVertex3d(r * arc[i].x, r * arc[i].y, 0.0f);
				glTexCoord2d(arc[i + 1].u, 0.0f);
				glVertex3d(r * arc[i+1].x, r * arc[i+1].y, 0.0f);
				//Underside
				glNormal3f(0.0f, 0.0f, -1.0f);
				glTexCoord2d(arc[i + 1].u, 0.0f);
				glVertex3d(r * arc[i + 1].x, r * arc[i + 1].y, 0.0f);
				glTexCoord2d(arc[i].u, 0.0f);
				glVertex3d(r * arc[i].x, r * arc[i].y, 0.0f);
				glTexCoord2d(0.5f * (arc[i].u + arc[i + 1].x), 1.0f);
				glVertex3d(0.0f, 0.0f, 0.0f);
			}

			//Arc coordinates no longer needed, free allocated memory
			delete[] arc;
			arc = NULL;

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glEndList();

		//Load different texture for trunk
		texture_file = "bark.tga";
		if (!(image_data = (ubyte*)tga_load(texture_file, &image_width,
			&image_height, TGA_TRUECOLOR_24)))
		{
			fprintf(stderr, "Tree::Initialize: Couldn't load %s\n", texture_file);
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

		//Create trunk geometry list
		trunk_list = glGenLists(1);
		glNewList(trunk_list, GL_COMPILE);
		glColor3f(1.0, 1.0, 1.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_obj);
		
		//Trunk vertices
		coord3D vertices[8] = { {-0.2f, -0.2f, 0.0f},
								{ 0.2f, -0.2f, 0.0f},
								{-0.2f,  0.2f, 0.0f},
								{ 0.2f,  0.2f, 0.0f},
								{-0.2f, -0.2f, 1.0f},
								{ 0.2f, -0.2f, 1.0f},
								{-0.2f,  0.2f, 1.0f},
								{ 0.2f,  0.2f, 1.0f} };

		glBegin(GL_QUADS);

			//Front
			glNormal3f(0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
			//Back
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
			//Right
			glNormal3f(1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
			//Left
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
			//No top
			//Bottom
			glNormal3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glEndList();
	}
	return true;
}

//Render geometry to screen
void
Tree::Draw(void)
{
	glPushMatrix();
		//Set tree position
		glTranslatef(position.x, position.y, position.z);
		glPushMatrix();
			//Draw trunk
			glScalef(radius, radius, height * trunk_ratio);
			glCallList(trunk_list);
		glPopMatrix();
		//Draw foliage
		glTranslatef(0.0f, 0.0f, height * trunk_ratio);
		glScalef(radius, radius, height * (1.0f - trunk_ratio));
		glCallList(foliage_list);
	glPopMatrix();
}