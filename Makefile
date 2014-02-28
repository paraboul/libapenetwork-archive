CC=gcc
DEBUG_FLAGS=-g
RELEASE_FLAGS=-O2
CFLAGS= $(RELEASE_FLAGS) $(DEBUG_FLAGS) -Wall -Wextra -fvisibility=hidden -Wno-unused-parameter
INC=-I./c-ares/
AR=ar
ARFLAGS=rcs
SOURCES=native_netlib.c ape_pool.c ape_http_parser.c ape_array.c ape_buffer.c ape_events.c ape_event_epoll.c ape_events_loop.c ape_socket.c ape_timers_next.c ape_base64.c ape_timers.c ape_dns.c ape_event_select.c ape_event_kqueue.c ape_hash.c	
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=libnativenetwork.a

all: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS) 
	$(AR) $(ARFLAGS) $@ $(OBJECTS) 

$(SOURCES): c-ares/libcares.la

.c.o:
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

c-ares/libcares.la: c-ares/Makefile
	cd c-ares;make

c-ares/Makefile:
	wget http://c-ares.haxx.se/download/c-ares-1.10.0.tar.gz
	tar -xaf c-ares-1.10.0.tar.gz
	rm c-ares-1.10.0.tar.gz
	mv c-ares-1.10.0 c-ares
	cd c-ares;./configure --enable-static --enable-shared

.PHONEY: clean propperclean

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	
propperclean:
	rm -rf c-ares
