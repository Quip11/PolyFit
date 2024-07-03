LIBS= -lgtest -lm -lstdc++
CC= gcc
CFLAGS= -O2 -Wall
CXX= g++
CXXFLAGS= $(CFLAGS) -std=c++17
LDFLAGS= -s

PROGRAM= Polynomial

all: $(PROGRAM)

%: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $* $*.cpp $(LIBS)

clean:
	rm -f $(PROGRAM) *.o *.a a.out core errlist

