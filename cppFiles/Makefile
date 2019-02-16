CXX = g++
CXXFLAGS = -std=c++11

.PHONY: clean cleanDebug cleanall test

all: main

run:
	./main

# File dependancy tree

utility.o: utility.cpp defines.h utility.h
	$(CXX) $(CXXFLAGS) -c $<

Archive.o: Archive.cpp defines.h Archive.h utility.h
	$(CXX) $(CXXFLAGS) -c $<

main.o: main.cpp Archive.h defines.h utility.h
	$(CXX) $(CXXFLAGS) -c $<

main: main.o Archive.o utility.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test: test.cpp
	$(CXX) $(CXXFLAGS) -o test test.cpp
	./test

clean:
	rm -rf *.o main

cleanDebug:
	rm *.dbg

cleanall: clean cleanDebug