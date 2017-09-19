/*
        
    Method implementations for the pathfinder class.
    
    assignment: CSCI 262 Project 1 - Pathfinder        

    author: Lane Starritt

    last modified: 2017-09-18
*/

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>

#include "pathfinder.h"

using namespace std;

// constructor
pathfinder::pathfinder() { ; }


// initialize()
//
// Take in data from a provided input stream and store for later use.
// Set the size of the drawing window and open it.
void pathfinder::initialize(istream &in, string title) {
	// DONE: read in header to get _rows and columns
    _cols = 0;
    _rows = 0;
    string temp;

    in.ignore(10, ' ');
    in >> temp;
    _cols = stoi(temp, nullptr, 10);
    cout << _cols << endl;

    in.ignore(10, ' ');
    in >> temp;
    _rows = stoi(temp, nullptr, 10);
    cout << _rows << endl;

	// DONE: read in elevation data, set up data structure(s)
    _minEl = 999999;
    _maxEl = 0;
    for(int j = 0; j < _rows; j++){
        vector<int> vRows;
        for(int i =0; i < _cols; i++){
            in >> temp;
            vRows.push_back(stoi(temp, nullptr, 10));
            if (vRows.back() > _maxEl){
                _maxEl = vRows.back();
            } else if(vRows.back() < _minEl){
                _minEl = vRows.back();
            }
        }
        _victor.push_back(vRows);
    }
    cout << "maxEl = " << _maxEl << " minEl = " << _minEl << endl;
    cout << "Done with Data Initilization!" << endl;


	// DONE: optional - track max/min elevations as you process data
	// Could also do this in separate methods after the fact

	// DONE: modify the code below to open the correctly sized window
    _window.initialize(_cols, _rows);
    _window.open();
    cout << "Window Opened!" << endl;
}


// draw_map()
//
// Display map data as a grayscale image on drawing window.  Grayscale
// level represents elevation relative to the min/max elevation.
void pathfinder::draw_map() { 
    cout << "Map Draw begun!"<< endl;
	// DONE: rewrite this method
    int tempInt1 = (_maxEl - _minEl);
    for (int i = 0; i < _rows; i++){
        for (int j = 0; j < _cols; j++) {
            int tempInt2 = float(((_victor[i][j] - _minEl) / float(tempInt1)) * 256); //math to make it RGB clean
            _window.set_color(j, i, tempInt2, tempInt2, tempInt2);
        }
    }
    cout << "Done Drawing Window!" << endl;
}


// draw_optimal_paths()
//
// Discover and draw optimal paths from west to east using dynamic programming.
int pathfinder::draw_optimal_paths() { 
	// DONE: compute minimum cost for every point on map, keeping track
	// of next move necessary to attain min cost
	// Consider adding helper method(s) to do this work
     
    _diffArray = vector<vector<int>>(_rows, vector<int>(_cols));
    _dir = vector<vector<char>>(_rows, vector<char>(_cols));
    for (int i = _cols - 2; i >= 0; i--){
        for (int j = 0; j < _rows; j++) {
            int temp1 = 9999, temp2 = 9999, temp3 = 9999;
            if((_rows - j - 1 > 0) && (j > 0)){
                temp1 = abs(_victor[j][i] - _victor[j - 1][i + 1]) + _diffArray[j - 1][i + 1];
                temp2 = abs(_victor[j][i] - _victor[j][i + 1]) + _diffArray[j][i + 1];
                temp3 = abs(_victor[j][i] - _victor[j + 1][i + 1]) + _diffArray[j + 1][i + 1];
            }else if((_rows - j - 1) == 0){
                temp1 = abs(_victor[j][i] - _victor[j - 1][i + 1]) + _diffArray[j - 1][i + 1];
                temp2 = abs(_victor[j][i] - _victor[j][i + 1]) + _diffArray[j][i + 1];
            }else if(j == 0){
                temp2 = abs(_victor[j][i] - _victor[j][i + 1]) + _diffArray[j][i + 1];
                temp3 = abs(_victor[j][i] - _victor[j + 1][i + 1]) + _diffArray[j + 1][i + 1];
            }
                //code to figure out which temp is lowest and insert total cost and tempVC
            if(temp1 <= temp2 && temp1 <= temp3){
                _dir[j][i] = 'N';
                _diffArray[j][i] = temp1;//comment this lone out for greedy
            }else if(temp2 <= temp1 && temp2 <= temp3){
                _dir[j][i] = 'E';
                _diffArray[j][i] = temp2;//comment this lin out for greedy
            }else if(temp3 < temp1 && temp3 < temp2){
                _dir[j][i] = 'S';
                _diffArray[j][i] = temp3;//commen tthis line outfor greedy
            }else{
                _dir[j][i] = 'E';
                _diffArray[j][i] = temp2;//comment this line out for greedy
                cout << j << ',' << i << "zz" << endl;
            }
        }
    }

	// DONE: plot optimal paths from every westmost point, in blue (or color
	// of your choice) - consider adding helper method(s) to do this work
    for (int i = 0; i < _rows; i++){
        int xDist = 0;
        int yDist = i;
        while(xDist < _cols){
            //cout << i << ';' << xDist << ',' << yDist << ':' << _rows << ',' << _cols << _dir[yDist][xDist] << endl;
            if(_dir[yDist][xDist] == 'N'){
                _window.set_color(++xDist, --yDist, 0, 0, 255);
                //cout << 'N' << endl;
            }else if(_dir[yDist][xDist] == 'E'){
                _window.set_color(++xDist, yDist, 0, 0, 225);
                //cout << 'E' << endl;
            }else if(_dir[yDist][xDist] == 'S'){
                _window.set_color(++xDist, ++yDist, 0, 0, 255);
                //cout << 'S' << endl;
            }else{
                xDist++;
                //cout << "Err - no valid dir at " << xDist << ',' << yDist << ':' << i << endl;
            }
        }    
    }

	// TODO: find/plot best of the optimal paths in yellow (or color of your
	// choice) - consider adding helper method(s) to do this work

    int best = 0;
    for(int i = 0; i < _rows; i++){
        if(_diffArray[i][0] < _diffArray[best][0]){
            best = i;
        }
        cout << best <<endl;
    }
    int xCount = 0;
    int yCount = best;
    while(xCount < _cols){
        if(_dir[yCount][xCount] == 'N'){ 
            _window.set_color(++xCount, --yCount, 0, 255, 0);
            //cout << 'N' << endl;
        }else if(_dir[yCount][xCount] == 'E'){
            _window.set_color(++xCount, yCount, 0, 255, 0);
            //cout << 'E' << endl;
         }else if(_dir[yCount][xCount] == 'S'){
            _window.set_color(++xCount, ++yCount, 0, 255, 0);
            //cout << 'S' << endl;
        }else{
            xCount++;
            //cout << "Err - no valid dir at " << xDist << ',' << yDist << ':' << i << endl;
        } 
    }

	// DONE: return cost of best cost path
    return _diffArray[best][0];
}

