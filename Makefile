CC = g++
CFLAGS = -g -Wall -std=c++11
CFLAGS += -I/usr/local/Cellar/boost/1.57.0/include
EXECUTABLE = panda
SOURCES = main.cpp board.cpp defs.cpp utils.cpp search.cpp eval.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -I. -o $@ $(OBJECTS)

%.o: %.cpp
	$(CC) $(CFLAGS) -I. -c -o $@ $<

clean:
	$(RM) *.o *.d *~ $(EXECUTABLE)

#CFLAGS += -MMD
#-include $(OBJECTS:.o=.d)