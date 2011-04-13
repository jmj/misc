
INCLUDES = /home/jjones/src/arduino-0022/hardware/arduino/cores/arduino


CC = avr-gcc
CFLAGS = -Os -DF_CPU=16000000UL -mmcu=atmega328p -I$(INCLUDES)

all: main.hex

main.hex: main
	avr-objcopy -O ihex -R .eeprom main main.hex

main: main.o
	$(CC) $(CFLAGS) main.o -o main core.a

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

upload: main.hex
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:led.hex

clean:
	- rm main.hex main main.o
