ifeq ($(OS),Windows_NT)
	uname_S := Windows
	WinBuild = 1
	BFLAGS= -DWinBuild
else
	uname_S := $(shell uname -s)
	ifeq ($(uname_S),Darwin)
		uname_S := DARWIN
		MacBuild = 1
		LDFLAGS="-L/opt/homebrew/opt/openssl/lib"
		CPPFLAGS="-I/opt/homebrew/opt/openssl/include"
		BFLAGS= -DMacBuild
	else 
		WinBuild = 0
		BFLAGS= 	
	endif
endif


$(info $$uname_S is [${uname_S}])
$(info $$LDFLAGS is [${LDFLAGS}])
$(info $$CPPFLAGS is [${CPPFLAGS}])
$(info $$BFLAGS is [${BFLAGS}])


# Declaration of variables
CC = gcc
CC_FLAGS = -w -g $(CPPFLAGS)
TMP = $(SRCS:.c=.c~) 




OBJ = *.o *.c~ talker talker2 listener showip conlisten genlisten hostent ptrent getaddrinfo givemeSockets security1 tcpechoserv sendmeudp receivemeudpreceivemeudp_recv  tcpbasicclient simptcplisten ticlient tiserver simpletcpserver simpletcpclient socketoptions signal sslc ssls interrrupted macros broadcast_udp support iomodels-blocking iomodels-nonblocking iomodels-signal tc

all: talker talker2 listener showip conlisten genlisten hostent ptrent getaddrinfo givemeSockets security1 tcpechoserv sendmeudp receivemeudp receivemeudp_recv  tcpbasicclient simptcplisten ticlient tiserver simpletcpserver simpletcpclient socketoptions signal sslc ssls interrupted macros broadcast_udp support iomodels-blocking iomodels-nonblocking iomodels-signal tc




# targets
tc: trivial-tcp-server.c
	$(CC) -Wall -o tc trivial-tcp-server.c

iomodels-blocking: io-models-blocking.c
	$(CC) -Wall -o iomodels-blocking io-models-blocking.c

iomodels-nonblocking: io-models-nonblocking.c
	$(CC) -Wall -o iomodels-nonblocking io-models-nonblocking.c

iomodels-signal: io-models-signal.c
	$(CC) -Wall -o iomodels-signal io-models-signal.c

support: support.c
	$(CC) -Wall -o support support.c -Wint-to-pointer-cast

broadcast_udp: broadcast_udp.c
	$(CC) -Wall -o broadcast_udp broadcast_udp.c 

## By adding -D<variable_name> we can add defines from compiler arguments. 
macros: macros.c
	$(CC) -Wall -o macros macros.c -DmyDEFINE

interrupted: interrupted.o
	$(CC) -Wall -o interrupted interrupted.c

sslc: sslconnect.o 
	$(CC) -Wall $(CC_FLAGS) $(BFLAGS)  -o sslc sslconnect.c -lssl -lcrypto $(LDFLAGS)
ssls: sslserver.o
	$(CC) -Wall $(CC_FLAGS) $(BFLAGS) -o ssls sslserver.c -lssl -lcrypto $(LDFLAGS)
signal: signal.o
	$(CC) signal.o -o signal
security1: security1.o
	$(CC) security1.o -o security1 -lcrypto -lssl $(LDFLAGS)

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

