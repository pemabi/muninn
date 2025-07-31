CXX = c++
CXXFLAGS = -std=c++14 -g
PROFILE_FLAGS = -std=c++14 -g -O2

main: src/*.cpp
	$(CXX) $(CXXFLAGS) src/*.cpp -o main

profile: src/*.cpp
	$(CXX) $(PROFILE_FLAGS) src/*.cpp -o main

test: tests/*.cpp $(filter-out src/main.cpp,$(wildcard src/*.cpp))
	$(CXX) $(CXXFLAGS) $^ -o test_runner

clean:
	rm -f main test_runner *.o
