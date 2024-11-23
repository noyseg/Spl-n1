# Please implement your Makefile rules and targets below.
all: clean link run

link: compile
	@echo "Linking Object files"
	g++ -o bin/main bin/main.o bin/Auxiliary.o

compile: src/main.cpp src/Auxiliary.cpp 
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/main.o src/main.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp

run:
	./main

clean:
	rm -f main
# Customize this file to define how to build your project.