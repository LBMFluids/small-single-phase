#include <iostream>
#include <ctime>
#include <array>
#include "../../geometry.h"
#include "../../fluid.h"
#include "../../lbm.h"

using std::cout;
using std::endl;
using std::string;

int main()
{
	// Parameters
	constexpr size_t Nx = 199, Ny = 95, Nd = 9;
	int max_steps = 1500;
	double rho_0 = 1.0;

	// Simulation time measurment
	std::time_t sim_start, sim_end;

	// Output file names
	string file_rho("./arrays/rectangular/rho.txt"), 
		   file_ux("./arrays/rectangular/ux.txt"), file_uy("./arrays/rectangular/uy.txt");

	// Simulation objects
	Geometry<Nx,Ny> geom;
	Fluid<Nx,Ny,Nd> fluid("water");
	LBM<Nx,Ny,Nd> lbm(fluid);

	// Force - flow in positive y direction
	const double dPdL = 1e-6;
	std::array<double,9> force = {0, 0, -1, 0, 1, -1, -1, 1, 1};
	std::for_each(force.begin(), force.end(), [&dPdL](double &f){ f*=(-dPdL*(1.0/6.0)); });

	// Geometry setup and initialization
	geom.add_walls();
	std::vector<size_t> square = {6,6};
	std::vector<size_t> array = {3, Nx-2, 8, Ny-5, 10, 10};
	geom.add_array(square, array, "square");
	geom.write("./arrays/rectangular/geom_squares.txt");
	fluid.simple_ini(geom, rho_0);
	fluid.write_f("./arrays/rectangular/f_ini_rarr.txt");

	// Simulation
	int step = 1;
	sim_start = std::time(nullptr);
	while (step <= max_steps){
		lbm.f_equilibrium();
		lbm.collide(fluid);
		lbm.add_volume_force(geom, fluid, force);
		lbm.stream(geom, fluid);
		step++;
	}
	sim_end = std::time(nullptr);
	std::cout << "Simulation time: " << std::difftime(sim_end, sim_start) << std::endl; 

	// Save final data
	fluid.write_rho(file_rho);
	fluid.write_ux(file_ux);
	fluid.write_uy(file_uy);
}
