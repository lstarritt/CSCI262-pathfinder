#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

#include "pathfinder.h"

using namespace std;

int main(int argc, char* argv[]) {
	cout << "Welcome to Pathfinder!" << endl;
	cout << "======================" << endl << endl;
	
	// get the filename to process first
	string data_file_name;
	cout << "Please enter the map data filename, e.g., \"colorado.dat\": ";
	cin >> data_file_name;

	// open the file
	ifstream data(data_file_name);
	while (!data) {
		cerr << "Error opening file \"" << data_file_name << "\"" << endl;
		data.clear();
		cout << "Please enter the data filename, e.g., \"colorado.dat\": ";
		cin >> data_file_name;
		data.open(data_file_name);
	}
		
	// create a pathfinder object and initialize
	pathfinder p;
	p.initialize(data, "Pathfinder");

	// close the data file
	data.close();

	// draw the map 
	p.draw_map();

	// show optimal paths from all starting points, and
	// print the best path elevation change
	int optimal = p.draw_optimal_paths();
	cout << "Best optimal path total: " << optimal << endl;

	// wait until user closes window or hits Ctrl-C
	while (p.is_open());
}
