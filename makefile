CFLAGS:= -Wall -std=c++17
CC:=g++

%.o: %.cpp
	$(CC) $(CFLAGS)  -o $@ -c $<

simulator: main.o simulator.o memory.o imem.o regfile.o decoder.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o *.exe
