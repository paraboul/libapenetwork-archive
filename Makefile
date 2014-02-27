CC=gcc
DEBUG_FLAGS=-g
RELEASE_FLAGS=-O2
CFLAGS= $(RELEASE_FLAGS) $(DEBUG_FLAGS) -Wall -Wextra -fvisibility=hidden
INC=-I./c-ares/
AR=ar
ARFLAGS=rcs
SOURCES=native_netlib.c ape_pool.c ape_hash.c ape_http_parser.c ape_array.c ape_buffer.c ape_events.c ape_event_kqueue.c ape_event_epoll.c ape_event_select.c ape_events_loop.c ape_socket.c ape_dns.c ape_timers.c ape_timers_next.c ape_base64.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=libnativenetwork.a

all: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS) 

.c.o:
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

.PHONEY: clean

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
