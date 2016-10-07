###############################################################################
# Makefile for the project placaAdicionalChopper
###############################################################################

## General Flags
PROJECT = placaAdicionalChopper
MCU = atmega328p
TARGET = ${PROJECT}.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d
## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=${PROJECT}.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = ${PROJECT}.o ATmega328.o mcp2515.o spi.o mcp2515_set_mode.o
OBJECTS += mcp2515_get_message.o mcp2515_read_id.o mcp2515_buffer.o 
OBJECTS += mcp2515_write_id.o mcp2515_send_message.o

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) ${PROJECT}.hex ${PROJECT}.eep ${PROJECT}.lss size

## Compile
${PROJECT}.o: ../${PROJECT}.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

%.o: ../bibliotecas/%.c
	echo $< $@
	$(CC) $(INCLUDES) $(CFLAGS) -c $<
	
##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) ${TARGET} dep/* ${PROJECT}.hex ${PROJECT}.eep ${PROJECT}.lss ${PROJECT}.map

grava: 
	sudo avrdude -c arduino -p m328p -P ~/../../dev/ttyACM0 -U flash:w:"${PROJECT}.hex":a

grava1: 
	sudo avrdude -c arduino -p m328p -P ~/../../dev/ttyACM1 -U flash:w:"${PROJECT}.hex":a
	
## Other dependencies
##-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

