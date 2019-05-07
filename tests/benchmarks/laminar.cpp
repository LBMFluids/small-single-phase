#include <iostream>
#include <ctime>
#include <array>
#include "../../geometry.h"
#include "../../fluid.h"
#include "../../lbm.h"
#include "../common/lbm_tests.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[])
{
	if (argc != 2){
		std::cout << "Usage ./test path_to_results_dir" << std::endl;  
		exit(1);
	}

	// Parameters
	constexpr size_t Nx = 50, Ny = 10, Nd = 9;
	int max_steps = 10000;
	int disp_every = 2500;
	int save_every = 2500;
	double rho_0 = 1.0;

	// Simulation time measurment
	std::time_t sim_start, sim_end;

	// Output file names
	string path(argv[1]);
	if (path.back() != '/')
		path.append(1, '/');
	string file_rho = path + "rho.txt", 
		   file_ux = path + "ux.txt", file_uy = path + "uy.txt";
	string file_rho_temp = path + "rho", 
		   file_ux_temp = path + "ux", file_uy_temp = path + "uy";

	// Simulation objects
	Geometry<Nx,Ny> geom;
	Fluid<Nx,Ny,Nd> fluid("water");
	LBM<Nx,Ny,Nd> lbm(fluid);

	// Force - flow in positive y direction
    // (notice the sign change due to dPdL)
	const double dPdL = 1e-5;
	std::array<double,9> force = {0, 0, -1, 0, 1, -1, -1, 1, 1};
	std::for_each(force.begin(), force.end(), [&dPdL](double &f){ f*=(-dPdL*(1.0/6.0)); });

	// Geometry setup and initialization
	geom.add_walls();
	fluid.simple_ini(geom, rho_0);
	
	// To generate files for computing of the correct solution
	// (needed only once)
	/*geom.write("./comp_data/test_2/geom.txt");
	fluid.write_f("./comp_data/test_2/f_cpp");
	return 0;
	*/
		
	// Simulation
	int step = 1;
	sim_start = std::time(nullptr);
	while (step <= max_steps){
		lbm.f_equilibrium();
		lbm.collide(fluid);
		lbm.add_volume_force(geom, fluid, force);
		lbm.stream(geom, fluid);
		if (!(step%disp_every))
			cout << "---- Iteration " << step << std::endl;
		if (!(step%save_every))
			fluid.save_state(file_rho_temp, file_ux_temp, file_uy_temp, step);
		step++;
	}
	sim_end = std::time(nullptr);
	std::cout << "Simulation time: " << std::difftime(sim_end, sim_start) << std::endl; 

	// Save final data
	fluid.write_rho(file_rho);
	fluid.write_ux(file_ux);
	fluid.write_uy(file_uy);

	// Compare to known solution
	string file_comp("./comp_data/test_2/f_cor");
	if (!compare_f(file_comp, fluid, 1e-5)){
		std::cout << "\033[33;1;101m - Test 2 FAILED \033[0m" << std::endl;
		exit(1);
	}else{
		std::cout << "\033[34;1;106m - Test 2 passed \033[0m" << std::endl;
	}

	return 0;
}
