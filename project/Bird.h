/*
 * Bird.h: Header file for bird class
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 26, 2023
 */

#ifndef _BIRD_H_
#define _BIRD_H_

#include <Fl/gl.h>
#include "Datatypes.h"

class Bird {
private:
    //Bird components
    GLubyte list_tip;
    GLubyte list_mid;
    GLubyte list_base;
    GLubyte list_tail;
    GLubyte list_body;
    GLubyte list_beak;
    GLuint  texture_obj;
    bool    initialized;    // Whether or not we have been initialised.

    //Animation control contants and parameters
    const float RATE = 4.0f;    //Flap rate
    const float RANGE = 20.0f;  //Range of flapping motion
    const float PHASE = -0.6f;  //Offset each wing segment to look more natural
    float theta = 0.0f;         //Rotational angle to control flapping

    //Allows for scalar control as method
    coord3D scale = { 1.0f, 1.0f, 1.0f };

public:
    //Constructor and destructor
    Bird(void);
    ~Bird(void);

    bool    Initialize(void);
    void    Update(float dt);
    void    Draw(void);

    // Object parameter controls
    void    SetScale(float x, float y, float z);
    void    SetPhaseOffset(float offset);
};

#endif