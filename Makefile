CFLAGS := -Wall -std=c++11
LDFLAGS :=
OS := $(shell uname -s)

PROGS := 01 02 04 05 06

ifneq ($(OS), Darwin)
	LDFLAGS += -pthread
endif

ALL: $(PROGS)

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

05: 05.o util.o status.o
	g++ $(LDFLAGS) -o $@ $^

06: 06.o util.o status.o
	g++ $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o $(PROGS)