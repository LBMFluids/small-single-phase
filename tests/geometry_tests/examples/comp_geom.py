#!/usr/bin/python2.7

# Script for computing geometric properties

import math

# Set 1
square = [4,8]
array = [5,45,10,90,5,5]
alpha = 30
beta = 180-2.0*alpha

# Set 2
#square = [4,2]
#array = [5,45,10,90,5,5]
#alpha = 75
#beta = 180-2.0*alpha

# Set 3
#square = [2,4]
#array = [5,45,10,90,5,5]
#alpha = 75
#beta = 180-2.0*alpha

alrad = alpha*math.pi/180.0
berad = beta*math.pi/180.0

dx_alpha = math.sin(alrad)/math.sin(berad)*(array[4]+square[0])
dxs = math.floor((array[4]+square[0])/2.0)
dys = math.floor(math.sqrt(dx_alpha*dx_alpha-dxs*dxs))

print '- '*20
print 'Diagonal distance dx_alpha: ' + str(dx_alpha)
print 'Staggered center-to-center distance in x, dxs: ' + str(dxs) 
print 'Staggered center-to-center distance in y, dys: ' + str(dys)
print 'Initial staggered coordinates, xs0: ' + str(array[0]+dxs) + ' ys0: ' + str(array[2]+dys)
print 'xsF: ' + str(array[1]-dxs) + ' ysF: ' + str(array[3]-dys)
print '- '*20
