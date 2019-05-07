#include <iostream>
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

	// Additional parameters
	int max_steps = 1000;
	int disp_every = 200;
	double rho_0 = 1.0;

	// Output file names
	string path(argv[1]);
	if (path.back() != '/')
		path.append(1, '/');
	string file_rho = path + "rho.txt", 
		   file_ux = path + "ux.txt", file_uy = path + "uy.txt";

	// Simulation objects
	Geometry<5,10> geom;
	Fluid<5,10,9> fluid("water");
	LBM<5,10,9> lbm(fluid);

	// Simulation
	fluid.simple_ini(geom, rho_0);

	// To generate files for computing of the correct solution
	// (needed only once)
	/*geom.write("./comp_data/test_1/geom.txt");
	fluid.write_f("./comp_data/test_1/f_cpp");
	return 0;
	*/

	int step = 1;
	while (step <= max_steps){
		lbm.f_equilibrium();
		lbm.collide(fluid);
		lbm.stream(geom, fluid);
		if (!(step%disp_every))
			cout << "---- Iteration " << step << std::endl;
		step++;
	}

	// Save final data
	fluid.write_rho(file_rho);
	fluid.write_ux(file_ux);
	fluid.write_uy(file_uy);

	// Compare to known solution
	string file_comp("./comp_data/test_1/f_cor");
	if (!compare_f(file_comp, fluid, 1e-5)){
		std::cout << "\033[33;1;101m - Test 1 FAILED \033[0m" << std::endl;
		exit(1);
	}else{
		std::cout << "\033[34;1;106m - Test 1 passed \033[0m" << std::endl;
	}

	return 0;
}
