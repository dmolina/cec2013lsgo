CC=g++
# CXXFLAGS=-Wall -pedantic -std=c++11 -O3
CXXFLAGS=-Wall -pedantic -std=c++0x  -ggdb -DDEBUG

OBJECTS=demo.o  Benchmarks.o \
F1.o F2.o F3.o F4.o F5.o F6.o F7.o F8.o F9.o F10.o\
F11.o F12.o F13.o F14.o F15.o 

demo: 	$(OBJECTS)
	$(CC) $(CXXFLAGS) -o demo $(OBJECTS)

demo.o: demo.cpp Header.h  Benchmarks.h \
F1.h F2.h F3.h F4.h F5.h F6.h F7.h F8.h F9.h F10.h\
F11.h F12.h F13.h F14.h F15.h 
	$(CC) $(CXXFLAGS) -c demo.cpp 

Benchmarks.o:  Benchmarks.h Benchmarks.cpp
	$(CC) $(CXXFLAGS) -c Benchmarks.cpp

F1.o: F1.h Benchmarks.h F1.cpp
	$(CC) $(CXXFLAGS) -c F1.cpp

F2.o: F2.h Benchmarks.h F2.cpp
	$(CC) $(CXXFLAGS) -c F2.cpp

F3.o: F3.h Benchmarks.h F3.cpp
	$(CC) $(CXXFLAGS) -c F3.cpp

F4.o: F4.h Benchmarks.h F4.cpp
	$(CC) $(CXXFLAGS) -c F4.cpp

F5.o: F5.h Benchmarks.h F5.cpp
	$(CC) $(CXXFLAGS) -c F5.cpp

F6.o: F6.h Benchmarks.h F6.cpp
	$(CC) $(CXXFLAGS) -c F6.cpp

F7.o: F7.h Benchmarks.h F7.cpp
	$(CC) $(CXXFLAGS) -c F7.cpp

F8.o: F8.h Benchmarks.h F8.cpp
	$(CC) $(CXXFLAGS) -c F8.cpp

F9.o: F9.h Benchmarks.h F9.cpp
	$(CC) $(CXXFLAGS) -c F9.cpp

F10.o: F10.h Benchmarks.h F10.cpp
	$(CC) $(CXXFLAGS) -c F10.cpp

F11.o: F11.h Benchmarks.h F11.cpp
	$(CC) $(CXXFLAGS) -c F11.cpp

F12.o: F12.h Benchmarks.h F12.cpp
	$(CC) $(CXXFLAGS) -c F12.cpp

F13.o: F13.h Benchmarks.h F13.cpp
	$(CC) $(CXXFLAGS) -c F13.cpp

F14.o: F14.h Benchmarks.h F14.cpp
	$(CC) $(CXXFLAGS) -c F14.cpp

F15.o: F15.h Benchmarks.h F15.cpp
	$(CC) $(CXXFLAGS) -c F15.cpp

.PHONY : clean
clean:
	rm -f demo $(OBJECTS)
