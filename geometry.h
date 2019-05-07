#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <array>
#include <string>
#include <vector>

// Model geometry
// x is the horizontal and y the vertical direction
template<std::size_t Lx, std::size_t Ly> 
class Geometry{
public:
	// Constructor - creates Lx x Ly fluid domain
	Geometry();

	// Output geometry to a file
	void write(std::string);
	// Read geometry from a file
	void read(std::string);

	// Add side walls 	
	void add_walls(size_t dH=1, std::string where ="y");

	// Objects
	void add_rectangle(size_t, size_t, size_t, size_t);
	void add_ellipse(size_t, size_t, size_t, size_t);
	// Arrays of objects 
	void add_array(std::vector<size_t>, std::vector<size_t>, std::string, size_t alpha=0);

	// Other
	const bool operator()(size_t i, size_t j) const { return geom[i][j]; } 

private:
	// Geometric domain
	std::array<std::array<bool, Lx+1>, Ly+1> geom;
	// Pi
	const double pi = std::acos(-1);
};

template<std::size_t Lx, std::size_t Ly> 
Geometry<Lx, Ly>::Geometry()
{
	auto row = geom.begin();
	while (row != geom.end()){
		(*row).fill(1);
		row++;
	}
}

template<std::size_t Lx, std::size_t Ly>
void Geometry<Lx, Ly>::write(std::string filename)
{
	std::ofstream output(filename);
	for (auto row : geom){
		std::copy(row.begin(), row.end(), 
			std::ostream_iterator<bool>(output, " "));
		output << '\n';
	}
	// Check if write successful
	if (output.rdstate())
		throw std::runtime_error("Writing to the file " + filename +  " failed ");
}

template<std::size_t Lx, std::size_t Ly>
void Geometry<Lx, Ly>::read(std::string filename)
{
	std::ifstream in(filename);
	if (!in)
		throw std::runtime_error("Opening the file " + filename +  " failed ");
	std::string text;
	size_t yi = 0;
    while (std::getline(in, text)){
		std::string column;
		std::istringstream  row(text);
		size_t xj = 0;
		while (row >> column)
			geom[yi][xj++] = std::stoi(column);
		yi++;
	}
	// Check if read successful
	if (in.bad())
		throw std::runtime_error("Reading the file " + filename +  " failed ");	
}

template<std::size_t Lx, std::size_t Ly>
void Geometry<Lx, Ly>::add_walls(size_t dH, std::string where)
{
	if (where == "y"){
		if (!((dH>0) && (dH<=Lx))) 
			throw std::runtime_error("Wall thickness smaller than 0 or larger than domain width (x)");
		for (size_t i=0; i<geom.size(); i++)
			for (size_t j=0; j<dH; j++){
				geom[i][j] = 0;
				geom[i][(geom[i].size()-1)-j] = 0;
			}
	}else if (where == "x"){
		if (!((dH>0) && (dH<=Ly)))
			throw std::runtime_error("Wall thickness smaller than 0 or larger than domain height (y)");
		size_t bottom = geom.size()-1;
		for (size_t i=0; i<dH; i++){
			std::fill(geom[i].begin(), geom[i].end(), 0);
			bottom -= i;
			std::fill(geom[bottom].begin(), geom[bottom].end(), 0);
		}
	}else{
		throw std::invalid_argument( "No options for position argument: " + where);
	}
}

template<std::size_t Lx, std::size_t Ly>
void Geometry<Lx, Ly>::add_rectangle(size_t b, size_t a, size_t xc, size_t yc)
{
	if (a%2 || b%2)
		std::cout << "Warning!!! Sides not divisible by 2" 
				  << " - applying default float -> int truncation" << std::endl;
	// Upper and lower bounds of the object
	auto row = geom.begin() + yc - a/2;
	auto af = geom.begin() + yc + a/2 + 1;

	if (!(row >= geom.begin() && row < geom.end()))
		throw std::runtime_error("Wrong system dimensions!");
	if (!(af >= geom.begin() && af <= geom.end()))
		throw std::runtime_error("Wrong system dimensions!");

	// Fill in the columns
	while (row != af){
		auto b0 = (*row).begin() + xc - b/2;
		auto bf = (*row).begin() + xc + b/2 + 1;
		if (!(b0 >= (*row).begin() && b0 <= (*row).end()))
			throw std::runtime_error("Wrong system dimensions!");
		if (!(bf >= (*row).begin() && bf <= (*row).end()))
			throw std::runtime_error("Wrong system dimensions!");
		std::fill(b0, bf, 0);
		row++;
	}
}

template<std::size_t Lx, std::size_t Ly>
void Geometry<Lx, Ly>::add_ellipse(size_t a, size_t b, size_t xc, size_t yc)
{
	// Use indices rather than iterators since it's point-wise filling
	// size_t is unsigned, it will never be 0 - convert to int
	if (!((int)(yc-b) >= 0 && (yc+b) < Ly+1))
		throw std::runtime_error("Wrong system dimensions!");
	if (!((int)(xc-a) >= 0 && (xc+a) < Lx+1))
		throw std::runtime_error("Wrong system dimensions!");
	for (size_t jy = yc - b; jy <= yc + b; jy++)
		for (size_t ix = xc - a ; ix <= xc + a; ix++)	
			if (((ix-xc)*(ix-xc)*b*b + (jy-yc)*(jy-yc)*a*a) <=a*a*b*b)
				geom[jy][ix] = 0;
}

template<std::size_t Lx, std::size_t Ly>
void Geometry<Lx, Ly>::add_array(std::vector<size_t> obj_params, std::vector<size_t> arr_params, std::string object, size_t alpha)
{
	size_t b = obj_params[0], a = obj_params[1];
 	size_t x0 = arr_params[0], xf = arr_params[1], y0 = arr_params[2], yf = arr_params[3], 
		   dx = arr_params[4], dy = arr_params[5];

	// Compute the number of objects in each row and column
	size_t Nrow = (yf-y0-a-dy)/(a+dy) + 2;
	size_t Ncol = (xf-x0-b-dx)/(b+dx) + 2;

	// Simplified treatment for alpha = 0 i.e. no staggering
	if (!alpha){
		size_t yi = y0;
		for (size_t i=0; i<Nrow; i++){
			size_t xj = x0;
			for (size_t j=0; j<Ncol; j++){
				if (object == "rectangle" || object == "square")
					this->add_rectangle(b, a, xj, yi);
				else if (object == "ellipse" || object == "circle")
					this->add_ellipse(b/2, a/2, xj, yi);
				else
					throw std::invalid_argument( "No options for object type argument: " + object);
				xj += (b+dx);
			}
			yi += (a+dy);
		}
	}else{
		if(alpha >= 90)
			throw std::invalid_argument( "Invalid staggering angle range. Valid range is 0-90 degrees.");
		// Compute corrected x and y distances for even rows
		double alpha_rad = (double)alpha*pi/180.0;
		double beta_rad = (double)(180-2*alpha)*pi/180.0;
		double dx_alpha = std::sin(alpha_rad)/std::sin(beta_rad)*(double)(dx + b);
		size_t dxs = (dx+b)/2;
		size_t dys = (size_t)(std::sqrt(dx_alpha*dx_alpha - (double)(dxs*dxs)));
		size_t xs0 = x0 + dxs, xsf = xf - dxs;
		size_t Nrow = (yf-y0-a-(dys-a))/(a+(dys-a)) + 2;

		// Compute number of objects in even (staggered) rows
		size_t Nstg = (xsf-xs0-b-dx)/(b+dx) + 2;
		// Create the array
		size_t yi = y0;
		for (size_t i=0; i<Nrow; i++){
			size_t xj = i%2?xs0:x0;
			for (size_t j=0; j<(i%2?Nstg:Ncol); j++){
				if (object == "rectangle" || object == "square")
					this->add_rectangle(b, a, xj, yi);
				else if (object == "ellipse" || object == "circle")
					this->add_ellipse(b/2, a/2, xj, yi);
				else
					throw std::invalid_argument( "No options for object type argument: " + object);
				xj += (b+dx);
			}
			yi += dys;
		}

		// Actual alpha value - information for the user
		dx_alpha = std::sqrt((double)(dxs*dxs) + (double)(dys*dys));
		double new_alpha = std::asin(dx_alpha/(double)(dx+b)*std::sin(beta_rad));
		std::cout << "Requested staggering angle: " << alpha << "\n"
				  << "Actual staggering angle: " << 180.0*new_alpha/pi << std::endl;
		std::cout << "y distance between object centers: " << dys << std::endl;
	}	
}
#endif

