#!/usr/bin/env -S make -f
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

CXXFLAGS = -O2 -Wall -Wshadow -Wextra $(OUTER_CXXFLAGS)
LDFLAGS = -L. -lano -Wl,-rpath=.

objs = Utility.o Particle.o System.o Interface.o
libs = libano.so
bins = Simulate
deps = $(objs:%.o=%.d) $(bins:%=%.d)

all : libs bins

libs : $(libs)

bins : $(bins)

clean :
	$(RM) $(objs) $(bins:%=%.o) $(libs) $(bins) $(deps)
	$(RM) output.txt phase_*.png concept.gif

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ -c -fPIC

libano.so : $(objs)
	$(CXX) $(CXXFLAGS) $^ -o $@ -shared -fPIC

% : %.o $(libs)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

%.d : %.cpp
	$(CXX) $(CXXFLAGS) $< -MM >$@

include $(deps)
