CXX = mpicxx -std=c++14

.PHONY: all clean
all: p01 p02 p03 p04 p05 p06 p07 p08 p09

p%: p%.cpp
	$(CXX) $^ -o $@

clean:
	rm -f p*.exe p*.
