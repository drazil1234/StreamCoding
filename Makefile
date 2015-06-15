CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
OBJS = Socket.o DRNG.o sts/rngtest.o hash.o BlockCipher.o BlueBerryCode.o TreeCode.o
GTESTFLAGS = gtest.o -lpthread

.PHONY: clean runtest

all: $(OBJS) core Noise

test: RNGTest TreeCodeTest BlueBerryTest NoiseTest

NoiseTest: Socket.o Noise NoiseTest.cpp gtest.o
	$(CXX) $(CXXFLAGS) $(GTESTFLAGS) NoiseTest.cpp Socket.o -o NoiseTest

Noise: Noise.cpp Socket.o DRNG.o BlockCipher.o hash.o sts/rngtest.o
	$(CXX) $(CXXFLAGS) Noise.cpp Socket.o DRNG.o BlockCipher.o hash.o sts/rngtest.o -o Noise

TreeCodeTest: TreeCode.o TreeCodeTest.cpp gtest.o DRNG.o sts/rngtest.o BlockCipher.o hash.o
	$(CXX) $(CXXFLAGS) $(GTESTFLAGS) TreeCodeTest.cpp DRNG.o sts/rngtest.o BlockCipher.o hash.o TreeCode.o -o TreeCodeTest

RNGTest: RNGTest.cpp gtest.o BlockCipher.o hash.o sts/rngtest.o DRNG.o
	$(CXX) $(CXXFLAGS) $(GTESTFLAGS) RNGTest.cpp BlockCipher.o hash.o DRNG.o sts/rngtest.o -o RNGTest

runtest: test
	./NoiseTest
	#./TreeCodeTest
	#./RNGTest
	#./BlueBerryTest

BlueBerryTest: BlueBerryTest.cpp gtest.o BlueBerryCode.o
	$(CXX) $(CXXFLAGS) $(GTESTFLAGS) BlueBerryTest.cpp BlueBerryCode.o -o BlueBerryTest 

gtest.o: gtest/gtest-all.cc gtest/gtest.h
	$(CXX) $(CXXFLAGS) -isystem . -pthread gtest/gtest-all.cc -c -o gtest.o

sts/rngtest.o: sts/rngtest.h
	$(MAKE) -C sts

core: core.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) core.cpp -pthread $(OBJS) -o core

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	-rm *.o
	-rm RNGTest
	-rm TreeCodeTest
	-rm BlueBerryTest
	-rm NoiseTest
	-rm core
