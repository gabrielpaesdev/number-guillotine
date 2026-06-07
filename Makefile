TARGET = NumberGuillotine


CC = gcc


CFLAGS = -Wall -Wextra -O2 $(shell pkg-config --cflags gtk+-3.0)


LIBS = $(shell pkg-config --libs gtk+-3.0)


SRCS = main.c engine.c lang.c resources.c


OBJS = $(SRCS:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)


.PHONY: all clean
