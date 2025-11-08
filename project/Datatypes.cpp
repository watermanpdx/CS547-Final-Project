/*
 * Datatypes.cpp: Common datatypes implementations used for cross-project use
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 25, 2023
 */

#include "Datatypes.h"
#include <cmath>

//Default constructor
Mesh::Mesh(void)
{
	vertices = NULL;
	faces = NULL;
}

//Constructor w/ data elements to initialize member variables
Mesh::Mesh(coord3D* vert_arr, int vert_len, triangleIndices* face_arr, int face_len)
{
	num_vertices = vert_len;
	vertices = new coord3D[num_vertices];
	for (int i = 0; i < num_vertices; ++i)	//Deep-copy
	{
		vertices[i].x = vert_arr[i].x;
		vertices[i].y = vert_arr[i].y;
		vertices[i].z = vert_arr[i].z;
	}

	num_faces = face_len;
	faces = new triangleIndices[num_faces];
	for (int i = 0; i < num_faces; ++i)	//Deep-copy
	{
		faces[i].v1 = face_arr[i].v1;
		faces[i].v2 = face_arr[i].v2;
		faces[i].v3 = face_arr[i].v3;
	}
}

//Destructor
Mesh::~Mesh(void)
{
	if (vertices != NULL)
	{
		delete[] vertices;
		vertices = NULL;
	}
	if (faces != NULL)
	{
		delete[] faces;
		faces = NULL;
	}
}

//Calculate unit-vector normal from vertices v1, v2, and v3
void
Normal(coord3D* v1, coord3D* v2, coord3D* v3, coord3D* result)
{
	// Resultant vectors a (v1 -> v2) and b (v1 -> v3)
	float ax = v2->x - v1->x;
	float ay = v2->y - v1->y;
	float az = v2->z - v1->z;
	float bx = v3->x - v1->x;
	float by = v3->y - v1->y;
	float bz = v3->z - v1->z;

	// Cross product
	result->x = ay * bz - az * by;
	result->y = az * bx - ax * bz;
	result->z = ax * by - ay * bx;

	// Convert to unit vector
	float mag = sqrt(result->x * result->x + result->y * result->y + result->z * result->z);
	result->x /= mag;
	result->y /= mag;
	result->z /= mag;

	//Returns resultant norm by arguments list
}