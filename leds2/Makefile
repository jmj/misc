#PREFIX=/usr/local/msp430-gcc-4.4.3
PREFIX=/opt/msp430-gcc-4.4.5
CC=$(PREFIX)/bin/msp430-gcc

#CFLAGS= -Wall -g -mmcu=msp430x2012
CFLAGS=-Os -Wall -g -mmcu=msp430x2012
#CFLAGS=-Os -Wall -g -I/opt/msp430-gcc-4.4.5/msp430/include

#OBJS=reg.o morse.o main.o
OBJS=main.o


all: $(OBJS)
	$(CC) $(CFLAGS) $(EXTRA) -o main.elf $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -fr main.elf $(OBJS)
