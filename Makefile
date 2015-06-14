CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
OBJS = 
GTESTFLAGS = gtest.o -lpthread

.PHONY: clean runtest

all: $(OBJS)

test: RNGTest TreeCodeTest BlueBerryTest

TreeCodeTest: TreeCode.o TreeCodeTest.cpp DRNG.o sts/rngtest.o BlockCipher.o hash.o
	$(CXX) $(CXXFLAGS) $(GTESTFLAGS) TreeCodeTest.cpp DRNG.o sts/rngtest.o BlockCipher.o hash.o TreeCode.o -o TreeCodeTest

RNGTest: RNGTest.cpp gtest.o BlockCipher.o hash.o sts/rngtest.o DRNG.o
	$(CXX) $(CXXFLAGS) $(GTESTFLAGS) RNGTest.cpp BlockCipher.o hash.o DRNG.o sts/rngtest.o -o RNGTest

runtest: test
	./TreeCodeTest
	#./RNGTest
	#./BlueBerryTest

BlueBerryTest: BlueBerryTest.cpp gtest.o BlueBerryCode.o
	$(CXX) $(CXXFLAGS) $(GTESTFLAGS) BlueBerryTest.cpp BlueBerryCode.o -o BlueBerryTest 

gtest.o: gtest/gtest-all.cc gtest/gtest.h
	$(CXX) $(CXXFLAGS) -isystem . -pthread gtest/gtest-all.cc -c -o gtest.o

sts/rngtest.o: sts/rngtest.h
	$(MAKE) -C sts

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	-rm *.o
	-rm RNGTest
	-rm TreeCodeTest
	-rm  BlueBerryTest
