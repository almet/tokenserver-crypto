all:
	g++ -o crypto crypto.cpp -Wall -ansi -pedantic-errors -lcryptopp -lpthread
