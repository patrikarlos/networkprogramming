# Declaration of variables
CC = gcc
CC_FLAGS = -w -g  -Wall -Wextra

OBJECTS = $(wildcard *.o)

all: sendto receive


sendto: sendmetcp.o
	$(CC) sendmetcp.o -o sendto  -lssl -lcrypto

receive: receivemetcp.o
	$(CC) receivemetcp.o -o receive  -lssl -lcrypto

### Build stuff
# To obtain object files
%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@
 
# To remove generated files
clean:
	rm -f $(OBJECTS) *.exe
