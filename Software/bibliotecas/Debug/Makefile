################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../ATmega328.c \
../can_buffer.c \
../main.c \
../mcp2515.c \
../mcp2515_buffer.c \
../mcp2515_error_register.c \
../mcp2515_get_dyn_filter.c \
../mcp2515_get_message.c \
../mcp2515_read_id.c \
../mcp2515_regdump.c \
../mcp2515_send_message.c \
../mcp2515_set_dyn_filter.c \
../mcp2515_set_mode.c \
../mcp2515_sleep.c \
../mcp2515_static_filter.c \
../mcp2515_write_id.c \
../spi.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
ATmega328.o \
can_buffer.o \
main.o \
mcp2515.o \
mcp2515_buffer.o \
mcp2515_error_register.o \
mcp2515_get_dyn_filter.o \
mcp2515_get_message.o \
mcp2515_read_id.o \
mcp2515_regdump.o \
mcp2515_send_message.o \
mcp2515_set_dyn_filter.o \
mcp2515_set_mode.o \
mcp2515_sleep.o \
mcp2515_static_filter.o \
mcp2515_write_id.o \
spi.o

OBJS_AS_ARGS +=  \
ATmega328.o \
can_buffer.o \
main.o \
mcp2515.o \
mcp2515_buffer.o \
mcp2515_error_register.o \
mcp2515_get_dyn_filter.o \
mcp2515_get_message.o \
mcp2515_read_id.o \
mcp2515_regdump.o \
mcp2515_send_message.o \
mcp2515_set_dyn_filter.o \
mcp2515_set_mode.o \
mcp2515_sleep.o \
mcp2515_static_filter.o \
mcp2515_write_id.o \
spi.o

C_DEPS +=  \
ATmega328.d \
can_buffer.d \
main.d \
mcp2515.d \
mcp2515_buffer.d \
mcp2515_error_register.d \
mcp2515_get_dyn_filter.d \
mcp2515_get_message.d \
mcp2515_read_id.d \
mcp2515_regdump.d \
mcp2515_send_message.d \
mcp2515_set_dyn_filter.d \
mcp2515_set_mode.d \
mcp2515_sleep.d \
mcp2515_static_filter.d \
mcp2515_write_id.d \
spi.d

C_DEPS_AS_ARGS +=  \
ATmega328.d \
can_buffer.d \
main.d \
mcp2515.d \
mcp2515_buffer.d \
mcp2515_error_register.d \
mcp2515_get_dyn_filter.d \
mcp2515_get_message.d \
mcp2515_read_id.d \
mcp2515_regdump.d \
mcp2515_send_message.d \
mcp2515_set_dyn_filter.d \
mcp2515_set_mode.d \
mcp2515_sleep.d \
mcp2515_static_filter.d \
mcp2515_write_id.d \
spi.d

OUTPUT_FILE_PATH +=GccApplication1.elf

OUTPUT_FILE_PATH_AS_ARGS +=GccApplication1.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler



































./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.106\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.106\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="GccApplication1.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.106\gcc\dev\atmega328p"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "GccApplication1.elf" "GccApplication1.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "GccApplication1.elf" "GccApplication1.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "GccApplication1.elf" > "GccApplication1.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "GccApplication1.elf" "GccApplication1.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "GccApplication1.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "GccApplication1.elf" "GccApplication1.a" "GccApplication1.hex" "GccApplication1.lss" "GccApplication1.eep" "GccApplication1.map" "GccApplication1.srec" "GccApplication1.usersignatures"
	