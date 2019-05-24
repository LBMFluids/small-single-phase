#include <algorithm>
#include <vector>
#include <array>
#include "../../geometry.h"
#include "../../fluid.h"
#include "../common/lbm_tests.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

// Print test outcome
void tst_pass(bool val, const std::string msg);

bool just_fluid();
bool fluid_w_walls();
bool fluid_w_object();
bool fluid_w_array();

int main()
{
	tst_pass(just_fluid(), "Just fluid");
	tst_pass(fluid_w_walls(), "Fluid with walls");
	tst_pass(fluid_w_object(), "Fluid with object");
	tst_pass(fluid_w_array(), "Fluid with array");
}

bool just_fluid()
{
	// Objects
	Geometry<5,3> geom;
	Fluid<5,3,9> fluid("coffee");
	LBM<5,3,9> lbm(fluid);

	// Force
	const double dPdL = 1e-3;
	std::array<double,9> force = {0, 0, -1, 0, 1, -1, -1, 1, 1};
	std::for_each(force.begin(), force.end(), [&dPdL](double &f){ f*=(-dPdL*(1.0/6.0)); });
	
	// Initialization
	fluid.simple_ini(geom, 1.5);

	// Matlab input data for comparison
	// (needed only once, then reused) 	
	geom.write("./comp_data/just_fluid/geom.txt");
	fluid.write_f("./comp_data/just_fluid/f_cpp");

	lbm.f_equilibrium();
	lbm.collide(fluid);
	lbm.add_volume_force(geom, fluid, force);
	lbm.stream(geom, fluid);

	// Comparison
	return compare_f("./comp_data/just_fluid/f_cor", fluid, 1e-5);
}

bool fluid_w_walls()
{
	Geometry<5,3> geom;
	Fluid<5,3,9> fluid;
	LBM<5,3,9> lbm(fluid);

	// Force
	const double dPdL = 1e-3;
	std::array<double,9> force = {0, 0, -1, 0, 1, -1, -1, 1, 1};
	std::for_each(force.begin(), force.end(), [&dPdL](double &f){ f*=(-dPdL*(1.0/6.0)); });

	geom.add_walls();
	fluid.simple_ini(geom, 1.5);

	geom.write("./comp_data/fluid_w_walls/geom.txt");
	fluid.write_f("./comp_data/fluid_w_walls/f_cpp");

	lbm.f_equilibrium();
	lbm.collide(fluid);
	lbm.add_volume_force(geom, fluid, force);
	lbm.stream(geom, fluid);
	
	return compare_f("./comp_data/fluid_w_walls/f_cor", fluid, 1e-5);
}

bool fluid_w_object()
{

	bool feq_cor = 0, f_cor = 0;

	Geometry<40,50> geom;
	geom.add_ellipse(10,10,20,20);
	geom.add_walls();

	Fluid<40,50,9> fluid;
	fluid.simple_ini(geom, 1.5);

	LBM<40,50,9> lbm(fluid);

	// Force
	const double dPdL = 1e-3;
	std::array<double,9> force = {0, 0, -1, 0, 1, -1, -1, 1, 1};
	std::for_each(force.begin(), force.end(), [&dPdL](double &f){ f*=(-dPdL*(1.0/6.0)); });

	geom.write("./comp_data/fluid_w_object/geom.txt");
	fluid.write_f("./comp_data/fluid_w_object/f_cpp");

	lbm.f_equilibrium();
	lbm.collide(fluid);
	lbm.add_volume_force(geom, fluid, force);
	lbm.stream(geom, fluid);

	return compare_f("./comp_data/fluid_w_object/f_cor", fluid, 1e-5);}

bool fluid_w_array()
{
	// Matlab call
	// f_eq_comp('./array/geom_arr.txt', './array/f_arr', './array/feq_cor')
	
	bool feq_cor = 0, f_cor = 0;

	// Geometry setup
	Geometry<56,100> geom;
	// First array
	std::vector<size_t> rect_1 = {2,4};
	std::vector<size_t> array_1 = {4, 58, 15, 50, 10, 5};
	geom.add_array(rect_1, array_1, "rectangle", 60);
	// Second array
	std::vector<size_t> rect_2 = {4,2};
	std::vector<size_t> array_2 = {8, 52, 60, 96, 3, 5};
	geom.add_array(rect_2, array_2, "square", 75);
	
	// Fluid setup
	Fluid<56,100,9> fluid;
	fluid.simple_ini(geom, 1.5);

	// Simulation
	LBM<56,100,9> lbm(fluid);

	// Force
	const double dPdL = 1e-3;
	std::array<double,9> force = {0, 0, -1, 0, 1, -1, -1, 1, 1};
	std::for_each(force.begin(), force.end(), [&dPdL](double &f){ f*=(-dPdL*(1.0/6.0)); });

	geom.write("./comp_data/fluid_w_array/geom.txt");
	fluid.write_f("./comp_data/fluid_w_array/f_cpp");

	lbm.f_equilibrium();
	lbm.collide(fluid);
	lbm.add_volume_force(geom, fluid, force);
	lbm.stream(geom, fluid);

	return compare_f("./comp_data/fluid_w_array/f_cor", fluid, 1e-5);
}

// Prints test name tname with color that depends on val being
// true or false
void tst_pass(bool val, const std::string tname)
{
	if (!val)
		std::cout << "\033[1;35m  - " << tname << " test failed\033[0;0m" <<std::endl;
	else
		std::cout << "\033[1;32m  - " << tname << " test passed\033[0;0m" <<std::endl;
}
