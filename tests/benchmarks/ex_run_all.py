#!/usr/bin/python
# Example based test
# Compiles and runs all examples, saves data, and compares to expected solution

# Input: correct absolute path
# Output: none, prints if a test passed or failed

# Last modified: April 2nd 2019

import subprocess, glob

path = '/Users/atruszkowska/projects/LBM_C++/array_v1/tests/benchmarks/'

# Compiler options
cx = 'g++'
std = '-std=c++11'
opt = '-O3'
# Directory with data to compare to
comp_dir = 'comp_data'

def make_dir(new_dir):
	''' Create a new directory, prompt user if already there '''
	if glob.glob(new_dir):
		msg = 'Directory ' + new_dir + ' already exists. Exiting!'
		raise ValueError, msg
	else:
		subprocess.call(['mkdir ' + new_dir], shell=True)

# Common test directory
make_dir(path + 'temp_res')
path = path + 'temp_res/'

# Test 1
test_dir = path + 'test_1/'
name = 'test_1'
make_dir(test_dir)
comp = ' '.join([cx, std, opt, '-o', test_dir + name, 'just_fluid.cpp'])
subprocess.call([comp], shell=True)
subprocess.call([test_dir + name + ' ' + test_dir], shell=True)
# 

# Test 2
test_dir = path + 'test_2/'
name = 'test_2'
make_dir(test_dir)
comp = ' '.join([cx, std, opt, '-o', test_dir + name, 'laminar.cpp'])
subprocess.call([comp], shell=True)
subprocess.call([test_dir + name + ' ' + test_dir], shell=True)
#

# Test 3
test_dir = path + 'test_3/'
name = 'test_3'
make_dir(test_dir)
comp = ' '.join([cx, std, opt, '-o', test_dir + name, 'rec_array.cpp'])
subprocess.call([comp], shell=True)
subprocess.call([test_dir + name + ' ' + test_dir], shell=True)
#
