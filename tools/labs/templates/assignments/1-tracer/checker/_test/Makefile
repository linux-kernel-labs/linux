#CPPFLAGS = -DDEBUG -DLOG_LEVEL=LOG_DEBUG
CFLAGS = -Wall -g -static -m32

.PHONY: all clean

all:
	$(CC) $(CFLAGS) -c tracer_test.c

clean:
	-rm -f *~ *.o
