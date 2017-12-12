CFLAGS := -Wall -std=c++11
LDFLAGS :=
OS := $(shell uname -s)

ifeq ($(OS), Linux)
	LDFLAGS += -pthread
else
	LDFLAGS += -lpthread
endif

ALL: 01 02 04

%.o: %.cc %.h
	g++ $(CFLAGS) -c -o $@ $<

%.o: %.cc
	g++ $(CFLAGS) -c -o $@ $<

01: 01.o util.o status.o
	g++ $(LDFLAGS) -o $@ $^

02: 02.o util.o status.o
	g++ $(LDFLAGS) -o $@ $^

04: 04.o util.o status.o
	g++ $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o 01 02 04