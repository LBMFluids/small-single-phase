#include <string>
#include <vector>
#include "geom_test_core.h"

// 
// Functions for basic geometry tests 
// Last modified: 04/17/2019
//

std::string tst_path("./tmp_files/");

void single_rectangle_test()
{
	// Rectangle
	Geometry<20,50> geom;
	// a(dx), b(dy), xc, yc
	geom.add_rectangle(4,8,10,25);
	geom.write(tst_path + "rectangle.txt");
	// Square
	Geometry<20,50> geom2;
	geom2.add_rectangle(4,4,10,25);
	geom2.write(tst_path + "square.txt");
}

void single_ellipse_test()
{
	// Ellipse
	Geometry<200,500> geom;
	geom.add_ellipse(20,50,100,250);
	geom.write(tst_path + "ellipse.txt");
	// Circle
	Geometry<200,500> geom2;
	geom2.add_ellipse(20,20,100,250);
	geom2.write(tst_path + "circle.txt");
}

void rectangle_array_test()
{
	Geometry<70,30> geom;
	std::vector<size_t> obj = {8, 2};
	std::vector<size_t> array = {15, 63, 5, 20, 3, 3};	
	array_test(geom, obj, array, 0, tst_path + "rectangle_array.txt", "rectangle");
}

void square_array_test()
{
	Geometry<10,10> geom;
	std::vector<size_t> obj = {2, 2};
	std::vector<size_t> array = {2, 7, 2, 8, 2, 3};	
	array_test(geom, obj, array, 0, tst_path + "square_array.txt", "square");
}

void ellipse_array_test()
{
	Geometry<70,30> geom;
	std::vector<size_t> obj = {16, 4};
	std::vector<size_t> array = {15, 63, 5, 20, 3, 3};	
	array_test(geom, obj, array, 0, tst_path + "ellipse_array.txt", "ellipse");
}

void circle_array_test()
{
	Geometry<10,10> geom;
	std::vector<size_t> obj = {2, 2};
	std::vector<size_t> array = {2, 7, 2, 8, 2, 3};	
	array_test(geom, obj, array, 0, tst_path + "circle_array.txt", "circle");
}

void staggered_rec_array_test()
{
	Geometry<50,100> geom_1;
	std::vector<size_t> rec = {2,4};
	std::vector<size_t> array = {5, 45, 10, 90, 5, 5};
	array_test(geom_1, rec, array, 75, tst_path + "stg_array_rec.txt", "rectangle");
}

void staggered_circular_array_test()
{
	Geometry<50,100> geom_1;
	std::vector<size_t> circ = {8,8};
	std::vector<size_t> array = {10, 40, 10, 90, 5, 5};
	array_test(geom_1, circ, array, 60, tst_path + "stg_array_circles.txt", "circle");
}

void wall_test()
{
	Geometry<20,10> geom;
	geom.add_walls(2);
	geom.write(tst_path + "walls_y.txt");
	Geometry<20,10> geomX;
	geomX.add_walls(2, "x");
	geomX.write(tst_path + "walls_x.txt");	
}

void read_test()
{
	staggered_circular_array_test();
	Geometry<50,100> geom;
	geom.read(tst_path + "stg_array_circles.txt");
	geom.write(tst_path + "geom_read_test.txt");
}

// Verify if bound checking is working
// Behavior is correct if the function returns true
bool test_object_bounds(std::string token)
{
	bool passed = false;
	const size_t Nx = 5, Ny = 10;
	Geometry<Nx,Ny> geom;
	if (token == "rectangle"){
		auto pmf = &Geometry<Nx,Ny>::add_rectangle;
		// 1) Check if works with
		// correct bounds
		std::array<size_t, 4> set_1 = {2,2,2,5};
		passed = geom_test_out_of_bounds(geom, pmf, set_1);
		// 2) Check all the out of bonds scenarios (b,yc,a,xc)
		std::vector<std::array<size_t, 4>> sets = {{2,2,2,0}, {2,2,2,10}, 
													{2,2,0,5}, {2,2,5,5}};
		for (auto set_i : sets)
			passed = geom_test_out_of_bounds(geom, pmf, set_i);
	}
	if (token == "ellipse"){
		auto pmf = &Geometry<Nx,Ny>::add_ellipse;
		// 1) Check if works with
		// correct bounds
		std::array<size_t, 4> set_1 = {1,1,2,5};
		passed = geom_test_out_of_bounds(geom, pmf, set_1);
		// 2) Check all the out of bonds scenarios (b,yc,a,xc)
		std::vector<std::array<size_t, 4>> sets = {{2,4,2,0}, {1,1,2,10}, 
													{1,2,0,5}, {1,1,5,5}};
		for (auto set_i : sets)
			passed = geom_test_out_of_bounds(geom, pmf, set_i);
	}
	return passed;
}


