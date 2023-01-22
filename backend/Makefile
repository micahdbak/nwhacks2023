BIN = ../bin

%.o: %.c
	cc -Wall -c -o $@ $<

backend: main.o thread.o
	mkdir -p $(BIN)
	cc -o $(BIN)/backend main.o thread.o
