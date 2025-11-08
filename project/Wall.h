/*
 * Wall.h: Header file for  wall class
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 15, 2023
 */

#ifndef _WALL_H_
#define _WALL_H_

#include "Block.h"

class Wall {
private:
    int     comp_len;
    Block*  components;
    bool    initialized;

public:
    Wall(void);
    ~Wall(void);

    bool    Initialize(void);
    void    Draw(void);
};

#endif