#include <vector>
#include <array>
#include "../../geometry.h"
#include "../../fluid.h"
#include "fluid_test_core.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

// Test setups
void empty();
void walls();
void stag_array();

// Driver for fluid testing
int main()
{
	empty();
	walls();
	stag_array();
}

void empty()
{
	std::cout << "\033[1;31m Fluid testing with empty geometry: \033[0m" << std::endl;
	Geometry<5,3> geom;
	fluid_run_all<5,3,9>(geom); 
}

void walls()
{
	std::cout << "\033[1;31m Fluid testing with geometry with walls: \033[0m" << std::endl;
	Geometry<5,3> geom;
	geom.add_walls();
	fluid_run_all<5,3,9>(geom); 
}

void stag_array()
{
	std::cout << "\033[1;31m Fluid testing with geometry with staggered arrays: \033[0m" << std::endl;
	// Geometry construction
	Geometry<56,100> geom;
	// First array
	std::vector<size_t> rect = {2,4};
	std::vector<size_t> array_1 = {4, 58, 15, 50, 10, 5};
	geom.add_array(rect, array_1, "rectangle", 60);
	// Second array
	std::vector<size_t> circ = {4,4};
	std::vector<size_t> array_2 = {8, 52, 60, 96, 3, 5};
	geom.add_array(circ, array_2, "circle", 75);
	// Test suite
	fluid_run_all<56,100,9>(geom); 
}

