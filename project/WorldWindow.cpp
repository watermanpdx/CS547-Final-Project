/*
 * CS559 Maze Project
 *
 * Class file for the WorldWindow class.
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

/*
 * Notes on usage:
 * Tab - Change camera view
 * Right/Left Arrow - change time of day lighting
 * Up/Down Arrow - increase/decrease mountain subdivision
 */

#include "WorldWindow.h"
#include <Fl/math.h>
#include <Fl/gl.h>
#include <GL/glu.h>
#include <stdio.h>

// Debug control - enable/disable scene objects
const bool WALL = true;
const bool FOREST = true;
const int  TREES_PER_PATCH = 50;
const bool MOUNTAIN = true;
const bool BIRDS = true;
const bool ARCH = true;

const double WorldWindow::FOV_X = 45.0;

WorldWindow::WorldWindow(int x, int y, int width, int height, char *label)
	: Fl_Gl_Window(x, y, width, height, label)
{
    button = -1;

    // Initial viewing parameters.
    phi = 45.0f;
    theta = 0.0f;
    dist = 100.0f;
    x_at = 0.0f;
    y_at = 0.0f;

}

void
WorldWindow::draw(void)
{
    double  eye[3];
	float   color[4];	//Modified, dir[4] replaced with member variable

    if ( ! valid() )
    {
	// Stuff in here doesn't change from frame to frame, and does not
	// rely on any coordinate system. It only has to be done if the
	// GL context is damaged.

	double	fov_y;

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Turn on back face culling. Faces with normals away from the viewer
	// will not be drawn.
	glEnable(GL_CULL_FACE);

	// Enable lighting with one light.
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// Ambient and diffuse lighting track the current color.
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	// Turn on normal vector normalization. You don't have to give unit
	// normal vector, and you can scale objects.
	glEnable(GL_NORMALIZE);

	// Set up the viewport.
	glViewport(0, 0, w(), h());

	// Set up the persepctive transformation.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	fov_y = 360.0f / M_PI * atan(h() * tan(FOV_X * M_PI / 360.0) / w());
	gluPerspective(fov_y, w() / (float)h(), 1.0, 1000.0);

	// Do some light stuff. Diffuse color, and zero specular color
	// turns off specular lighting.
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT_COLORS[time_day]);
	color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_SPECULAR, color);

	// Initialize all the objects.
	ground.Initialize();
	if (WALL) { wall.Initialize(); }
	if (FOREST) {
		forest_b.SetPatch(-40.0f, -20.0f, -40.0f, 40.0f, TREES_PER_PATCH);
		forest_b.Initialize();
		forest_l.SetPatch(-30.0f, 40.0f, -40.0f, -20.0f, TREES_PER_PATCH);
		forest_l.Initialize();
		forest_r.SetPatch(-30.0f, 40.0f, 20.0f, 40.0f, TREES_PER_PATCH);
		forest_r.Initialize();
	}
	if (MOUNTAIN) { mountain.Initialize(); }
	if (BIRDS) {
		bird_track_1.Initialize();
		bird_track_1.SetBirdPhase(0.5f); //Stop flaps from syncing
		bird_track_2.Initialize();
		bird_track_3.SetBirdPhase(1.3f);
		bird_track_3.Initialize();
	}
	if (ARCH) { 
		arch.SetPosition(45.0f, 0.0f, 0.0f);
		arch.Initialize();
	}
    }

    // Stuff out here relies on a coordinate system or must be done on every
    // frame.

    // Clear the screen. Color and depth.
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Set up the viewing transformation. The viewer is at a distance
    // dist from (x_at, y_ay, 2.0) in the direction (theta, phi) defined
    // by two angles. They are looking at (x_at, y_ay, 2.0) and z is up.
    eye[0] = x_at + dist * cos(theta * M_PI / 180.0) * cos(phi * M_PI / 180.0);
    eye[1] = y_at + dist * sin(theta * M_PI / 180.0) * cos(phi * M_PI / 180.0);
    eye[2] = 2.0 + dist * sin(phi * M_PI / 180.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//Change view based on view selection
	coord3D cam_pos = { eye[0], eye[1], eye[2] };
	coord3D cam_ctr = { x_at, y_at, 2.0f };
	coord3D* p_pos = &cam_pos;
	coord3D* p_ctr = &cam_ctr;
	switch (view_selection)
	{
	case 0:		//Standard view
		//Do nothing
		break;
	case 1:		//Bird view
		p_pos = bird_track_1.GetBirdPositionPointer();
		p_ctr = bird_track_1.GetBirdDirectionPointer();
		break;
	case 2:		//Low view
		p_pos->x = 50.0f; p_pos->y = 0.0f; p_pos->z = 1.0f;
		p_ctr->x = 0.0f; p_ctr->y = 0.0f; p_ctr->z = 8.0f;
		break;
	default:
		//Do nothing
		break;
	}
    gluLookAt(	p_pos->x, p_pos->y, p_pos->z,
				p_ctr->x, p_ctr->y, p_ctr->z,
				0.0f, 0.0f, 1.0f);	

    // Position the light source. This has to happen after the viewing
    // transformation is set up, so that the light stays fixed in world
    // space. This is a directional light - note the 0 in the w component.
    glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_DIRS[time_day]);

    // Draw stuff. Everything.
    ground.Draw();
	if (WALL) { wall.Draw(); }
	if (FOREST) {
		forest_b.Draw();
		forest_l.Draw();
		forest_r.Draw();
	}
	if (MOUNTAIN) { mountain.Draw(); }
	if (BIRDS) {
		bird_track_1.Draw();
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 10.0f);
			glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
			glScalef(0.8f, 0.8f, 1.0f);
			bird_track_2.Draw();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 8.0f);
			glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);
			glScalef(0.9f, 0.9f, 1.0f);
			bird_track_3.Draw();
		glPopMatrix();
	}
	if (ARCH) { arch.Draw(); }

	// Set the lighting color and direction based on time of day (selected by user)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT_COLORS[time_day]);
	glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_DIRS[time_day]);
	glClearColor(SKY_COLORS[time_day][0],
		SKY_COLORS[time_day][1],
		SKY_COLORS[time_day][2],
		SKY_COLORS[time_day][3]);
}

void
WorldWindow::Drag(float dt)
{
    int	    dx = x_down - x_last;
    int     dy = y_down - y_last;

    switch ( button )
    {
      case FL_LEFT_MOUSE:
	// Left button changes the direction the viewer is looking from.
	theta = theta_down + 360.0f * dx / (float)w();
	while ( theta >= 360.0f )
	    theta -= 360.0f;
	while ( theta < 0.0f )
	    theta += 360.0f;
	phi = phi_down + 90.0f * dy / (float)h();
	if ( phi > 89.0f )
	    phi = 89.0f;
	if ( phi < -5.0f )
	    phi = -5.0f;
	break;
      case FL_MIDDLE_MOUSE:
	// Middle button moves the viewer in or out.
	dist = dist_down - ( 0.5f * dist_down * dy / (float)h() );
	if ( dist < 1.0f )
	    dist = 1.0f;
	break;
      case FL_RIGHT_MOUSE: {
	// Right mouse button moves the look-at point around, so the world
	// appears to move under the viewer.
	float	x_axis[2];
	float	y_axis[2];

	x_axis[0] = -(float)sin(theta * M_PI / 180.0);
	x_axis[1] = (float)cos(theta * M_PI / 180.0);
	y_axis[0] = x_axis[1];
	y_axis[1] = -x_axis[0];

	x_at = x_at_down + 100.0f * ( x_axis[0] * dx / (float)w()
				    + y_axis[0] * dy / (float)h() );
	y_at = y_at_down + 100.0f * ( x_axis[1] * dx / (float)w()
				    + y_axis[1] * dy / (float)h() );
	} break;
      default:;
    }
}

bool
WorldWindow::Update(float dt)
{
    // Update the view. This gets called once per frame before doing the
    // drawing.

    if ( button != -1 ) // Only do anything if the mouse button is down.
	Drag(dt);

    // Animate objects.
	if (BIRDS) {
		bird_track_1.Update(dt);
		bird_track_2.Update(dt);
		bird_track_3.Update(dt);
	}

    return true;
}

int
WorldWindow::handle(int event)
{
    // Event handling routine. Only looks at mouse events.
    // Stores a bunch of values when the mouse goes down and keeps track
    // of where the mouse is and what mouse button is down, if any.
	switch ( event )
    {
      case FL_PUSH:
        button = Fl::event_button();
	  case FL_KEYDOWN:
		key_button = Fl::event_key();
		switch (key_button)
		{
		// Up/Down arrows control subdivision level of mountain
		  case FL_Up:
			  mountain.IncreaseSubdivision();
			  break;
		  case FL_Down:
			  mountain.DecreaseSubdivision();
			  break;
		// Left/Right arrows control time of day
		  case FL_Right:
			  ++time_day;
			  time_day = (time_day % MAX_TIME_DAY + MAX_TIME_DAY) % MAX_TIME_DAY;	//Wrap around if over
			  break;
		  case FL_Left:
			  --time_day;
			  time_day = (time_day % MAX_TIME_DAY + MAX_TIME_DAY) % MAX_TIME_DAY;	//Wrap around if under
			  break;

		// Change the camera view
		  case FL_Tab:
			  ++view_selection;
			  view_selection = (view_selection % MAX_VIEW_SELECTIONS);
			  break;

		//Default
		  default:
			  break;
		}
	x_last = x_down = Fl::event_x();
	y_last = y_down = Fl::event_y();
	phi_down = phi;
	theta_down = theta;
	dist_down = dist;
	x_at_down = x_at;
	y_at_down = y_at;
	return 1;
      case FL_DRAG:
	x_last = Fl::event_x();
	y_last = Fl::event_y();
	return 1;
      case FL_RELEASE:
        button = -1;
	return 1;
    }

    // Pass any other event types on the superclass.
    return Fl_Gl_Window::handle(event);
}


