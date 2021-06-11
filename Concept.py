#!/usr/bin/env python3
#
#    Copyright (C) 2021 @lyazj
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <https://www.gnu.org/licenses/>.

import ctypes
import re
import numpy as np
import matplotlib.pyplot as plt
import imageio
import os
import multiprocessing as mp
import platform

infile = 'input.txt'
outfile = 'output.txt'
soname = 'libano.so'
sofile = os.path.join('.', soname)
if(platform.system() == 'Windows'):
  make = 'mingw32-make'
  clean = 'clean -e "RM=del /f /q"'
else:
  make = 'make'
  clean = 'clean'
mbits = platform.architecture()[0][:2]

class Particle:
  def __init__(self, args):
    self.no = args[0]
    self.name = args[1]
    self.r = np.array(list(map(float, args[2:5])))
    self.v = np.array(list(map(float, args[5:8])))
    self.e = args[8]
    self.m = args[9]
    self.death = args[10]
  def __repr__(self):
    return f'<{self.no}>{self.name}@{self.r}'

particles = []

try:
  simulate = ctypes.CDLL(sofile).simulate_prolong
except:
  os.system(f'{make} {clean} && {make} {soname}'
      f' -j {mp.cpu_count()} -e OUTER_CXXFLAGS=-m{mbits}')
  simulate = ctypes.CDLL(sofile).simulate_prolong

prolong = 15

if not simulate(infile.encode(), outfile.encode(), prolong):
  with open(outfile, 'r') as of:
    out = of.read()
  beg = out.find('\n', out.find('complete process (all)')) + 1
  end = out.rfind('\n', beg, out.find('end of listing', beg))
  if beg == 0 or end == -1:
    print("ERROR: Invalid output from function `simulate`.")
  else:
    for i in re.split(r'\[.*\]', out[beg:end])[1:]:
      particles.append([])
      for j in i.strip().split('\n')[1:]:
        particles[-1].append(Particle(j.strip().split()))

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    for ph in range(len(particles)):
      ax.set_xlim([-25, 25])
      ax.set_ylim([-25, 25])
      ax.set_zlim([-25, 25])
      print('Plotting phase:', ph)
      for p in particles[ph]:
        # print('Plotting particle:', p)
        ax.plot(*p.r, 'o')
      print(f'Saving image: phase_{ph}.png... ', end = '', flush = True)
      plt.savefig(f'phase_{ph}.png')
      plt.cla()
      print('OK!')
      
    print('Generating gif file... ', flush = True, end = '')
    imageio.mimsave('concept.gif', (imageio.imread(f'phase_{ph}.png')
        for ph in range(len(particles))), 'GIF', duration = 0.05)
    print('OK!')
