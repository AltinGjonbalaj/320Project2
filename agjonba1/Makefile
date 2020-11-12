OBJS = main.o cache.o
CFLAGS = -Wall -g
CC = g++


all: cache
	
predictors:	$(OBJS)
	$(CC) $(CFLAGS) -o $(@) $(OBJS)

.cpp.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean all

clean:
	rm -rf *.o $(cache)

