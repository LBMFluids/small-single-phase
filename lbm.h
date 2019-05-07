#ifndef LBM_H
#define LBM_H

#include <algorithm>
#include <array>

#include "fluid.h"

template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
class Fluid;

// LBM simulation-related operations 
template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
class LBM{
	typedef std::array<std::array<double, Lx+1>, Ly+1> grid_2D;
	typedef std::array<std::array<std::array<double, Nd>, Lx+1>, Ly+1> grid_full;
public:
	// Constructors
	LBM() = delete;
	LBM(Fluid<Lx,Ly,Nd>& fl): fluid_1(fl), fluid_2(*(new Fluid<Lx,Ly,Nd>)), single_component(true)
		{ fl.prop_ini(feq); }

	// Operations
	void f_equilibrium();
	void collide(Fluid<Lx,Ly,Nd> &fluid) { collide(fluid.f, fluid.omega); }
	void add_volume_force(const Geometry<Lx,Ly>& geom, 
					Fluid<Lx,Ly,Nd> &fluid, const std::array<double, Nd> &force) const
	{ add_volume_force(geom, fluid.f, force); }
	void stream(const Geometry<Lx,Ly>& geom, Fluid<Lx,Ly,Nd> &fluid) const
		{ stream(geom, fluid.f); }
	
	// Getters
	grid_full get_feq() const { return feq; }

	// Destructors
	~LBM() = default; 
private:
	bool single_component;
	double f1 = 3.0, f2 = 4.5, f3 = 1.5;
   
  	const std::vector<double> Cx = {0.0, 1.0, 0.0, -1.0, 0.0, 1.0, -1.0, -1.0, 1.0};
	const std::vector<double> Cy = {0.0, 0.0, 1.0, 0.0, -1.0, 1.0, 1.0, -1.0, -1.0};
	const std::vector<size_t> bb_rules = {3, 4, 1, 2, 7, 8, 5, 6};

	Fluid<Lx,Ly,Nd> &fluid_1, &fluid_2;
	grid_full feq;

	void collide(grid_full&, double&);
	void add_volume_force(const Geometry<Lx,Ly>&, grid_full&, const std::array<double, Nd>&) const;
	void stream(const Geometry<Lx,Ly>&, grid_full&) const;
};

template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void LBM<Lx,Ly,Nd>::f_equilibrium()
{	
	double rt0, rt1, rt2;
	double ueqxij, ueqyij, uxsq, uysq, uxuy5,
		   uxuy6, uxuy7, uxuy8, usq;
	grid_2D rho, ux, uy;

	if (single_component){
		fluid_1.compute_macroscopic();
		std::copy(fluid_1.rho.begin(), fluid_1.rho.end(), rho.begin());
  		std::copy(fluid_1.ux.begin(), fluid_1.ux.end(), ux.begin());
		std::copy(fluid_1.uy.begin(), fluid_1.uy.end(), uy.begin());
	}

	for (size_t i=0; i<Ly+1; i++){
        for (size_t j=0; j<Lx+1; j++){		
			rt0 = 4.0/9.0*rho[i][j];
			rt1 = 1.0/9.0*rho[i][j];
	 		rt2 = 1.0/36.0*rho[i][j];

			ueqxij =  ux[i][j];
      		ueqyij =  uy[i][j];
	        uxsq   =  ueqxij * ueqxij;
			uysq   =  ueqyij * ueqyij;
			uxuy5  =  ueqxij +  ueqyij;
			uxuy6  = -ueqxij +  ueqyij;
		 	uxuy7  = -ueqxij + -ueqyij;
		 	uxuy8  =  ueqxij + -ueqyij;
		
			usq    =  uxsq + uysq;

			feq[i][j][0] = rt0*(1.0 - f3*usq);
			feq[i][j][1] = rt1*(1.0 + f1*ueqxij + f2*uxsq - f3*usq);
			feq[i][j][2] = rt1*(1.0 + f1*ueqyij + f2*uysq - f3*usq);
			feq[i][j][3] = rt1*(1.0 - f1*ueqxij + f2*uxsq - f3*usq);
			feq[i][j][4] = rt1*(1.0 - f1*ueqyij + f2*uysq - f3*usq);
			feq[i][j][5] = rt2*(1.0 + f1*uxuy5 + f2*uxuy5*uxuy5 - f3*usq);
			feq[i][j][6] = rt2*(1.0 + f1*uxuy6 + f2*uxuy6*uxuy6 - f3*usq);
			feq[i][j][7] = rt2*(1.0 + f1*uxuy7 + f2*uxuy7*uxuy7 - f3*usq);
			feq[i][j][8] = rt2*(1.0 + f1*uxuy8 + f2*uxuy8*uxuy8 - f3*usq);
		}
	}
}

template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void LBM<Lx,Ly,Nd>::collide(grid_full &f, double &omega)
{
	for (size_t i=0; i<Ly+1; i++){
		for (size_t j=0; j<Lx+1; j++){
			for (size_t k=0; k<Nd; k++){						
				f[i][j][k] = (1.0 - omega)*f[i][j][k] + omega*feq[i][j][k];							
			}
		}
	}
}

template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void LBM<Lx,Ly,Nd>::add_volume_force(const Geometry<Lx,Ly>& geom, grid_full &f, 
				const std::array<double, Nd> &force) const
{
	for (size_t i=0; i<Ly+1; i++){
		for (size_t j=0; j<Lx+1; j++){
			for (size_t k=0; k<Nd; k++){
				if (geom(i,j))					
					f[i][j][k] += force[k];							
			}
		}
	}
}

template<std::size_t Lx, std::size_t Ly, std::size_t Nd>
void LBM<Lx,Ly,Nd>::stream(const Geometry<Lx,Ly>& geom, grid_full &f) const
{
	grid_full temp;
	size_t ist = 0, jst = 0;

	for (size_t i=0; i<Ly+1; i++){
		for (size_t j=0; j<Lx+1; j++){
			// Skip solid nodes
			if (!geom(i,j))
				continue;	
			for (size_t k=1; k<Nd; k++){
				// Periodic boundaries
				if (Cy[k]>0)
					ist = (i+Cy[k]<Ly+1)?(i+Cy[k]):0;
				else
					ist = (i+Cy[k]>=0)?(i+Cy[k]):Ly;
				if (Cx[k]>0)
					jst = (j+Cx[k]<Lx+1)?(j+Cx[k]):0;
				else
					jst = (j+Cx[k]>=0)?(j+Cx[k]):Lx;
				// Streaming with bounce-back
				if (geom(ist,jst))					
					temp[ist][jst][k] = f[i][j][k];
				else
					temp[i][j][bb_rules[k-1]] = f[i][j][k];
			}
		}
	}
	// Copy temp to f
	for (size_t i=0; i<Ly+1; i++){
		for (size_t j=0; j<Lx+1; j++){
			// Skip solid nodes
			if (!geom(i,j))
				continue;
			for (size_t k=1; k<Nd; k++){
				f[i][j][k] = temp[i][j][k];							
			}
		}
	}
}

#endif




