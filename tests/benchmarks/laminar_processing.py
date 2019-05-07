#!/usr/bin/python2.7

import sys
import matplotlib.pyplot as plt
import numpy as np
from numpy import linalg as la

# Path to custom python modules
src_path = '/Users/atruszkowska/projects/LBM_C++/array_v1/tests/benchmarks/lbm_python/'
sys.path.insert(0, src_path)

import lbm_vis as vis
import lbm_utils as util
import lbm_calc as clc

# # # # # # # # # # # # # # # # # # # # #
# 1) Plot data from specific instances
# of files in a sequence
#
# - - - Input - - - 
#fname = './laminar/uy_'
#ext = '.txt'
#row = 5
#ncol = 51 
# Saves to use for plotting
#steps = list(range(1000, 6000, 1000))
#steps.extend((500,10000))
#steps.sort()
# x axis
#x = np.arange(0,ncol)
#lab = ['x, lu', 'uy, lu/ts', 'Development of y velocity profile']
## Currently needs manual adjustment of fig vs. legend size 
#vis.plot_sequence(fname, ext, row, steps, x, plt.cm.ocean, lab)
# # # # # # # # # # # # # # # # # # # # #

# # # # # # # # # # # # # # # # # # # # #
# 2) Plot with analytical solution 
#
# - - - Input - - -
# Channel dimensions
ncol = 51
nrow = 11
# Which row to compare/plot
row = 5
dPdL = 1e-5
fname = './laminar/uy.txt'
# Analytical
x_an, uy_an = clc.laminar_flow(ncol, nrow, dPdL)
# Computed
uy = np.loadtxt(fname)[row,:]
# 
plt.plot(x_an, uy_an, 'k', linewidth=2.0, label='Analytical solution')
plt.plot(x_an, uy, 'r-.', linewidth=2.0, label='Computed')
plt.legend(loc='best')
plt.xlabel('x, lu')
plt.ylabel('uy, lu/ts')
plt.grid()
plt.show()
# # # # # # # # # # # # # # # # # # # # #


