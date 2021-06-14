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

# Available linkage: ELF WIN
linkage = ELF
# Available bits (for WIN only): 32, 64
bits = 64

obj_bases = Utility Particle System Interface
lib_bases = ani
bin_bases = sim
dep_bases = $(obj_bases) $(bin_bases)

inc_postfix = .h
src_postfix = .cpp
obj_postfix = .o
lib_postfix = .so
bin_postfix = 
dep_postfix = .d

inc_prefix = include
src_prefix = src
obj_prefix = tmp
lib_prefix = lib
bin_prefix = bin
bin_to_lib_prefix = ../lib

ifeq ($(linkage), ELF)
  CXX = g++
else ifeq ($(linkage), WIN)
  ifeq ($(bits), 32)
    CXX = i686-w64-mingw32-g++ -static-libgcc -static-libstdc++
    obj_postfix = 32.o
    lib_postfix = 32.dll
    bin_postfix = 32.exe
    dep_postfix = 32.d
  else ifeq ($(bits), 64)
    CXX = x86_64-w64-mingw32-g++ -static-libgcc -static-libstdc++
    obj_postfix = 64.o
    lib_postfix = 64.dll
    bin_postfix = 64.exe
    dep_postfix = 64.d
  endif
endif

obj_names = $(obj_bases:%=%$(obj_postfix))
lib_names = $(lib_bases:%=lib%$(lib_postfix))
bin_names = $(bin_bases:%=%$(bin_postfix))
dep_names = $(dep_bases:%=%$(dep_postfix))

objs = $(obj_names:%=$(obj_prefix)/%)
libs = $(lib_names:%=$(lib_prefix)/%)
bins = $(bin_names:%=$(bin_prefix)/%)
deps = $(dep_names:%=$(obj_prefix)/%)
pres = $(obj_prefix) $(lib_prefix) $(bin_prefix)

CXXFLAGS = -O2 -I$(inc_prefix)
LDFLAGS = -L$(lib_prefix) $(patsubst lib%,-l%,$(basename $(lib_names))) \
          -Wl,-rpath=. \
          -Wl,-rpath=$(lib_prefix) \
          -Wl,-rpath=$(bin_to_lib_prefix)

all : libs bins

libs : $(libs)

bins : $(bins)

clean :
	$(RM) $(libs) $(bins) $(deps) $(deps:%$(dep_postfix)=%$(obj_postfix))

$(obj_prefix)/%$(obj_postfix) : $(src_prefix)/%$(src_postfix)
	$(CXX) $(CXXFLAGS) $< -o $@ -c -fPIC

$(lib_prefix)/libani$(lib_postfix) : $(objs)
	$(CXX) $(CXXFLAGS) $^ -o $@ -shared -fPIC

$(bin_prefix)/%$(bin_postfix) : $(obj_prefix)/%$(obj_postfix) $(libs)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

$(obj_prefix)/%$(dep_postfix) : $(src_prefix)/%$(src_postfix) $(pres)
	@>$@ echo "$(subst  \ ,,$(patsubst %.o:,$(obj_prefix)/%$(obj_postfix):, \
		$(shell $(CXX) $(CXXFLAGS) $< -MM)))"

$(pres) :
	mkdir -p $@

include $(deps)
