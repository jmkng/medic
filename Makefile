CC      = cc
CFLAGS  = -Wall -Wextra -std=c23 -pedantic
TARGET  = medic
SRC     = main.c

.PHONY: all clean run

ifeq ($(shell uname),Darwin)
    SRC += $(wildcard *_darwin.c)
else ifeq ($(shell uname),Linux)
    SRC += $(wildcard *_linux.c)
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

