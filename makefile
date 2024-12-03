CXX = c++
CXXFLAGS = -std=c++11
main: src/*.cpp
	$(CXX) $(CXXFLAGS) src/*.cpp -o main

test: tests/test_tables.cpp src/bitboard.cpp src/common.cpp src/init.cpp
	$(CXX) $(CXXFLAGS) $^ -o test_runner

clean:
	rm -f main test_runner *.o
