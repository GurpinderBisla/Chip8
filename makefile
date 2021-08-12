LINKER_FLAGS := $(shell sdl2-config --libs)
CFLAGS := $(shell sdl2-config --cflags)

SRC := $(chip8.c display.c cpu.c)

FLAGS := -Wextra -Wall -Werror -pedantic

all: $(SRC)
	gcc $(FLAGS) $(LINKER_FLAGS) $(CFLAGS) -o chip8 chip8.c
