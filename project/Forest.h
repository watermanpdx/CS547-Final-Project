/*
 * Tree.h: Header file for forest class
 *
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 11, 2023
 */

#ifndef _FOREST_H_
#define _FOREST_H_

#include "Tree.h"

class Forest
{
	private:
		Tree*	components;
		bool    initialized = false;    // Whether or not we have been initialised.
        bool    patch_set = false;      // Whether forest perimeter and number of trees set

        // Forest perimeter and number of trees
        int     comp_len;
        float   perimeter_x_min = -10.0f;
        float   perimeter_x_max = -10.0f;
        float   perimeter_y_min = -10.0f;
        float   perimeter_y_max = -10.0f;

        // Range of tree sizes (fixed)
        const float H_MAX = 9.0f;
        const float H_MIN = 4.0f;
        const float R_MAX = 2.0f;
        const float R_MIN = 0.75f;
        const float T_MAX = 0.4f;
        const float T_MIN = 0.1f;

	public:
        //Constructor and destructor
        Forest(void);
        ~Forest(void);

        bool    Initialize(void);
        void    Draw(void);

        //Object parameter control methods
        void    SetPatch(float x_min, float x_max, float y_min, float y_max, int number_trees);
};

#endif