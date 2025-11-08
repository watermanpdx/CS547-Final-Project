/*
 * Mountain.h: Header file for mountain class
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 25, 2023
 */

#ifndef _MOUNTAIN_H_
#define _MOUNTAIN_H_

#include <Fl/gl.h>
#include "Datatypes.h"

#define MAX_LEVEL 5

class Mountain {
private:
	//Multiple display lists for multiple subdivisions
	GLubyte display_lists[MAX_LEVEL] = { NULL, NULL, NULL, NULL, NULL };
	GLuint  texture_obj;
	bool	initialized = false;

	//Control subdivision pre-computing and current selection
	int		computed_level = 0;
	int		level = 0;

	//How strong new vertices randomly vary
	const float VARIANCE = 2.0f;

	//Subdivision meshes
	Mesh*	meshes[MAX_LEVEL] = { NULL, NULL, NULL, NULL, NULL };

	//Private methods used to compute geometry, not needed externally
	void	SeedMesh(void);
	int		Subdivide(void);
	int		UpdateVertices(coord3D* vertices, Mesh* seed, int** indices, int i, int j, int &index);
	bool	BuildDisplayList(int index);

public:
	Mountain(void);
	~Mountain(void);
	
	bool	Initialize(void);
	void	Draw(void);

	//Control current subdivision level
	int		IncreaseSubdivision(void);
	int		DecreaseSubdivision(void);
};

#endif