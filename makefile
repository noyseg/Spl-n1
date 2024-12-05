# Please implement your Makefile rules and targets below.
# all: clean link run checkMemo
all: run

link: compile
	@echo "Linking Object files"
	g++ -o bin/main bin/main.o bin/Auxiliary.o bin/Settlement.o bin/Facility.o bin/SelectionPolicy.o bin/Plan.o bin/Action.o bin/Simulation.o

compile: src/main.cpp src/Auxiliary.cpp 
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/main.o src/main.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Settlement.o src/Settlement.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Facility.o src/Facility.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Plan.o src/Plan.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Action.o src/Action.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Simulation.o src/Simulation.cpp


run:
	./bin/simulation config_file.txt

clean:
	rm -f bin/*

checkMemo:
	valgrind --leak-check=full --show-reachable=yes ./bin/main config_file.txt
# Customize this file to define how to build your project.