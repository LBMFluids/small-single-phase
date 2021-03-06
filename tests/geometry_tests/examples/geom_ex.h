#include <iostream>
#include <vector>
#include <string>
#include <array>
#include "../../geometry.h"

// Aliases for pointers to functionis
template<std::size_t Lx, std::size_t Ly>
using pof = void(Geometry<Lx, Ly>::*)(size_t, size_t, size_t, size_t);

// Object creation checks
// Checks if particular function correctly throws on object being created out of domain bounds
template<std::size_t Lx, std::size_t Ly>
bool geom_test_out_of_bounds(Geometry<Lx, Ly>, pof<Lx, Ly>, std::array<size_t, 4>);

// -----------------
// Object creation
// -----------------
// Verify the response to out-of-bounds
// scenario (for the computational domain)
template<std::size_t Lx, std::size_t Ly>
bool geom_test_out_of_bounds(Geometry<Lx, Ly> geom, pof<Lx, Ly> make_object, std::array<size_t, 4> params)
{
	std::cout << "----- Checking out-of-bounds response" << std::endl;
	try{
		(geom.*make_object)(params[0], params[1], params[2], params[3]);
	}catch (...){
		std::cout << "Out-of-bounds detected -----" << std::endl;
		return true;	
	}
	return false;	
}

// Final condense tests
bool test_object_bounds(std::string);
void single_rectangle_test();
void single_ellipse_test();
void rectangle_array_test();
void ellipse_array_test();
void circle_array_test();
void square_array_test();
void wall_test();
void staggered_square_array_test();
void staggered_circular_array_test();
void read_test();

// Examples
void ex_single_square();
void ex_single_circle();
void ex_step();
void ex_reg_squares();
void ex_reg_ellipses();
void ex_stag_rectancles();
void ex_stag_circ();
void ex_read_write();

// Arrays - wrappers
template<std::size_t Lx, std::size_t Ly>
void array_test(Geometry<Lx,Ly> geom, 
			std::vector<size_t> obj_params, 
			std::vector<size_t> arr_params, size_t alpha, 
			std::string fname, std::string object)
{
	if (!alpha)
		geom.add_array(obj_params, arr_params, object);
	else
		geom.add_array(obj_params, arr_params, object, alpha);
	geom.write(fname);
}




