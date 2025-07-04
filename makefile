CXX = c++
CXXFLAGS = -std=c++14
main: src/*.cpp
	$(CXX) $(CXXFLAGS) src/*.cpp -o main

test: tests/*.cpp $(filter-out src/main.cpp,$(wildcard src/*.cpp))
	$(CXX) $(CXXFLAGS) $^ -o test_runner

clean:
	rm -f main test_runner *.o
