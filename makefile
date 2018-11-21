# Declaration of variables
CC = gcc
CC_FLAGS = -w
 


OBJECTS = $(wildcard *.o)


# targets
talker: talker.o
	$(CC) talker.o -o talker

listener: listener.o
	$(CC) listener.o -o listener

showip: showip.o
	$(CC) showip.o -o showip

### Build stuff
# To obtain object files
%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@
 
# To remove generated files
clean:
	rm -f talker $(OBJECTS)
