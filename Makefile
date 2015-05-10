CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
OBJS = 

.PHONY: clean

all: $(OBJS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm *.o
