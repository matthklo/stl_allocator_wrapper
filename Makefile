.PHONY: all clean

all: demo

demo: demo.cpp
	g++ -o demo --std=c++11 demo.cpp

clean:
	rm -f demo

