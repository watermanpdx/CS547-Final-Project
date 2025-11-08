/*
 * CS559 Maze Project
 *
 * Class header file for the WorldWindow class. The WorldWindow is
 * the window in which the viewer's view of the world is displayed.
 *
 * (c) Stephen Chenney, University of Wisconsin at Madison, 2001-2002
 *
 * Modified ==============================================================
 * Author:	Tayte Waterman
 * Course:	CS547 - Computer Graphics, Fall Term
 * Assgn:	Final Project
 * Org:		Portland State University
 * Date:	Nov 27, 2023
 */

#ifndef _WORLDWINDOW_H_
#define _WORLDWINDOW_H_

#include <Fl/Fl.h>
#include <Fl/Fl_Gl_Window.h>
#include "Ground.h"
#include "Track.h"
#include "Wall.h"
#include "Forest.h"
#include "Mountain.h"
#include "Bird.h"
#include "Arch.h"


// Subclass the Fl_Gl_Window because we want to draw OpenGL in here.
class WorldWindow : public Fl_Gl_Window {
    public:
	// Constructor takes window position and dimensions, the title.
	WorldWindow(int x, int y, int w, int h, char *label);

	// draw() method invoked whenever the view changes or the window
	// otherwise needs to be redrawn.
	void	draw(void);

	// Event handling method. Uses only mouse events.
	int	handle(int);

	// Update the world according to any events that have happened since
	// the last time this method was called.
	bool	Update(float);

    private:
	// The ground object.
	Ground	ground;
	//Flying birds
	Track	bird_track_1;
	Track	bird_track_2;
	Track	bird_track_3;
	//Park wall
	Wall	wall;
	//Forest(s)
	Forest	forest_b;
	Forest	forest_l;
	Forest	forest_r;
	//Mountain
	Mountain mountain;
	//Arch
	Arch arch;

	//Lighting, direction, and sky parameters for time-of-day lighting
	const int MAX_TIME_DAY = 4;
	const float LIGHT_COLORS[4][4] = {	{1.0f, 1.0f, 1.0f, 1.0f},	//Day
										{1.0f, 0.6f, 0.5f, 1.0f},	//Dusk
										{0.25f, 0.25f, 0.4f, 1.0f},	//Night
										{1.0f, 0.6f, 0.5f, 1.0f} };	//Morning
	const float LIGHT_DIRS[4][4] = {	{1.0f, 1.0f, 1.0f, 0.0f},		//Day
										{1.0f, 1.0f, 0.5f, 0.0f},		//Dusk
										{-1.0f, 0.0f, 1.0f, 0.0f},		//Night
										{-1.0f, -1.0f, 0.5f, 0.0f} };	//Morning
	const float SKY_COLORS[4][4] = {	{0.529f, 0.808f, 0.922f, 1.0f},		//Day
										{0.933f, 0.565f, 0.384f, 1.0f},		//Dusk
										{0.110f, 0.103f, 0.240f, 1.0f},		//Night
										{0.933f, 0.565f, 0.384f, 1.0f} };	//Morning
	int time_day = 0;	//Track time of day

	const int MAX_VIEW_SELECTIONS = 3;
	int view_selection = 0;

	static const double FOV_X; // The horizontal field of view.

	float	phi;	// Viewer's inclination angle.
	float	theta;	// Viewer's azimuthal angle.
	float	dist;	// Viewer's distance from the look-at point.
	float	x_at;	// The x-coord to look at.
	float	y_at;	// The y-coord to look at. z_at is assumed 2.0.

	int     button;	// The mouse button that is down, -1 if none.
	int		key_button;
	int	x_last;	// The location of the most recent mouse event
	int	y_last;
	int	x_down; // The location of the mouse when the button was pushed
	int	y_down;
	float   phi_down;   // The view inclination angle when the mouse
			    // button was pushed
	float   theta_down; // The view azimuthal angle when the mouse
			    // button was pushed
	float	dist_down;  // The distance when the mouse button is pushed.
	float	x_at_down;  // The x-coord to look at when the mouse went down.
	float	y_at_down;  // The y-coord to look at when the mouse went down.

	void	Drag(float);	// The function to call for mouse drag events
};


#endif
