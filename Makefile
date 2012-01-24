all:
	g++ -o worker worker.cpp tokencrypto.cpp -Wall -ansi -pedantic-errors -lcryptopp -lpthread
