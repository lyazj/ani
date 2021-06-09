#!/usr/bin/env -S make -f

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
