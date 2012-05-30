all: tests/test.cpp api/dlm.cpp
	g++ tests/test.cpp api/dlm.cpp -o test
