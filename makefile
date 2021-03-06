# version 1.0

all : build.out

build.out : temp/core.o temp/main.o temp/tests.o temp/algo.o
	g++ -o build.out temp/core.o temp/main.o temp/tests.o temp/algo.o

temp/core.o : code/core.cpp code/core.hpp
	g++ -c code/core.cpp -o temp/core.o

temp/tests.o : code/tests.cpp code/tests.hpp code/algo.hpp
	g++ -c code/tests.cpp -o temp/tests.o

temp/algo.o : code/algo.cpp code/algo.hpp code/core.hpp
	g++ -c code/algo.cpp -o temp/algo.o

temp/main.o : code/main.cpp code/tests.hpp
	g++ -c code/main.cpp -o temp/main.o
