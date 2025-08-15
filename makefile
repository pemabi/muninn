CXX = c++
CXXFLAGS = -std=c++14 -g
PROFILE_FLAGS = -std=c++14 -g -O2
LDFLAGS = -framework Accelerate

main: src/*.cpp
	$(CXX) $(CXXFLAGS) src/*.cpp $(LDFLAGS) -o main

profile: src/*.cpp
	$(CXX) $(PROFILE_FLAGS) src/*.cpp $(LDFLAGS) -o main

test: tests/*.cpp $(filter-out src/main.cpp,$(wildcard src/*.cpp))
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o test_runner

clean:
	rm -f main test_runner *.o
