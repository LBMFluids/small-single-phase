#include <string>
#include <vector>
#include "geom_ex.h"
// Functions for basic geometry tests 

// Example programs
void ex_single_square()
{
	Geometry<20,50> geom;
	geom.add_rectangle(4,4,10,20);
	geom.add_walls();
	geom.write("single_square.txt");
}

void ex_single_circle()
{
	Geometry<40,50> geom;
	geom.add_ellipse(10,10,20,20);
	geom.add_walls();
	geom.write("single_circle.txt");
}

void ex_step()
{
	Geometry<60,30> geom;
	geom.add_rectangle(20,10,20,7);
	geom.add_walls(2,"x");
	geom.write("step.txt");
}

void ex_reg_squares()
{
	Geometry<62,100> geom;
	std::vector<size_t> square = {4,4};
	std::vector<size_t> array = {4, 58, 10, 90, 5, 5};
	geom.add_array(square, array, "rectangle");
	geom.write("reg_array_sq.txt");
}

void ex_reg_ellipses()
{
	Geometry<200,100> geom;
	std::vector<size_t> ellipse = {16,8};
	std::vector<size_t> array = {40, 180, 16, 91, 20, 10};
	geom.add_array(ellipse, array, "ellipse");
	geom.add_walls(1,"x");
	geom.write("reg_array_ell.txt");
}

void ex_stag_circ()
{
	Geometry<248,200> geom;
	std::vector<size_t> circle = {16,16};
	std::vector<size_t> array = {16, 232, 40, 180, 20, 5};
	geom.add_array(circle, array, "circle", 60);
	geom.add_walls();
	geom.write("stag_array_circ.txt");
}

void ex_stag_rectancles()
{
	// Geometry object
	Geometry<56,100> geom;
	// First array
	std::vector<size_t> rect_1 = {2,4};
	std::vector<size_t> array_1 = {4, 58, 15, 50, 10, 5};
	geom.add_array(rect_1, array_1, "rectangle", 60);
	// Second array
	std::vector<size_t> rect_2 = {4,2};
	std::vector<size_t> array_2 = {8, 52, 60, 96, 3, 5};
	geom.add_array(rect_2, array_2, "square", 75);
   	// Write to file	
	geom.write("stag_array_rec.txt");
}

void ex_read_write()
{
	Geometry<56,100> geom;
	geom.read("stag_array_rec.txt");
	geom.write("geom_stag_test.txt");
}

// Various geometry testing functions with
// further visualization
void single_rectangle_test()
{
	// Rectangle
	Geometry<20,50> geom;
	// a(y), b(x), xc, yc
	geom.add_rectangle(4,8,10,25);
	geom.write("rectangle.txt");
	// Square
	Geometry<20,50> geom2;
	geom2.add_rectangle(4,4,10,25);
	geom2.write("square.txt");
}

void single_ellipse_test()
{
	// Ellipse
	Geometry<200,500> geom;
	geom.add_ellipse(20,50,100,250);
	geom.write("ellipse.txt");
	// Circle
	Geometry<200,500> geom2;
	geom2.add_ellipse(20,20,100,250);
	geom2.write("circle.txt");
}

void rectangle_array_test()
{
	Geometry<70,30> geom;
	std::vector<size_t> obj = {8, 2};
	std::vector<size_t> array = {15, 63, 5, 20, 3, 3};	
	array_test(geom, obj, array, 0, "rectangle_array.txt", "rectangle");
}

void square_array_test()
{
	Geometry<10,10> geom;
	std::vector<size_t> obj = {2, 2};
	std::vector<size_t> array = {2, 7, 2, 8, 2, 3};	
	array_test(geom, obj, array, 0, "square_array.txt", "square");
}

void ellipse_array_test()
{
	Geometry<70,30> geom;
	std::vector<size_t> obj = {16, 4};
	std::vector<size_t> array = {15, 63, 5, 20, 3, 3};	
	array_test(geom, obj, array, 0, "ellipse_array.txt", "ellipse");
}

void circle_array_test()
{
	Geometry<10,10> geom;
	std::vector<size_t> obj = {2, 2};
	std::vector<size_t> array = {2, 7, 2, 8, 2, 3};	
	array_test(geom, obj, array, 0, "circle_array.txt", "circle");
}

void staggered_square_array_test()
{
	Geometry<50,100> geom_1;
	std::vector<size_t> square = {2,4};
	std::vector<size_t> array = {5, 45, 10, 90, 5, 5};
	array_test(geom_1, square, array, 75, "stg_array_1.txt", "rectangle");
}

void staggered_circular_array_test()
{
	Geometry<50,100> geom_1;
	std::vector<size_t> square = {8,8};
	std::vector<size_t> array = {10, 40, 10, 90, 5, 5};
	array_test(geom_1, square, array, 60, "stg_array_circles.txt", "circle");
}

void wall_test()
{
	Geometry<20,10> geom;
	geom.add_walls(2);
	geom.write("walls_y.txt");
	geom.add_walls(2, "x");
	geom.write("walls_x.txt");	
}

void read_test()
{
	staggered_circular_array_test();
	Geometry<50,100> geom;
	geom.read("stg_array_circles.txt");
	geom.write("geo__read_test.txt");
}

// Check if bound checking is working
bool test_object_bounds(std::string token)
{
	bool passed = false;
	if (token == "rectangle"){
		const size_t Nx = 5, Ny = 10;
		Geometry<Nx,Ny> geom;
		auto pmf = &Geometry<5,10>::add_rectangle;
		// 1) Check if works with
		// correct bounds
		std::array<size_t, 4> set_1 = {2,2,2,5};
		passed = !geom_test_out_of_bounds(geom, pmf, set_1);
		// 2) Check all the out of bonds scenarios (a,b,xc,yc)
		std::vector<std::array<size_t, 4>> sets = {{2,2,2,0}, {2,2,2,9}, 
													{2,2,0,5}, {2,2,4,5}};
		// Unfortunately need to run it by hand or switch from assertions 
		// to throws
		passed = geom_test_out_of_bounds(geom, pmf, sets[3]);
	}
	return passed;
}


