#!/usr/bin/python

# Compiles and runs all the tests for the geometry class

# NOTES: 
# - The entire python side has an awkward dimension flip (x becomes y compared to C++ interface) - to be resolved in the future
# - Even worse, objects and walls need a flip but arrays do not

# Last modified: 04/24/2019

import subprocess, glob
from geom_test import *

def tst_pss(res):
	if res:
		msg('Test passed', CYAN)
	else:
		msg('Test failed', GREEN)

### Input 
# Path to the main directory
path = '/Users/atruszkowska/projects/LBM_C++/array_v1/tests/geometry_tests/'
# Compiler options
cx = 'g++'
std = '-std=c++11'
opt = '-O3'
# Directory with temporary output (hardcoded in C++)
out_dir = 'tmp_files'
# Full path to that dir
fpath = path + out_dir + '/'
# All files to compile
files = 'geom_test_main.cpp geom_test_core.cpp'

### C++ side and postprocessing
# Create a temporary result directory (highly recommend
# to remove it before a new series of tests)
subprocess.call(['mkdir ' + fpath], shell=True)
# Compile and run the C++ side (all of it)
comp = ' '.join([cx, std, opt, '-o', 'gtst', files])
subprocess.call([comp], shell=True)
subprocess.call(['./gtst'], shell=True)
# Postprocess with Matlab (C++ output files without 
# extensions
test_list = ['rectangle_array', 'square_array', 'ellipse_array', 'circle_array', 'stg_array_rec', 'stg_array_circles']
for ftest in test_list:
	out_arr = fpath + ftest + '_out' + '.txt'
	comm = './run_m_proc.sh ' + fpath + ftest + '.txt ' + out_arr 
	subprocess.call([comm], shell=True)

### Run the actual tests

# Test 1 - rectangle
dx, dy, x0, y0 = 8, 4, 25, 10
rec = rectangle(dx, dy, x0, y0)
geom = np.loadtxt(fpath + 'rectangle.txt')
msg('Rectangle:', RED)
tst_pss(rec.correct(geom))

# Test 2 - square
dx, dy, x0, y0 = 4, 4, 25, 10
sq = rectangle(dx, dy, x0, y0)
geom = np.loadtxt(fpath + 'square.txt')
msg('Square:', RED)
tst_pss(sq.correct(geom))

# Test 3 - ellipse
dx, dy, x0, y0 = 50, 20, 250, 100
ell = ellipse(dx, dy, x0, y0)
geom = np.loadtxt(fpath + 'ellipse.txt')
msg('Ellipse:', RED)
tst_pss(ell.correct(geom))

# Test 4 - circle
dx, dy, x0, y0 = 20, 20, 250, 100
circ = ellipse(dx, dy, x0, y0)
geom = np.loadtxt(fpath + 'circle.txt')
msg('Circle:', RED)
tst_pss(circ.correct(geom))

# Test 5 - x-walls
dh, xy = 2, 'y'
wllx = walls(dh, xy)
geom = np.loadtxt(fpath + 'walls_x.txt')
msg('x-walls:', RED)
tst_pss(wllx.correct(geom))

# Test 6 - y-walls
dh, xy = 2, 'x'
wlly = walls(dh, xy)
geom = np.loadtxt(fpath + 'walls_y.txt')
msg('y-walls:', RED)
tst_pss(wlly.correct(geom))

# Test 7 - array of rectangles 
arr = obj_array(8, 2, 15, 63, 5, 20, 3, 3)
out_arr = fpath + 'rectangle_array_out.txt'
geom = np.loadtxt(out_arr)
msg('Array of rectangles:', RED)
tst_pss(arr.correct(geom))

# Test 8 - array of squares
arr = obj_array(2, 2, 2, 7, 2, 8, 2, 3)
out_arr = fpath + 'square_array_out.txt'
geom = np.loadtxt(out_arr)
msg('Array of squares:', RED)
tst_pss(arr.correct(geom))

# Test 9 - array of ellipses
arr = obj_array(16, 4, 15, 63, 5, 20, 3, 3)
out_arr = fpath + 'ellipse_array_out.txt'
geom = np.loadtxt(out_arr)
msg('Array of ellipses:', RED)
tst_pss(arr.correct(geom))

# Test 10 - array of circles
arr = obj_array(2, 2, 2, 7, 2, 8, 2, 3)
out_arr = fpath + 'circle_array_out.txt'
geom = np.loadtxt(out_arr)
msg('Array of circles:', RED)
tst_pss(arr.correct(geom))

# Test 11 - staggered array of rectangles
arr = stag_array(2, 4, 5, 45, 10, 90, 5, 5, 75)
out_arr = fpath + 'stg_array_rec_out.txt'
geom = np.loadtxt(out_arr)
msg('Staggered array of rectangles:', RED)
tst_pss(arr.correct(geom))

# Test 12 - staggered array of circles
arr = stag_array(8, 8, 10, 40, 10, 90, 5, 5, 60)
out_arr = fpath + 'stg_array_circles_out.txt'
geom = np.loadtxt(out_arr)
msg('Staggered array of circles:', RED)
tst_pss(arr.correct(geom))

# Test 13 - read/write test
frd = fpath + 'stg_array_circles.txt'
fwrt = fpath + 'geom_read_test.txt'
geom_1 = np.loadtxt(frd)
geom_2 = np.loadtxt(fwrt)
msg('Read/write test:', RED)
tst_pss(np.array_equal(geom_1, geom_2))
	
