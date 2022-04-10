#ifndef FLUID_H
#define FLUID_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <tuple>
#include <array>
#include <string>

#include "geometry.h"
#include "lbm.h"

// Fluid properties, density
// distribution and derived variables
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
class Fluid{
	typedef std::array<std::array<double, Lx+1>, Ly+1> grid_2D;
   	typedef std::array<std::array<std::array<double, Nd>, Lx+1>, Ly+1> grid_full;
   	typedef std::array<std::array<std::array<double, Lx+1>, Ly+1>, Nd> rev_grid_full;
	template<std::size_t Lxf, std::size_t Lyf, std::size_t Ndf>
	friend class LBM;	
public:
	// Constructors
	Fluid(const std::string &s, double speed, double rel) :
			name(s), cs(speed), tau(rel)
		{ omega = 1/tau; nu = 1./3*(tau - 0.5); prop_ini(f); }
	Fluid(const std::string &s) : Fluid(s, 1./3, 1.0){ prop_ini(f); }
	Fluid() : Fluid("fluid", 1./3, 1.0){ prop_ini(f); }

	// Initialization
	void simple_ini(const Geometry<Lx,Ly>&, double);

	// Getters
	std::tuple<std::string, double, double, double, double>get_props() { return std::make_tuple(name, cs, tau, nu, omega); }
	grid_2D get_rho() { compute_rho(); return rho; }
	grid_2D get_ux() { compute_ux(); return ux; }
	grid_2D get_uy() { compute_uy(); return uy; }
	grid_full get_f() const { return f; }

	// I/O
	void print_properties() const;
	void write_rho(const std::string fname){ get_rho(); write_var(rho, fname); }
	void write_ux(const std::string fname){ get_ux(); write_var(ux, fname); }
	void write_uy(const std::string fname){ get_uy(); write_var(uy, fname); }
	void write_f(const std::string fname) const { write_var(f, fname); }
	void save_state(const std::string frho, const std::string fux, 
						const std::string fuy, const int step);
private:
	std::string name;
	double cs, tau, nu, omega;
	grid_full f;
	grid_2D rho, ux, uy;

	const std::vector<double> Cx = {0.0, 1.0, 0.0, -1.0, 0.0, 1.0, -1.0, -1.0, 1.0};
    const std::vector<double> Cy = {0.0, 0.0, 1.0, 0.0, -1.0, 1.0, 1.0, -1.0, -1.0};	

	void prop_ini(grid_2D&);
	void prop_ini(grid_full&);

	void write_var(grid_2D&, const std::string);
	void write_var(const grid_full&, const std::string) const;

	void compute_rho();
	void compute_vel(grid_2D&, const std::vector<double>&);
	void compute_ux(){ compute_vel(ux, Cx); } 
	void compute_uy(){ compute_vel(uy, Cy); }
	void compute_macroscopic();
};

// Initializations
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void Fluid<Lx, Ly, Nd>::prop_ini(grid_2D& prop)
{
	for (size_t i=0; i<Ly+1; i++)
		for (size_t j=0; j<Lx+1; j++)
			prop[i][j] = 0.0;
}

template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void Fluid<Lx, Ly, Nd>::prop_ini(grid_full& f)
{
	for (size_t i=0; i<Ly+1; i++)
		for (size_t j=0; j<Lx+1; j++)
			for (size_t k=0; k<Nd; k++)
				f[i][j][k] = 0.0;
}

template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void Fluid<Lx, Ly, Nd>::simple_ini(const Geometry<Lx,Ly>& geom, double rho_0)
{
	for (size_t i=0; i<Ly+1; i++)
		for (size_t j=0; j<Lx+1; j++)
			for (size_t k=0; k<Nd; k++)
				if (geom(i,j))
					f[i][j][k] = rho_0/9.0;
}

// Getters
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void Fluid<Lx, Ly, Nd>::compute_rho()
{
	prop_ini(rho);
	for (size_t i=0; i<Ly+1; i++)
		for (size_t j=0; j<Lx+1; j++)
			for (size_t k=0; k<Nd; k++)
				rho[i][j] += f[i][j][k];	
}

template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void Fluid<Lx, Ly, Nd>::compute_vel(grid_2D& vel, const std::vector<double>& Cij)
{
	compute_rho();
	prop_ini(vel);
	for (size_t i=0; i<Ly+1; i++){
		for (size_t j=0; j<Lx+1; j++){
			for (size_t k=0; k<Nd; k++)
				vel[i][j] += f[i][j][k]*Cij[k];
			vel[i][j] /= rho[i][j];
		}
	}
}

template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void Fluid<Lx, Ly, Nd>::compute_macroscopic()
{
	prop_ini(rho);
	prop_ini(ux);
	prop_ini(uy);

	for (size_t i=0; i<Ly+1; i++){
		for (size_t j=0; j<Lx+1; j++){
			for (size_t k=0; k<Nd; k++){
				ux[i][j] += f[i][j][k]*Cx[k];
				uy[i][j] += f[i][j][k]*Cy[k];
				rho[i][j] += f[i][j][k];
			}
			ux[i][j] /= rho[i][j];
			ux[i][j] /= rho[i][j];
		}
	}
}

// I/O
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void Fluid<Lx, Ly, Nd>::print_properties() const 
{ 
	std::cout << "Fluid: " << name << "\n" 
		<< "Speed of sound: " << cs << ", relaxation time: " << tau
    	<< "\nViscosity: " << nu << ", omega: "	<< omega
		<< "\nNumber of nodes: " << (Lx+1) << " x " << (Ly+1) << " x " << Nd << std::endl; 
}

template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void Fluid<Lx, Ly, Nd>::write_var(grid_2D& var, const std::string filename)
{
	std::ofstream output(filename);
	for (auto row : var){ 
		std::copy(row.begin(), row.end(), 
			std::ostream_iterator<double>(output, " "));
		output << '\n';
	}
	if (output.rdstate())
		throw std::runtime_error("Writing to the file " + filename +  " failed ");
}

template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void Fluid<Lx, Ly, Nd>::write_var(const grid_full& f, const std::string fname) const
{
	// Reverse grid_full
	rev_grid_full rev_f;
	for (size_t i=0; i<Ly+1; i++)
		for (size_t j=0; j<Lx+1; j++)
			for (size_t k=0; k<Nd; k++)
				rev_f[k][i][j] = f[i][j][k];
	// Write rev_f to files, one file per direction	
	for (size_t k=0; k<Nd; k++){
		std::ostringstream name;
		name << fname << "_" << k << ".txt";	
		std::ofstream output(name.str());
		for (size_t i=0; i<Ly+1; i++){ 
			std::copy(rev_f[k][i].begin(), rev_f[k][i].end(),
							std::ostream_iterator<double>(output, " "));
			output << std::endl;
		}
		if (output.rdstate())
			throw std::runtime_error("Writing to the file " + fname +  " failed ");
	}
}

template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void Fluid<Lx, Ly, Nd>::save_state(const std::string frho, const std::string fux, 
	const std::string fuy, const int step)
{
	// Lambda to generate file names indicating the step
	auto gen_name = [](std::string fname, int k)
		{ 	std::ostringstream name; 	
			name << fname << "_" << k << ".txt"; 
			return name.str(); };
	// Compute and save each variable
	write_var(rho, gen_name(frho, step));	
	write_var(ux, gen_name(fux, step));
	write_var(uy, gen_name(fuy, step));
}

#endif
