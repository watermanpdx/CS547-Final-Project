/*
 * Tree.h: Header file for tree class
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 11, 2023
 */

#ifndef _TREE_H_
#define _TREE_H_

#include <Fl/gl.h>
#include "Datatypes.h"

using namespace std;

class Tree {
private:
    static GLubyte foliage_list;    //Share list across class instances
    static GLubyte trunk_list;      //Share list across class instances
    GLuint  texture_obj;

    const float PI = 3.141593f;

    const static int FACES = 8; //Object sweep resolution
    static int instances;       //Keep track of total instances of tree

    //Tree attributes (parameters)
    coord3D position = { 0.0f, 0.0f, 0.0f };
    float height;       //Total tree height
    float radius;       //Tree "limbs" radius
    float trunk_ratio;  //Ratio of trunk height to total height

public:
    Tree(void);
    ~Tree(void);

    bool    Initialize(void);
    void    Draw(void);

    //Set object parameters and position
    void    SetPosition(float x, float y, float z);
    void    SetParameters(float height, float radius, float trunk_ratio);
};

#endif