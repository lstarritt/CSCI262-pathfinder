#ifndef _PATHFINDER_H
#define _PATHFINDER_H

/*
    pathfinder.h
        
    class definition for the pathfinder class.

    assignment: CSCI 262 Project 1 - Pathfinder        

    author: Lane Starritt

    last modified: 2017-09-13
*/

#include <string>
#include <istream>
#include <vector>

#include "drawing_window.h"

using namespace std;

/******************************************************************************
   class pathfinder

   Given access to a source of topographical data (a file specifying width and
   height and listing elevation data for the resulting grid), a pathfinder
   object can:
     - read in the data
     - display the data as a grayscale image, with gray level representing
       relative elevation, on a drawing_window
     - compute and display the optimal path from all western starting points
     - compute and display the single best optimal path from west to east

******************************************************************************/

class pathfinder {
public:
    // constructor - optional, but usually a good idea to set up your
    // internal state
	pathfinder();

    // public methods - these are required, but you can add additional as
    // needed
	void initialize(istream &in, string title);
	void draw_map();
	int draw_optimal_paths();

    // This is a convenience - lets the user of the class know when the
    // user has closed the drawing window (used by main() in this project)
	bool is_open() { return _window.is_open(); }

private:
    // if you rename this variable, be sure to change is_open() as well
    drawing_window _window;

    // TODO: add your internal data, methods, etc. here

    int _cols;
    int _rows;
    int _minEl;
    int _maxEl;
    vector <vector <int> > _victor;//What's out vector, Victor?
    vector <vector <int> > _diffArray;
    vector <vector <char>> _dir;//N for NE, S for SE, E for East

};

#endif
