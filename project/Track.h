/*
 * Track.h: Header file for a class that draws the train and its track.
 *
 * (c) 2001: Stephen Chenney, University of Wisconsin at Madison.
 * 
 * Modified ==============================================================
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 27, 2023
 */

#ifndef _TRAINTRACK_H_
#define _TRAINTRACK_H_

#include <Fl/gl.h>
#include "CubicBspline.h"
#include "Bird.h"
#include "Datatypes.h"

class Track {
  private:
    GLubyte 	    track_list;	    // The display list for the track.
    GLubyte 	    train_list;	    // The display list for the train.
    bool    	    initialized;    // Whether or not we have been initialized.
    CubicBspline    *track;	        // The spline that defines the track.
    float	    posn_on_track;      // The train's parametric position on the
				                    // track.
    float	    speed;	            // The train's speed, in world coordinates
    coord3D bird_position = { 0.0f, 0.0f, 0.0f };
    coord3D bird_direction = { 0.0f, 0.0f, 0.0f };

    static const int	TRACK_NUM_CONTROLS;	// Constants about the track.
    static const float 	TRACK_CONTROLS[][3];
    static const float 	TRAIN_ENERGY;
    const bool TRACK_VISIBLE = false;  //Control whether to display track or not 

    Bird bird;  //Bird in place of train

  public:
    Track(void) { initialized = false; posn_on_track = 0.0f; speed = 0.0f; };
    ~Track(void);

    bool    Initialize(void);	// Gets everything set up for drawing.
    void    Update(float);	// Updates the location of the train
    void    Draw(void);		// Draws everything.

    //Set bird flap phase to prevent birds wings too much in sync
    void    SetBirdPhase(float offset);

    //Get bird position and orientation
    coord3D*    GetBirdPositionPointer(void);
    coord3D*    GetBirdDirectionPointer(void);
};


#endif

