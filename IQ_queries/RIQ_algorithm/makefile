CXX=g++
CC_FLAGS=-std=c++11



riq: riq.o
	${CXX} riq.o -o riq

riq.o: riq.cpp
	${CXX} -c ${CC_FLAGS} -o riq.o riq.cpp

clean:
	rm *.o
	rm riq
