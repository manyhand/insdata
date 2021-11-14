CXX=g++
CC_FLAGS=-std=c++11


rlin: rlin.o
	${CXX} rlin.o -o rlin

rlin.o: rlin.cpp
	${CXX} -c ${CC_FLAGS} -o rlin.o rlin.cpp

clean:
	rm *.o
	rm rlin
