CC=g++
CFLAGS=-c -g -std=c++11 -pthread
SOURCES=main.cpp pathfinder.cpp drawing_window.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lpthread
LDFLAGS=-pthread
PROGRAM=pathfinder

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS) $(LDFLAGS) $(LIBS) 

.cpp.o: 
	$(CC) $(CFLAGS) $<

depend: .depend

.depend: $(SOURCES)
	rm -f .depend
	$(CC) $(CFLAGS) -MM $^ >> .depend;

include .depend

clean:
	rm -f .depend $(PROGRAM) $(OBJECTS)

