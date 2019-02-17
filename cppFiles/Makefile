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

test_h.o: test_h.cpp utility.h defines.h test_h.h
	$(CXX) $(CXXFLAGS) -c $<

test.o: test.cpp defines.h utility.h test_h.h
	$(CXX) $(CXXFLAGS) -c $<

test: test.o test_h.o utility.o
	$(CXX) $(CXXFLAGS) -o $@ $^
	./$@

clean:
	rm -rf *.o main

cleanDebug:
	rm *.dbg

cleanall: clean cleanDebug

gitCopy:
	cp *.cpp ../gitFolder/cppFiles/
	cp *.h ../gitFolder/cppFiles/
	rm ../gitFolder/cppFiles/test*
	cp Makefile ../gitFolder/cppFiles/
