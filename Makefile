CC      	= cc
CFLAGS  	= -Wall -Wextra -std=c23 -pedantic -Iinclude
TARGET  	= libmedic.a
DEMOTARGET  = medic

SRC = src/cpu.c src/fs.c src/host.c src/mem.c
OBJ := $(SRC:.c=.o)

ifeq ($(shell uname),Darwin)
    FW = -framework CoreFoundation
else
    FW =
endif

.PHONY: all clean run

all: $(TARGET) $(DEMOTARGET)

$(TARGET): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DEMOTARGET): $(TARGET) src/main.c
	$(CC) $(CFLAGS) src/main.c -L. -lmedic $(FW) -o $@

run: $(DEMOTARGET)
	./$(DEMOTARGET)

clean:
	rm -f $(OBJ) $(TARGET) $(DEMOTARGET)
