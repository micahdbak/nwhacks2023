BIN = ../bin

%.o: %.c
	cc -c -o $@ $<

backend: main.o
	mkdir -p $(BIN)
	cc -o $(BIN)/backend main.o
