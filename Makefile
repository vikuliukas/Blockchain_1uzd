CXXFLAGS=  -Wall -g

all: main.o funkcijos.o
	g++  main.o funkcijos.o -o main

main.o: main.cpp

funkcijos.o: funkcijos.cpp

test:	all
	./main

clean:
	rm -f *.o main hash.txt