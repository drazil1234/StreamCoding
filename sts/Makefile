C=clang
CXX=clang++
CFLAGS=-Wall -O2
CXXFLAGS=-Wall -O2 --std=c++11
OBJS=frequency.o cephes.o blockFrequency.o runs.o longestRunOfOnes.o matrix.o rank.o \
		 dfft.o discreteFourierTransform.o nonOverlappingTemplateMatchings.o \
		 overlappingTemplateMatchings.o universal.o linearComplexity.o serial.o \
		 approximateEntropy.o cusum.o randomExcursions.o randomExcursionsVariant.o

.PHONY=clean

all: rngtest.o

rngtest.o: $(OBJS)
	ld -r $(OBJS) -o rngtest.o

test: $(OBJS) test.cpp
	$(CXX) $(CXXFLAGS) test.cpp $(OBJS) -o test

#omit some deps here cause this is third party lib
#we don't expect to change *.h no matter what
%.o: %.c 
	$(C) $(CFLAGS) -c -o $@ $<

clean:
	-rm *.o
	-rm test

