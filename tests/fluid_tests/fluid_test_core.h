#ifndef FLUID_TEST_CORE_H
#define FLUID_TEST_CORE_H

#include <iostream>
#include <cmath>
#include <tuple>
#include <string>
#include <vector>
#include <array>
#include "../../geometry.h"
#include "../../fluid.h"

// Aliases
using packed = const std::tuple<std::string, double, double, double, double>;
template<std::size_t Lx, std::size_t Ly>
using grid_2D = std::array<std::array<double, Lx+1>, Ly+1>;
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
using grid_full = std::array<std::array<std::array<double, Nd>, Lx+1>, Ly+1>;

// Driver
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void fluid_run_all(const Geometry<Lx,Ly> geom);

// Utility functions
// Equality comparison with doubles
auto equal_doubles = [](double v1, double v2, double eps=1e-5){ return !(std::abs((v1 - v2)/v1) > eps); };
// Print test outcome
void tst_pass(bool val, const std::string msg);

// Individual tests
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
bool constructor_check();
// f values check
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
bool check_f_val(grid_full<Lx,Ly,Nd>, double);
// Property check
bool test_props(packed, const std::string, const std::vector<double>);
// Check if macroscopic property equal to expected
template<std::size_t Lx, std::size_t Ly>
bool check_macro_val(Geometry<Lx,Ly>,grid_2D<Lx,Ly>, double);
// Initial rho and its getter
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
bool rho_check(const Geometry<Lx,Ly>, double);
// Velocity computation and getter
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
bool vel_check(const Geometry<Lx,Ly>, double, double);

// Entire test suite
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void fluid_run_all(const Geometry<Lx,Ly> geom)
{
	// 1. Checks variable setting and related f initialization
	tst_pass(constructor_check<Lx,Ly,Nd>(), "Constructor");
	// 2. Checks proper f initialization from rho_0 and rho computation
	double rho_0 = 5.0;
	tst_pass(rho_check<Lx,Ly,Nd>(geom, rho_0), "Density initialization and getter");
    // 3. Checks computation and getter of ux/uy
	double ux = 0.0, uy = 0.0;
	tst_pass(vel_check<Lx,Ly,Nd>(geom, ux, uy), "Velocity initialization and getter");	
}

// Contructor/initialization test
// - Saves generated properties
// - Verifies f is properly initialized
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
bool constructor_check()
{
	// Default constructor
	Fluid<Lx, Ly, Nd> fld_1;
	std::vector<double> exp_1 = {1./3., 1.0, 1./6., 1.0};
	if (!test_props(fld_1.get_props(), "fluid", exp_1))
		return false;
	if (!check_f_val<Lx,Ly,Nd>(fld_1.get_f(), 0.0))
		return false;
	// Constructor with a custom fluid name
	Fluid<Lx, Ly, Nd> fld_2("coffee");
	std::vector<double> exp_2 = exp_1;
	if (!test_props(fld_2.get_props(), "coffee", exp_2))
		return false;
	if (!check_f_val<Lx,Ly,Nd>(fld_2.get_f(), 0.0))
		return false;
	// Custom properties 
	Fluid<Lx, Ly, Nd> fld_3("jogurt", 1./5, 2.0);
	std::vector<double> exp_3 = {1./5., 2.0, 1./2., 0.5};
	if (!test_props(fld_3.get_props(), "jogurt", exp_3))
		return false;
	if (!check_f_val<Lx,Ly,Nd>(fld_3.get_f(), 0.0))
		return false;
	//
	return true;
}

// True if fluid properties equal expected values and name 
bool test_props(packed props, const std::string name, const std::vector<double> exp)
{
	if (std::get<0>(props)!=name)
		return false;
	if (!equal_doubles(std::get<1>(props), exp.at(0)))
		return false;
	if (!equal_doubles(std::get<2>(props), exp.at(1)))
		return false;
	if (!equal_doubles(std::get<3>(props), exp.at(2)))
		return false;
	if (!equal_doubles(std::get<4>(props), exp.at(3)))
		return false;
	return true;
}

// True if all entries in f are equal to val
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
bool check_f_val(grid_full<Lx,Ly,Nd> f, double val)
{
	for (size_t i=0; i<Ly+1; i++)
		for (size_t j=0; j<Lx+1; j++)
			for (size_t k=0; k<Nd; k++)
				if (!equal_doubles(f[i][j][k], val))
					return false;
	return true;
}

// True if all entries in macroscopic variable var 
// are equal to val
template<std::size_t Lx, std::size_t Ly>
bool check_macro_val(Geometry<Lx,Ly> geom, grid_2D<Lx,Ly> var, double val)
{
	double exp = 0.0;
	for (size_t i=0; i<Ly+1; i++)
		for (size_t j=0; j<Lx+1; j++){
			exp = (geom(i,j) == 0) ? 0.0 : val;
			if (!equal_doubles(var[i][j], exp))
				return false;
		}
	return true;
}

// Initial setting of f from density and test of the density getter
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
bool rho_check(const Geometry<Lx,Ly> geom, double rho_0)
{
	Fluid<Lx, Ly, Nd> fluid;
	fluid.simple_ini(geom, rho_0);
	if (!check_macro_val<Lx,Ly>(geom, fluid.get_rho(), rho_0))
		return false;
	return true;
}

// Initial velocity computtion and velocity getter
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
bool vel_check(const Geometry<Lx,Ly> geom, double ux, double uy)
{
	Fluid<Lx, Ly, Nd> fluid;
	fluid.simple_ini(geom, 1.5);
	if (!check_macro_val<Lx,Ly>(geom, fluid.get_ux(), ux))
		return false;
	if (!check_macro_val<Lx,Ly>(geom, fluid.get_uy(), uy))
		return false;
	return true;
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
#endif
