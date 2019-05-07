#!/usr/bin/python2.7

# Module with miscelaneous tools

import numpy as np

def compute_all(ext, sequence, fun):
	''' Apply functions on data from all files 
		numbered as defined in sequence '''
	# ext - file name extension
	# sequence - file numbers to process
	# fun - (file name, function to apply) tuples
	# Returns a numpy array with rows corresponding 
	# to results of processing one dataset
	# Only the final result returned (i.e. 5 functions then result of applying the 5th one, not the intermediates)
	# May have sequence ordering issues - to do
	temp = []
	for num in sequence:
		for fname, f in fun:
			if fname is None:
				data = f(data)
			else:
				data = f(np.loadtxt(fname + str(num) + ext))
		temp.append(data)
	return np.array(temp)

def compute_all_2args(ext, sequence, fun):
	''' Apply functions on two datasets from all files 
		numbered as defined in sequence '''
	# ext - file name extension
	# sequence - file numbers to process
	# fun - (file name 1, file name 2, function to apply) tuples
	# Returns a numpy array with rows corresponding 
	# to results of processing one dataset
	# Only the final result returned (i.e. 5 functions then result of applying the 5th one, not the intermediates)
	# May have sequence ordering issues - to do
	temp = []
	for num in sequence:
		for fname_1, fname_2, f, g in fun:
			if fname_1 is None:
				if not g is None:
					data_1 = f(data_1)
					data_2 = g(data_2)
				else:
					data = f(data_1, data_2)
			else:
				if not g is None:
					data_1 = f(np.loadtxt(fname_1 + str(num) + ext))
					data_2 = g(np.loadtxt(fname_2 + str(num) + ext))
				else:
					data_1 = np.loadtxt(fname_1 + str(num) + ext)
					data_2 = np.loadtxt(fname_2 + str(num) + ext)
					data = f(data_1, data_2)
		temp.append(data)
	return np.array(temp)

def apply_all(fun, data):
	''' Apply function fun to each 2D array stored
		in data '''
	temp = []
	for grid in data:
		temp.append(fun(grid))
	return temp

def read_all(file_names):
	''' Read files from the list as np arrays '''
	temp = []
	for f in file_names:
		temp.append(np.loadtxt(f))
	return temp
