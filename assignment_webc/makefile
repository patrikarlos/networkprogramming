# Declaration of variables
CC = gcc
CC_FLAGS = -w -g  -Wall -Wextra

OBJECTS = $(wildcard *.o)

all: myget 

myget: webc.o
	$(CC) webc.o -o myget -lcurl


### Build stuff
# To obtain object files
%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@
 
# To remove generated files
clean:
	rm -f $(OBJECTS) *.exe
