CC      = cc
CFLAGS  = -Wall -Wextra -std=c23 -pedantic
TARGET  = medic
SRC     = main.c

.PHONY: all clean run

ifeq ($(shell uname),Darwin)
    SRC += $(wildcard *_darwin.c)
	FW = -framework CoreFoundation
else ifeq ($(shell uname),Linux)
    SRC += $(wildcard *_linux.c)
	FW = 
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(FW)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

