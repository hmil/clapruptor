CC=avr-gcc
CFLAGS=-g -O3 -mmcu=attiny10 -DF_CPU=1000000UL

BIN=main.hex
AVRDUDE_CONF=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf
AVRISP=/dev/cu.usbmodem14201

OBJ := main.o

.PHONY: build
build: $(BIN)

%.elf: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

.PHONY: upload
upload: $(BIN)
	# avrdude -C $(AVRDUDE_CONF) -p t10 -P $(AVRISP) -c avrisp -b 19200 -v -U flash:w:$(BIN)
	avrdude -C $(AVRDUDE_CONF) -p t10 -c usbasp -v -U flash:w:$(BIN)
	# avrdude -C $(AVRDUDE_CONF) -pm8 -P $(AVRISP) -c avrisp -b 19200 -v -U flash:w:usbasp.atmega8.2011-05-28.hex:i

.PHONY: fuses
fuses:
	# avrdude -C $(AVRDUDE_CONF) -p t10 -c usbasp -b 19200 -v
	avrdude -C $(AVRDUDE_CONF) -p t10 -c usbasp -b 19200 -v -U lfuse:w:con:r
