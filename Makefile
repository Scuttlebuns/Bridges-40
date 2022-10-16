CC=g++

TARGET =  all 

SRCS = main.cc

OBJS = $(SRCS:.cc=.o)

EXE = $(SRCS:.cc=)

CPPFLAGS = -g -O1 -std=c++2a -fsanitize=address -DMAKEFILE
LDFLAGS = -fsanitize=address

LIBS =  -lcurl

.SUFFIXES: .cc

.cpp.o:  
	$(CC) $(CPPFLAGS)  $< -o $@

all : a.out

a.out: main.o
	$(CC) -g main.o $(LIBS) $(LDFLAGS)

clean:
	rm -rf *.o a.out core
