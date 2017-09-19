/*
	drawing_window.cpp

	Implementation code for the drawing_window class.

	author: C. Painter-Wakefield

	last modified: 9/2/2017
*/

#include <string>
#include <thread>
#include <cmath> 
#include <ctime>

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include "drawing_window.h"

using namespace std;
using namespace sf;


// initialize: setup the patches collection 
void drawing_window::initialize(unsigned width, unsigned height) {
	_destroy();

	_width = width;
	_height = height;

	// calculate number of patches needed
	unsigned pw = ceil(width / double(PATCH_SIZE));
	unsigned ph = ceil(height / double(PATCH_SIZE));

	_patch_count = pw * ph;
	_patches = new patch[_patch_count];

	for (int x = 0; x < pw; x++) {
		for (int y = 0; y < ph; y++) {
			if (x == pw - 1 && width % PATCH_SIZE != 0) {
				_patches[x * ph + y].width = width % PATCH_SIZE;
			} else {
				_patches[x * ph + y].width = PATCH_SIZE;
			}
		
			if (y == ph - 1 && height % PATCH_SIZE != 0) {
				_patches[x * ph + y].height = height % PATCH_SIZE;
			} else {
				_patches[x * ph + y].height = PATCH_SIZE;
			}

			_patches[x * ph + y].x_off = x * PATCH_SIZE;
			_patches[x * ph + y].y_off = y * PATCH_SIZE;
			_patches[x * ph + y].dirty = true;
			_patches[x * ph + y].update();
		}
	}
}

// destroy: close things down, delete used memory 
void drawing_window::_destroy() {
	if (_window != NULL) {
		_window->close();
		delete _window;
	}
	if (_patches != NULL) {
		delete[] _patches;
	}
}

// open the RenderWindow and pass drawing off to a new thread 
void drawing_window::open(string title) {
	if (is_open()) {
		delete _window;
		close();
	}

	_window = new RenderWindow(VideoMode(_width, _height), title);
	_window->setActive(false);

	thread t(drawing_window::_draw, this);
	_draw_thread = &t;
	_draw_thread->detach();
}

// close the window 
void drawing_window::close() {
	if (_window != NULL && _window->isOpen()) {
		_window->close();
	}
}

// draw a point on the drawing window, which will later get drawn
// on the SFML RenderWindow 
void drawing_window::set_color(unsigned x, unsigned y, char r, char g, char b) {
	unsigned pw = ceil(_width / double(PATCH_SIZE));
	unsigned ph = ceil(_height / double(PATCH_SIZE));

	_patches[(x / PATCH_SIZE) * ph + (y / PATCH_SIZE)].set_point(x % PATCH_SIZE, y % PATCH_SIZE, r, g, b);
}


// run the SFML event loop, redrawing the window from the patch collection roughly
// 60 times per second
void drawing_window::_draw(drawing_window* dw) {

	clock_t last_draw = 0;

	// run the program as long as the window is open
	while (dw->_window->isOpen()) {
		// check all the window's events that were triggered 
		// since the last iteration of the loop
		sf::Event event;
		while (dw->_window->pollEvent(event)) {
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed) dw->_window->close();
		}
		if (!dw->_window->isOpen()) break;

		// check that enough time has passed to worry about redraw
		clock_t now = clock();
		if (last_draw != 0 && (now - last_draw) / double(CLOCKS_PER_SEC) < (1.0 / 60.0)) {
			unsigned millis_to_wait = ((1.0 / 60.0) - (now - last_draw) / double(CLOCKS_PER_SEC)) * 1000;
                        this_thread::sleep_for(chrono::milliseconds(millis_to_wait));
			continue;
		}

		// draw patches
		last_draw = now;
		for (int i = 0; i < dw->_patch_count; i++) {
			dw->_patches[i].draw(*(dw->_window));
		}

		// show on screen
		dw->_window->display();
	}
}

void drawing_window::patch::update() {
	mtx.lock(); // prevent changes to this patch while refreshing the vertex array
	if (dirty) {
		points.clear();
		points.setPrimitiveType(Triangles);
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				Color c(data[x][y][0], data[x][y][1], data[x][y][2]);
				points.append({{float(x + x_off), float(y + y_off)}, c});
				points.append({{float(x + x_off + 1), float(y + y_off)}, c});
				points.append({{float(x + x_off), float(y + y_off + 1)}, c});
				points.append({{float(x + x_off + 1), float(y + y_off)}, c});
				points.append({{float(x + x_off + 1), float(y + y_off + 1)}, c});
				points.append({{float(x + x_off), float(y + y_off + 1)}, c});
			}
		}
		dirty = false;
	}
	mtx.unlock();
}

