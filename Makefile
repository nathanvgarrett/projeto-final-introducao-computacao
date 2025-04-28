CC = gcc
CFLAGS = -Wall -g

SOURCES = data.c wug.c util.c projeto.c
HEADERS = data.h wug.h util.h
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = projeto

TEST_SOURCES = data.c wug.c util.c test.c
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)
TEST_EXECUTABLE = test

all: $(EXECUTABLE) $(TEST_EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del *.o $(EXECUTABLE).exe $(TEST_EXECUTABLE).exe