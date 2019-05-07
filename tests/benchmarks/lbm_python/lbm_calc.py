#!/usr/bin/python2.7

# Module with various computations - analytical solutions, physical quantities 

import numpy as np

def laminar_flow(ncol, nrow, dPdL):
	''' Analytical solution	for laminar flow 
			between two parallel plates in y direction'''
	channel_hw = (ncol-2.0)/2.0
	uy_max = dPdL*(channel_hw*channel_hw)/(2.0*1.0/6.0)
	uy_av = uy_max*(2.0/3.0)
	x = np.arange(-channel_hw, channel_hw)
	x += 0.5
	uy = uy_max*(1.0-(x/channel_hw)*(x/channel_hw))
	uy = np.insert(uy, 0, 0)
	uy = np.insert(uy, uy.size, 0)
	x = np.insert(x, 0, x[0]-1)
	x = np.insert(x, x.size, x[-1]+1)
	return x, uy

# Compute velocity magnitude for the entire domain 
def vel_mag(ux, uy):
	''' Compute velocity magnitude for the entire domain '''
	return np.transpose(np.sqrt(ux*ux + uy*uy))

# Mean value of array x with 0s excluded 
def ave(x): 
	''' Return mean of array x with 0s excluded '''
	return np.mean(x.ravel()[np.flatnonzero(x)])

# Compute the Reynolds number across a row/column
def Re(pos, x=None, u=None):
	''' Compute 2D parallel plate Re '''
	# pos - row/column number for intgration
	# x - direction of integration (if x is None then it's y)
	# u - velocity perpendicular to integration direction
	if x:
		return 2.0*6.0*np.trapz(u[pos,:])
	else:
		return 2.0*6.0*np.trapz(u[:,pos])



