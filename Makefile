LIBHOSE = ../powerhose/powerhose/libhose/# FIXME (what's the correct way to handle libs?)

all: copy-libhose protobuf
	g++ -o worker worker.cpp tokencrypto.cpp response.pb.cc libhose.o -Wall -ansi -pedantic-errors -lcryptopp -lpthread -lzmq -lprotobuf

copy-libhose:
	cp $(LIBHOSE)libhose.o .
	cp $(LIBHOSE)libhose.h .

protobuf:
	protoc response.proto --cpp_out=.
