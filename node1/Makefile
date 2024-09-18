# List all source files to be compiled; separate with space
SOURCE_FILES := \
	src/main.c \
	src/hal/hal_uart.c \
	src/log/log.c \
	src/hal/hal_gpio.c \
	src/application/gpio.c \
	src/application/sram.c

INCLUDE_PATHS := \
	-Isrc \
	-Isrc/application \
	-Isrc/hal \
	-Isrc/log

# Set this flag to "yes" (no quotes) to use JTAG; otherwise ISP (SPI) is used
PROGRAM_WITH_JTAG := yes

# Feel free to ignore anything below this line
PROGRAMMER := atmelice_isp
ifeq ($(PROGRAM_WITH_JTAG), yes)
	PROGRAMMER := atmelice
endif

BUILD_DIR := build
TARGET_CPU := atmega162
TARGET_DEVICE := m162
HEX_FILENAME := main.hex

CC := avr-gcc
CFLAGS := $(INCLUDE_PATHS) -O -std=c11 -mmcu=$(TARGET_CPU) -ggdb

# Replaces .c to .o in filename, removes directory list and appends the build dir in front.
# foo/bar.c -> foo/bar.o -> bar.o -> build/bar.o
OBJECT_FILES = $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCE_FILES:.c=.o)))

# Extracts directories from the list of source files and tells Makefile to look in those directories
vpath %.c $(dir $(SOURCE_FILES))

all: $(BUILD_DIR)/$(HEX_FILENAME)

$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "(CC) $<"
	@$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(HEX_FILENAME): $(OBJECT_FILES) | $(BUILD_DIR)
	@echo "(CC) a.out"
	@$(CC) $(CFLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)/a.out
	@echo "(CP) $(HEX_FILENAME)"
	@avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/a.out $(BUILD_DIR)/$(HEX_FILENAME)

.PHONY: flash
flash: $(BUILD_DIR)/$(HEX_FILENAME)
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/$(HEX_FILENAME):i

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)

.PHONY: erase
erase:
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e
	
.PHONY: debug
debug:
	if pgrep avarice; then pkill avarice; fi
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/$(HEX_FILENAME):i
	x-terminal-emulator -e avarice --edbg --ignore-intr :4242
	sleep 2
	avr-gdb -tui -iex "target remote localhost:4242" $(BUILD_DIR)/a.out
	killall -s 9 avarice	