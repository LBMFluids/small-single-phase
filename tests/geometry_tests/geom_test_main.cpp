#include <vector>
#include <array>
#include "../../geometry.h"
#include "geom_test_core.h"

// Last modified: 04/17/2019

using std::cout;
using std::endl;
using std::string;
using std::vector;

// Runs tests and saves geoms for further
// checking with Python
int main()
{
	// Single objects
	single_rectangle_test();
	single_ellipse_test();
		
	// Walls
	wall_test();

	// Arrays
	rectangle_array_test();	
	square_array_test();
	ellipse_array_test();
	circle_array_test();

	// Staggered arrays
	staggered_rec_array_test();
	staggered_circular_array_test();

	// Test reading geometry from file
	read_test();

	// Test bound checking
	std::string s("rectangle");
	std::cout << "\033[31;1;44m ----- Checking out-of-bounds response - rectangle\033[0m" << std::endl;
	bool bounds = test_object_bounds(s);
	cout << "Should be 1: " << bounds << endl;
	//
	s = "ellipse";
	std::cout << "\033[31;1;44m ----- Checking out-of-bounds response - ellipse\033[0m" << std::endl;
	bounds = test_object_bounds(s);
	cout << "Should be 1: " << bounds << endl;
}
