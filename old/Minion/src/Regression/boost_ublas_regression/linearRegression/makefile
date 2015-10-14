CC     = g++
CFLAGS = -g

all: main
 
main: main.o dataset.o linearRegr.o
# Commands start with TAB not spaces
	$(CC) $(LDFLAGS) -o $@ $^
 
main.o: main.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

dataset.o: dataset.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
 
lineraRegr.o: linearRegr.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main *.o
