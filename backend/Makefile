BIN = ../bin

%.o: %.c
	cc -c -o $@ $<

backend: main.o
	cc -o $(BIN)/backend main.o
