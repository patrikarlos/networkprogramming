# Declaration of variables
CC = gcc
CC_FLAGS = -w -g
 


OBJECTS = $(wildcard *.o talker talker2 listener showip conlisten genlisten hostent ptrent getaddrinfo givemeSockets security1 tcpechoserv sendmeudp receivemeudp reactionc reactions tcpbasicclient simptcplisten ticlient tiserver simpletcpserver)

all: talker talker2 listener showip conlisten genlisten hostent ptrent getaddrinfo givemeSockets security1 tcpechoserv sendmeudp receivemeudp reactionc reactions tcpbasicclient simptcplisten ticlient tiserver simpletcpserver




# targets
security1: security1.o
	$(CC) security1.o -o security1 -lcrypto

givemeSockets: givemeSockets.o
	$(CC) givemeSockets.o -o givemeSockets

getaddrinfo: getaddrinfo.o
	$(CC) getaddrinfo.o -o getaddrinfo

hostent: hostent.o
	$(CC) hostent.o -o hostent

ptrent: ptrent.o
	$(CC) ptrent.o -o ptrent

sendmeudp: sendmeudp.o
	$(CC) sendmeudp.o -o sendmeudp

receivemeudp: receivemeudp.o
	$(CC) receivemeudp.o -o receivemeudp

talker: talker.o
	$(CC) talker.o -o talker

talker2: talker2.o
	$(CC) talker2.o -o talker2

listener: listener.o
	$(CC) listener.o -o listener

conlisten: concurr-tcp.o
	$(CC) concurr-tcp.o -o conlisten

genlisten: concurr-tcp-gen.o
	$(CC) concurr-tcp-gen.o -o genlisten

simptcplisten: simple-tcp-gen.o
	$(CC) simple-tcp-gen.o -o simptcplisten

tcpbasicclient: tcp_basic_client.o
	$(CC) tcp_basic_client.o -o tcpbasicclient

showip: showip.o
	$(CC) showip.o -o showip

tcpechoserv: tcp_select_server.o
	$(CC) tcp_select_server.o -o tcpechoserv

ticlient: titfortat-tcp-client.o
	$(CC) titfortat-tcp-client.o -o $@

tiserver: titfortat-tcp-server.o
	$(CC) titfortat-tcp-server.o -o $@

simpletcpserver: simple-tcp-server.o
	$(CC) simple-tcp-server.o -o $@

### Build stuff
# To obtain object files
%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@
 
# To remove generated files
clean:
	rm -f talker $(OBJECTS)
