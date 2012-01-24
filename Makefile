all:
	g++ -o tokencrypto tokencrypto.cpp -Wall -ansi -pedantic-errors -lcryptopp -lpthread
