#!/usr/bin/python2.7

import sys
from functools import partial
# Path to custom python modules
src_path = './lbm_python/'
sys.path.insert(0, src_path)

import matplotlib.pylab as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import numpy as np

import lbm_vis as vis
import lbm_utils as util
import lbm_calc as clc

# # # # # # # # # # # # # # # # # # # # #
# 1) Compute and plot velocity mganitude  
#	development
# - - - Input - - -
# Data
#fname_ux = './arrays/rectangular/ux_'
#fname_uy = './arrays/rectangular/uy_'
#sequence = np.arange(10, 1.5e3+1, 10, np.int32)
## - - - - - - - - -
## Velocity magnitude
#ops = [(fname_ux, fname_uy, clc.vel_mag, None)]
#u_mag = util.compute_all_2args('.txt', sequence, ops) 
## Average velocity magnitude
#u_ave = util.apply_all(clc.ave, u_mag)
#u_max = util.apply_all(np.max, u_mag)
# Add in the docs as an example
#ops2 = [(fname_uy, clc.ave), (None, lambda x: 2*x), (None, lambda x: x*x)]
#u = util.compute_all('.txt', sequence, ops2)
#print u
# Another example
#ops3 = [(fname_ux, fname_uy, np.absolute, np.absolute), (None, fname_uy, np.max, np.max), (None, fname_uy, lambda x,y: np.max(np.maximum(x,y)), None)]
#u_abs_max = util.compute_all_2args('.txt', sequence, ops3)
#print u_abs_max
# Plot results 
#vis.simple_plot([u_ave, u_max], ['Average', 'Max'], sequence, 'time, ts', 'velocity, lu/ts', 'Velocity magnitude')
# # # # # # # # # # # # # # # # # # # # #

# # # # # # # # # # # # # # # # # # # # #
# 2) Paraview - simple surface plot of velocity magnitude  
# - - - Input - - -
# System dimensions
# x
#ncol = 200
## y
#nrow = 96
## Data
#fname_ux = './arrays/rectangular/ux.txt'
#fname_uy = './arrays/rectangular/uy.txt'
## Output
#fout = './arrays/rectangular/vel_mag.csv'
## - - - - - - - - -
## Data
#ux, uy = util.read_all([fname_ux, fname_uy])
## Velocity magnitude
#u_mag = clc.vel_mag(ux, uy) 
## CSV file for Paraview
#with open(fout, 'w') as fo:
#	fo.write('x coord, y coord, z coord, scalar\n')
#	for x in np.arange(0,ncol):
#		for y in np.arange(0,nrow):
#			fo.write(','.join(map(str, [x,y,0.0,u_mag[x][y]])) + '\n')
# # # # # # # # # # # # # # # # # # # # #

# # # # # # # # # # # # # # # # # # # # #
# 3) Compute and plot Reynolds number development
# - - - Input - - -
# System dimensions
# x
ncol = 200
# y
nrow = 96
# Data
fname_uy = './arrays/rectangular/uy_'
sequence = np.arange(10, 1.5e3+1, 10, np.int32)
# - - - - - - - - -
fnames = [fname_uy + str(i) + '.txt' for i in sequence]
bRe = partial(clc.Re, 50, True)
data = util.apply_all(bRe, util.read_all(fnames))
# Plot results 
vis.simple_plot([data], ['x = 50'], sequence, 'time, ts', 'Re, [-]', 'Reynolds number')
# # # # # # # # # # # # # # # # # # # # #
