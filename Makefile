LIBHOSE = ../powerhose/powerhose/libhose/# FIXME (what's the correct way to handle libs?)
BIN = worker

all: copy-libhose protobuf-cpp
	g++ -o $(BIN) worker.cpp tokencrypto.cpp response.pb.cc libhose.o -Wall -ansi -pedantic-errors -lcryptopp -lpthread -lzmq -lprotobuf

copy-libhose:
	cp $(LIBHOSE)libhose.o .
	cp $(LIBHOSE)libhose.h .

protobuf-cpp:
	protoc response.proto --cpp_out=.

serve: all
	./$(BIN)&

protobuf-py:
	protoc response.proto --python_out=.

consume: protobuf-py
	python consumer.py

