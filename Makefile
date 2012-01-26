LIBDIR = $(CURDIR)/libs
BIN = worker

all: protobuf-cpp
	g++ -g -Wall -o $(BIN) response.pb.cc worker.cpp tokencrypto.cpp -lhose -lcryptopp -lprotobuf -lpthread -lzmq -Wextra -pedantic -I $(LIBDIR) -L $(LIBDIR)

protobuf-cpp:
	protoc response.proto --cpp_out=.

serve: all
	./$(BIN)&

protobuf-py:
	protoc response.proto --python_out=.

consume: protobuf-py
	python consumer.py
