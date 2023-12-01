CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: main

main: main.cpp conversao.cpp conversao.h
	$(CXX) $(CXXFLAGS) -o main main.cpp conversao.cpp

clean:
	rm -f main

