#!/usr/bin/python2.7

# Script for visualizing LBM geometry files
# Prints some stats for basic checking

import argparse
import numpy as np
import scipy.sparse as sps
import matplotlib.pyplot as plt

hlp_msg = 'Visualize LBM geometry from file'
parser = argparse.ArgumentParser(description = hlp_msg)
parser.add_argument('-f', dest = 'filename', default = [''], type = str,
				  		nargs = 1, metavar = 'FILE', help='name of the input file')
args = parser.parse_args()
filename = args.filename[0]

if not filename:
	raise ValueError('No input filename provided')

geom = np.loadtxt(filename);

print 'Domain size: ' + str(geom.shape[0]) + 'x' + str(geom.shape[1])
print 'Number of solid nodes: ' + str(geom.size - np.nonzero(geom)[0].size)
print 'Number of fluid nodes: ' + str(np.nonzero(geom)[0].size)

# Fluid nodes
A_vis = sps.csr_matrix(geom)
# Solid nodes
geom_sld = geom
ind0 = np.where(geom_sld == 0)
ind1 = np.where(geom_sld == 1)
geom_sld[ind0[0],ind0[1]] = 1
geom_sld[ind1[0],ind1[1]] = 0
A_sld = sps.csr_matrix(geom_sld)

plt.spy(A_vis, c='b', markersize = 5)
plt.spy(A_sld, c='r', markersize = 5)
plt.gca().invert_yaxis()
plt.show()
