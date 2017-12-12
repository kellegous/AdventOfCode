ALL: 01 02 04

%.o: %.cc %.h
	g++ -Wall -std=c++11 -c -o $@ $<

01.o: 01.cc
	g++ -Wall -std=c++11 -c -o $@ $<

01: 01.o util.o status.o
	g++ -Wall -std=c++11 -o $@ $^

02: 02.cc
	g++ -Wall -o $@ $<

04.o: 04.cc
	g++ -Wall -std=c++11 -c -o $@ $<

04: 04.o util.o status.o
	g++ -Wall -std=c++11 -o $@ $^

clean:
	rm -f *.o 02 04