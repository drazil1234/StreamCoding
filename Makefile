CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
OBJS = 
GTESTFLAGS = gtest.o -lpthread

.PHONY: clean runtest

all: $(OBJS)

test: RNGTest

RNGTest: RNGTest.cpp gtest.o BlockCipher.o hash.o
	$(CXX) $(CXXFLAGS) $(GTESTFLAGS) RNGTest.cpp BlockCipher.o hash.o -o RNGTest

runtest: test
	./RNGTest

gtest.o: gtest/gtest-all.cc gtest/gtest.h
	$(CXX) $(CXXFLAGS) -isystem . -pthread gtest/gtest-all.cc -c -o gtest.o

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	-rm *.o
	-rm  RNGTest
