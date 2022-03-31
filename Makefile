
test: Test.cpp AStar.hpp PriorityQueue.hpp
	$(CXX) -O3 $^ -o $@
	./test

clean:
	rm -f test
