#include <algorithm>
#include <vector>
#include <array>
#include "../../geometry.h"
#include "../../fluid.h"
#include "lbm_tests.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

void just_fluid();
void fluid_w_walls();
void fluid_w_object();
void fluid_w_array();

int main()
{
	//just_fluid();	
	//fluid_w_walls();
	//fluid_w_object();
	fluid_w_array();
}

void just_fluid()
{
	// Matlab run
	//f_eq_comp('./empty/geom_empty.txt', './empty/f_empty', './empty/feq_cor')
	//col_forc_comp('./empty/geom_empty.txt', './empty/f_empty', 0.5,'./empty/f_cor') 

	// Objects
	Geometry<5,3> geom;
	Fluid<5,3,9> fluid("coffee", 0.66, 2.0);
	LBM<5,3,9> lbm(fluid);

	// Force
	const double dPdL = 1e-3;
	std::array<double,9> force = {0, 0, -1, 0, 1, -1, -1, 1, 1};
	std::for_each(force.begin(), force.end(), [&dPdL](double &f){ f*=(-dPdL*(1.0/6.0)); });
	
	// Testing flags
	bool feq_cor = 0, f_cor = 0;

	fluid.simple_ini(geom, 1.5);

	// Generate data for comparison 	
	geom.write("./empty/geom_empty.txt");
	fluid.write_f("./empty/f_empty");

	lbm.f_equilibrium();
	lbm.collide(fluid);
	lbm.add_volume_force(geom, fluid, force);
	lbm.stream(geom, fluid);

	// Actual comparison
	//feq_cor = compare_feq("./empty/feq_cor", lbm, 1e-5);
	//std::cout << feq_cor << std::endl;	
	f_cor = compare_f("./empty/f_cor", fluid, 1e-5);
	std::cout << f_cor << std::endl;
}

void fluid_w_walls()
{
	// Matlab run
	// f_eq_comp('./walls/geom_walls.txt', './walls/f_walls', './walls/feq_cor')
	
	Geometry<5,3> geom;
	Fluid<5,3,9> fluid;
	LBM<5,3,9> lbm(fluid);
	bool feq_cor = 0, f_cor = 0;

	// Force
	const double dPdL = 1e-3;
	std::array<double,9> force = {0, 0, -1, 0, 1, -1, -1, 1, 1};
	std::for_each(force.begin(), force.end(), [&dPdL](double &f){ f*=(-dPdL*(1.0/6.0)); });

	geom.add_walls();
	fluid.simple_ini(geom, 1.5);

	geom.write("./walls/geom_walls.txt");
	fluid.write_f("./walls/f_walls");

	lbm.f_equilibrium();
	lbm.collide(fluid);
	lbm.add_volume_force(geom, fluid, force);
	lbm.stream(geom, fluid);
	
	//feq_cor = compare_feq("./walls/feq_cor", lbm, 1e-5);
	//std::cout << feq_cor << std::endl;
	f_cor = compare_f("./walls/f_cor", fluid, 1e-5);
	//fluid.write_f("./walls/check");
	std::cout << f_cor << std::endl;
}

void fluid_w_object()
{
	// Matlab call
	// f_eq_comp('./obj/geom_obj.txt', './obj/f_obj', './obj/feq_cor')
	
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

	geom.write("./obj/geom_obj.txt");
	fluid.write_f("./obj/f_obj");

	lbm.f_equilibrium();
	lbm.collide(fluid);
	lbm.add_volume_force(geom, fluid, force);
	lbm.stream(geom, fluid);

	//feq_cor = compare_feq("./obj/feq_cor", lbm, 1e-5);
	//std::cout << feq_cor << std::endl;
	f_cor = compare_f("./obj/f_cor", fluid, 1e-5);
	std::cout << f_cor << std::endl;
}

void fluid_w_array()
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

	geom.write("./array/geom_arr.txt");
	fluid.write_f("./array/f_arr");

	lbm.f_equilibrium();
	lbm.collide(fluid);
	lbm.add_volume_force(geom, fluid, force);
	lbm.stream(geom, fluid);

	//feq_cor = compare_feq("./array/feq_cor", lbm, 1e-5);
	//std::cout << feq_cor << std::endl;
	f_cor = compare_f("./array/f_cor", fluid, 1e-5);
	std::cout << f_cor << std::endl;
}
