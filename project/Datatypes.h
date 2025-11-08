/*
 * Datatypes.h: Common datatypes shared within project
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 18, 2023
 */

#ifndef _DATATYPES_H_
#define _DATATYPES_H_

//Cartesian points on arc + % of circle
typedef struct {
    float u;
    float x;
    float y;
} coordArc;

//Floating point 3D point
typedef struct {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
} coord3D;

//Triangle vertices, stored as indices
typedef struct {
    int v1 = 0;
    int v2 = 0;
    int v3 = 0;
} triangleIndices;

//Mesh class for storing model vertices, and face info for rendering
class Mesh {
public:
    int num_vertices = 0;
    int num_faces = 0;
    coord3D* vertices;
    triangleIndices* faces;

    Mesh(void);
    Mesh(coord3D* vert_arr, int vert_len, triangleIndices* face_arr, int face_len);
    ~Mesh(void);
};

void    Normal(coord3D* v1, coord3D* v2, coord3D* v3, coord3D* result);

#endif