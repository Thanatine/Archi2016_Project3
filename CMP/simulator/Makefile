CMP: simulator.o instruction.o
	g++ -g -o CMP simulator.o instruction.o
simulator.o: simulator.cpp
	g++ -g -c simulator.cpp
instruction.o: instruction.cpp instruction.h
	g++ -g -c instruction.cpp
clean:
	rm -f CMP simulator.o instruction.o
