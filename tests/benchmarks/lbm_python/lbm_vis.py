#!/usr/bin/python2.7

# Module for visualization

import matplotlib
import matplotlib.pyplot as plt
import numpy as np

# Plot data from specific instances
# of files in a sequence
def plot_sequence(fname, ext, row, steps, x, scheme, lab):
	''' Plot data from files with extension 
		**steps** at position row '''
	# fname, ext - filename base and extension (fname(#Number)ext)
	# row - row to plot
	# steps - list of steps i.e. file numbers to plot
	# x - x (or y) direction, list
	# scheme - color scheme obtained with cm.colorscheme
	# lab - x and y axis names and title (any sequence)
	# For map list go here
	# https://matplotlib.org/examples/color/colormaps_reference.html
	colors = scheme(np.linspace(0,1,len(steps)+1))
	colors = colors[:-1]
	labels = [str(step) for step in steps]
	matplotlib.rcParams.update({'font.size': 18})
	# Process and plot
	for step, color, label in zip(steps, colors, labels):
		temp = fname + str(step) + ext
		data = np.loadtxt(temp)[row,:]
		ax = plt.plot(x, data, color=color, linewidth=2.0, label=label)
	#
	plt.legend()
	plt.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)
	plt.tight_layout()
	plt.xlabel(lab[0])
	plt.ylabel(lab[1])
	plt.title(lab[2])
	plt.grid()
	plt.show()

def simple_plot(data, labels, xaxis, xlabel, ylabel, title, scheme = plt.cm.hsv):
	''' Plot one or mulitple variables '''
	# data - rows correspond to different datasets as
	#	specified in labels
	# labels - names of the datasets
	# xaxis - common x data to plot against
	# xlabel, ylabel, title - x and y axis labels, plot name
	# scheme - colorscheme
	colors = scheme(np.linspace(0,1,len(labels)+1))
	matplotlib.rcParams.update({'font.size': 18})
	for var, color, label in zip(data, colors, labels):
		ax = plt.plot(xaxis, var, color=color, linewidth=2.0, label=label)	
	plt.legend(loc="best")
	plt.xlabel(xlabel)
	plt.ylabel(ylabel)
	plt.title(title)
	plt.grid()
	plt.show()

def surface(data, x, y, labels, title, orientation, scheme = plt.cm.jet):
	''' Surface plot of a 2D grid '''
	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')
	surf = ax.plot_surface(x, y, data, cmap=scheme, linewidth=0, antialiased=False)
	ax.set_aspect('equal')
	ax.view_init(90, 90)
	plt.colorbar(surf)
	plt.show()









