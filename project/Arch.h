/*
 * Block.h: Header file for arch class
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 28, 2023
 */

#ifndef _ARCH_H_
#define _ARCH_H_

#include <Fl/gl.h>
#include "Datatypes.h"

class Arch {
private:
    GLubyte display_list;
    GLuint  texture_obj;
    bool    initialized;

    const float PI = 3.141593f;

    const int INNER_FACES = 8;
    const int OUTER_FACES = 16;
    const int LEG_DIVS = 3;
    const float height = 8.5f;
    const float radius = 5.0f;
    const float thickness = 1.1f;
    const float texture_scale = 0.5f;

    coord3D scale = { 1.0f, 1.0f, 1.0f };       //User provided scaling, also controls texture scale
    coord3D position = { 0.0f, 0.0f, 0.0f };    //User provided position

public:
    Arch(void);
    ~Arch(void);

    bool    Initialize(void);
    void    Draw(void);

    //Set object parameters and position
    void    SetPosition(float x, float y, float z);
    void    SetScale(float x, float y, float z);
};

#endif