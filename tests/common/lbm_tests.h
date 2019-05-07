#ifndef LBM_TESTS_H
#define LBM_TESTS_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <array>
#include <string>

#include "../../lbm.h"

// Templated functions for LBM class testing

// Comparison functions

// Compare reference and computed equilibrium distributions 
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
bool compare_feq(const std::string fname, const LBM<Lx,Ly,Nd> &lbm, double eps)
{
	// Usefeul typedefs
	typedef std::array<std::array<std::array<double, Nd>, Lx+1>, Ly+1> grid_full;
	typedef std::array<std::array<std::array<double, Lx+1>, Ly+1>, Nd> rev_grid_full;

	grid_full feq = lbm.get_feq(); 
	// Reverse grid_full
	rev_grid_full rev_feq;
	for (size_t i=0; i<Ly+1; i++)
		for (size_t j=0; j<Lx+1; j++)
			for (size_t k=0; k<Nd; k++)
				rev_feq[k][i][j] = feq[i][j][k];

	// Read files and compare
	for (size_t k=0; k<Nd; k++){
		std::ostringstream name;
		name << fname << "_" << k << ".txt";
		std::ifstream input(name.str());
		if (!input)
			throw std::runtime_error("Opening the file " + fname +  " failed ");

		double correct = 0.0;
		for (size_t i=0; i<Ly+1; i++){
			for (size_t j=0; j<Lx+1; j++){
				input >> correct;
				if (std::abs((correct - rev_feq[k][i][j])/correct) > eps){
					std::cout << "Correct: " << correct 
							  << "\nComputed: " << rev_feq[k][i][j] 
							  << "\nOn Nd, Ny, Nx: " << k << " "  
							  << i << " " << j<< std::endl;
					return false;
				}
			}
		}
	}
	return true;
}

// Compare reference and computed density distributions 
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
bool compare_f(const std::string fname, const Fluid<Lx,Ly,Nd> &fluid, double eps)
{
	// Usefeul typedefs
	typedef std::array<std::array<std::array<double, Nd>, Lx+1>, Ly+1> grid_full;
	typedef std::array<std::array<std::array<double, Lx+1>, Ly+1>, Nd> rev_grid_full;

	grid_full f = fluid.get_f(); 
	// Reverse grid_full
	rev_grid_full rev_f;
	for (size_t i=0; i<Ly+1; i++)
		for (size_t j=0; j<Lx+1; j++)
			for (size_t k=0; k<Nd; k++)
				rev_f[k][i][j] = f[i][j][k];

	// Read files and compare
	for (size_t k=0; k<Nd; k++){
		std::ostringstream name;
		name << fname << "_" << k << ".txt";
		std::ifstream input(name.str());
		if (!input)
			throw std::runtime_error("Opening the file " + fname +  " failed ");

		double correct = 0.0;
		for (size_t i=0; i<Ly+1; i++){
			for (size_t j=0; j<Lx+1; j++){
				input >> correct;
				if (std::abs((correct - rev_f[k][i][j])/correct) > eps){
					std::cout << "Correct: " << correct 
							  << "\nComputed: " << rev_f[k][i][j] 
							  << "\nOn Nd, Ny, Nx: " << k << " "  
							  << i << " " << j<< std::endl;
					return false;
				}
			}
		}
	}
	return true;
}



#endif
