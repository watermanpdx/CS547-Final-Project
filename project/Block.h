/*
 * Block.h: Header file for block class
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 11, 2023
 */

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <Fl/gl.h>
#include "Datatypes.h"

class Block {
private:
    GLubyte display_list;
    GLuint  texture_obj;
    bool    initialized;

    const float texture_scale = 0.15f;          //Used to control texture scale
    coord3D scale = { 1.0f, 1.0f, 1.0f };       //User provided scaling, also controls texture scale
    coord3D position = { 0.0f, 0.0f, 0.0f };    //User provided position

public:
    //Constructor and destructor
    Block(void);
    ~Block(void);

    bool    Initialize(void);
    void    Draw(void);

    //Object parameter control methods
    void    SetScale(float x, float y, float z);
    void    SetPosition(float x, float y, float z);
};

#endif