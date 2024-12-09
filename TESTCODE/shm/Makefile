CC = gcc 
CFLAGS =  -std=c11 -D_GNU_SOURCE  -Wall -Wextra -g
D_FLAG = -D _PRINT_TEST -D_GNU_SOURCE -Wall -g 
RM =rm 
IPC_RM =ipcrm --all
all:
	send rcv

run: 
	./send

send: 
	$(CC) $(CFLAGS) $(D_FLAG) send.c library.h -o send
rcv:
	$(CC) $(CFLAGS) $(D_FLAG) rcv.c library.h -o rcv

struct :
	$(CC) $(CFLAGS) $(D_FLAG) struct.c -o struct
	$(CC) $(CFLAGS) $(D_FLAG) rcv_struct.c -o rcv_struct
	
rm:
	$(IPC_RM)
	$(RM) -f send rcv 
