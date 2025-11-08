/*
 * Mountain.cpp: A class that draws a mountain with subdivision fractaling
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 25, 2023
 */

#include "Mountain.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>
#include <cstdlib>
#include <math.h>
#include <ctime>

//Constructor
Mountain::Mountain(void)
{

}

//Destructor
Mountain::~Mountain(void)
{
	//Free allocated memory
	if (initialized)
	{
		for (int i = 0; i < MAX_LEVEL; ++i)
		{
			glDeleteLists(display_lists[i], 1);
			display_lists[i] = NULL;
		}
		glDeleteTextures(1, &texture_obj);
		texture_obj = NULL;
	}
	for (int i = 0; i < 3; ++i)
	{
		if (meshes[i] != NULL)
		{
			delete meshes[i];
		}
	}
}

//Define base mesh to apply subdivision on (Level 0)
void
Mountain::SeedMesh(void)
{
	//Mountain vertices
	coord3D vertices[13] = { { 8.0f, -5.0f, 0.0f },
								{ 4.0f, 6.0f, 6.0f },
								{ -7.0f, 5.0f, 10.0f },
								{ -5.0f, -7.0f, 7.0f },
								{ -1.0f, -1.0f, 13.0f },
								{ 6.0f, -13.0f, 0.0f },
								{ -8.0f, -11.0f, 7.5f },
								{ -10.0f, -20.0f, 0.0f },
								{ -17.0f, -10.0f, 0.0f },
								{ -14.0f, 13.0f, 0.0f },
								{ 0.0f, 17.0f, 0.0f },
								{ 8.0f, 9.0f, 0.0f },
								{ 16.0f, 6.0f, 0.0f } };
	//Mountain faces
	triangleIndices faces[16] = { { 4, 0, 1 },
									{ 4, 1, 2 },
									{ 4, 2, 3},
									{ 4, 3, 0 },
									{ 5, 0, 3 },
									{ 5, 3, 6 },
									{ 5, 6, 7 },
									{ 7, 6, 8},
									{ 6, 3, 8 },
									{ 3, 2, 8 },
									{ 9, 8, 2 },
									{ 9, 2, 1 },
									{ 10, 9, 1 },
									{ 11, 10, 1 },
									{ 12, 11, 1 },
									{ 12, 1, 0 } };
	//Store to mesh object
	meshes[0] = new Mesh(vertices, 13, faces, 16);
}

//Subdivide triangular faces in model and vary to generate additional detail
int
Mountain::Subdivide(void)
{
	// Indices table used to prevent double vertices logging
	Mesh* seed = meshes[computed_level];
	int** indices = new int*[seed->num_vertices];
	for (int i = 0; i < seed->num_vertices; ++i)
	{
		indices[i] = new int[seed->num_vertices];
		for (int j = 0; j < seed->num_vertices; ++j)
		{
			indices[i][j] = -1;	//Initialize to -1 (unused)
		}
	}

	// Temporary mesh elements
	coord3D* vertices = new coord3D[6 * seed->num_faces];
	triangleIndices* faces = new triangleIndices[4 * seed->num_faces];
	int v = 0; //vertex index
	int f = 0; //faces index

	// Subdivide faces generating new mesh
	for (int i = 0; i < seed->num_faces; ++i)
	{
		// Generate new vertices (if not already existing)
		int v1 = seed->faces[i].v1;
		int v1_new = UpdateVertices(vertices, seed, indices, v1, v1, v);
		int v2 = seed->faces[i].v2;
		int v2_new = UpdateVertices(vertices, seed, indices, v2, v2, v);
		int v3 = seed->faces[i].v3;
		int v3_new = UpdateVertices(vertices, seed, indices, v3, v3, v);
		int v12 = UpdateVertices(vertices, seed, indices, v1, v2, v);
		int v23 = UpdateVertices(vertices, seed, indices, v2, v3, v);
		int v13 = UpdateVertices(vertices, seed, indices, v1, v3, v);

		// Generate new faces
		faces[f++] = { v1_new, v12, v13 };
		faces[f++] = { v12, v2_new, v23 };
		faces[f++] = { v13, v23, v3_new };
		faces[f++] = { v13, v12, v23 };
	}

	// Free up indices table
	for (int i = 0; i < seed->num_vertices; ++i)
	{
		 delete[] indices[i];
		 indices[i] = NULL;
	}
	delete[] indices;
	indices = NULL;

	// Create and return new mesh object
	meshes[++computed_level] = new Mesh(vertices, v, faces, f);
	delete[] vertices;
	vertices = NULL;
	delete[] faces;
	faces = NULL;
	return computed_level;
}

//Update vertices and index table. Used to ensure when faces are subdivided, adjacent faces don't
//generate duplicate vertices
int
Mountain::UpdateVertices(coord3D * vertices, Mesh* seed, int** indices, int i, int j, int &index)
{
	// Ensure order of indices so vertices aren't entered twice
	if (i > j)
	{
		int temp = i;
		i = j;
		j = temp;
	}

	// If vertex already has been computed_level, just return index
	if (indices[i][j] >= 0)
	{
		return indices[i][j];
	}

	// If vertex is novel, add it to indices and vertices array
	indices[i][j] = index;
	if (i == j)
	{
		// Store original vertex to new vertices array
		vertices[index].x = seed->vertices[i].x;
		vertices[index].y = seed->vertices[i].y;
		vertices[index].z = seed->vertices[i].z;
	}
	else
	{
		// Compute new vertex at midpoint and randomly vary
		float variance = VARIANCE / (1.2f * (computed_level + 1));
		vertices[index].x = 0.5f * (seed->vertices[i].x + seed->vertices[j].x) + fmod(rand(), variance) - 0.5f * variance;
		vertices[index].y = 0.5f * (seed->vertices[i].y + seed->vertices[j].y) + fmod(rand(), variance) - 0.5f * variance;
		vertices[index].z = 0.5f * (seed->vertices[i].z + seed->vertices[j].z);
		if (vertices[index].z != 0.0f) // Do not vary height if on the ground
		{
			vertices[index].z += fmod(rand(), variance) - 0.5f * variance;
			if (vertices[index].z < 0.25f) // Prevent artifacts from being too close to the ground plane
			{
				vertices[index].z = 0.25f;
			}
		}
	}
	return index++;
}

//Build display list for a given subdivision level
bool
Mountain::BuildDisplayList(int index)
{
	//Load texture from file
	ubyte* image_data;
	int	    image_height, image_width;

	char* texture_file = "rock.tga";
	if (!(image_data = (ubyte*)tga_load(texture_file, &image_width,
		&image_height, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "Mountain::BuildDisplayList: Couldn't load %s\n", texture_file);
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

	//Create display list and add to display list array
	display_lists[index] = glGenLists(1);
	glNewList(display_lists[index], GL_COMPILE);
	glColor3f(1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj);

	glBegin(GL_TRIANGLES);

		for (int i = 0; i < meshes[index]->num_faces; ++i)
		{
			// Fetch vertices
			coord3D* v1 = NULL;
			v1 = &meshes[index]->vertices[meshes[index]->faces[i].v1];
			coord3D* v2 = NULL;
			v2 = &meshes[index]->vertices[meshes[index]->faces[i].v2];
			coord3D* v3 = NULL;
			v3 = &meshes[index]->vertices[meshes[index]->faces[i].v3];

			// Calculate normal
			coord3D norm;
			Normal(v1, v2, v3, &norm);

			// Draw triangle
			glNormal3f(norm.x, norm.y, norm.z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v1->x, v1->y, v1->z);
			glTexCoord2f(0.5f, 1.0f);
			glVertex3f(v2->x, v2->y, v2->z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v3->x, v3->y, v3->z);
		}

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEndList();

	return true;
}

//Increase subdivision level to be displayed
int
Mountain::IncreaseSubdivision(void)
{
	if (level < MAX_LEVEL - 1)
	{
		++level;
	}
	return level;
}

//Decrease subdivision level to be displayed
int
Mountain::DecreaseSubdivision(void)
{
	if (level > 0) {
		--level;
	}
	return level;
}

//Define model geometries including all sub-divisions
bool
Mountain::Initialize(void)
{
	srand(time(NULL));	//Seed rand()
	
	// Initialize seed mountain and pre-compute subdivisions
	SeedMesh();
	// Subdivide
	for (int i = 1; i < MAX_LEVEL; ++i)
	{
		Subdivide();
	}
	int j = 3;

	// Build display lists
	for (int i = 0; i < MAX_LEVEL; ++i)
	{
		if (!BuildDisplayList(i))
		{
			return false;
		}
	}

	//Complete initialization
	initialized = true;
	return true;
}

//Render geometry to screen
void
Mountain::Draw()
{
	glPushMatrix();
	glCallList(display_lists[level]);	//Only display selected subdivision level
	glPopMatrix();
}