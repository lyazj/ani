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

import os
import platform
import ctypes

lib_prefix = os.path.join(os.path.dirname(__file__), '..', 'lib', 'lib')
if platform.system() == 'Windows':
  lib_postfix = f'{platform.architecture()[0][:2]}.dll'
else:
  lib_postfix = '.so'

def get_lib_filename(lib_name):
  return f'{lib_prefix}{lib_name}{lib_postfix}'

def load_lib(lib_name):
  return ctypes.CDLL(get_lib_filename(lib_name))

def load_lib_symbol(lib_name, symbol_name):
  return load_lib(lib_name).symbol_name

lib_name = 'ani'
lib = load_lib(lib_name)

def simulate(infile = '-', outfile = '-', prolong = 0):
  return lib.simulate_prolong(infile.encode(), outfile.encode(), prolong)
