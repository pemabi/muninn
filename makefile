CXX = c++
CXXFLAGS = -std=c++11
main: src/*.cpp
	$(CXX) $(CXXFLAGS) src/*.cpp -o main

test: tests/*.cpp \
			src/bitboard.cpp src/common.cpp src/display.cpp src/init.cpp src/magics.cpp src/move.cpp src/position.cpp
	$(CXX) $(CXXFLAGS) $^ -o test_runner

clean:
	rm -f main test_runner *.o
