ifeq ($(OS),Windows_NT)
	uname_S := Windows
	WinBuild = 1
	BFLAGS= -DWinBuild
else 
	uname_S := $(shell uname -s)
	BFLAGS= 	
endif

# Declaration of variables
CC = gcc
CC_FLAGS = -w -g
TMP = $(SRCS:.c=.c~) 




OBJ = *.o *.c~ talker talker2 listener showip conlisten genlisten hostent ptrent getaddrinfo givemeSockets security1 tcpechoserv sendmeudp receivemeudpreceivemeudp_recv  tcpbasicclient simptcplisten ticlient tiserver simpletcpserver simpletcpclient socketoptions signal sslc ssls interrrupted macros

all: talker talker2 listener showip conlisten genlisten hostent ptrent getaddrinfo givemeSockets security1 tcpechoserv sendmeudp receivemeudp receivemeudp_recv  tcpbasicclient simptcplisten ticlient tiserver simpletcpserver simpletcpclient socketoptions signal sslc ssls interrupted macros




# targets

## By adding -D<variable_name> we can add defines from compiler arguments. 
macros: macros.c
	$(CC) -Wall -o macros macros.c -DmyDEFINE

interrupted: interrupted.o
	$(CC) -Wall -o interrupted interrupted.c

sslc: sslconnect.o
	$(CC) -Wall -o sslc sslconnect.c -lssl -lcrypto
ssls: sslserver.o
	$(CC) -Wall -o ssls sslserver.c -lssl -lcrypto
signal: signal.o
	$(CC) signal.o -o signal
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

receivemeudp_recv: receivemeudp_recv.o
	$(CC) receivemeudp_recv.o -o receivemeudp_recv

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

simpletcpclient: simple-tcp-client.o
	$(CC) simple-tcp-client.o -o $@

socketoptions: socketoptions.o
	$(CC) socketoptions.o -o $@

### Build stuff
# To obtain object files
%.o: %.c
	$(CC) -c $(CC_FLAGS) $(BFLAGS) $< -o $@
 
# To remove generated files
clean:
	rm -f $(OBJ)

