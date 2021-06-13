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

inc_prefix = include
src_prefix = src
obj_prefix = tmp
obj_names = Utility.o Particle.o System.o Interface.o
lib_prefix = lib
lib_names = libano.so
bin_prefix = bin
bin_names = Simulate
dep_prefix = $(obj_prefix)
dep_names = $(obj_names:%.o=%.d) $(bin_names:%=%.d)

bin_to_lib_prefix = ../lib

objs = $(obj_names:%=$(obj_prefix)/%)
libs = $(lib_names:%=$(lib_prefix)/%)
bins = $(bin_names:%=$(bin_prefix)/%)
deps = $(dep_names:%=$(dep_prefix)/%)
pres = $(obj_prefix) $(lib_prefix) $(bin_prefix)

CXXFLAGS = -O2 -I$(inc_prefix) $(OUTER_CXXFLAGS)
LDFLAGS = -L$(lib_prefix) -lano \
					-Wl,-rpath=. \
					-Wl,-rpath=$(lib_prefix) \
					-Wl,-rpath=$(bin_to_lib_prefix)

all : libs bins

libs : $(libs)

bins : $(bins)

clean :
	$(RM) $(objs) $(libs) $(bins) $(deps)

$(obj_prefix)/%.o : $(src_prefix)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ -c -fPIC
	@$(RM) $(@:%.o=%.d)

$(lib_prefix)/libano.so : $(objs)
	$(CXX) $(CXXFLAGS) $^ -o $@ -shared -fPIC

$(bin_prefix)/% : $(obj_prefix)/%.o $(libs)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

$(dep_prefix)/%.d : $(src_prefix)/%.cpp $(pres)
	@$(CXX) $(CXXFLAGS) $< -MM >$@

$(pres) :
	mkdir -p $@

include $(deps)
