CXX = mpicxx -std=c++14

.PHONY: all
all: p01 p02 p03 p04 p05 p06 p07

p01: p%: p%.cpp
	$(CXX) $^ -o $@
