CC = gcc 
CFLAGS =  -std=c11 -D_GNU_SOURCE  -Wall -Wextra -g
D_FLAG = -D _PRINT_TEST -D_GNU_SOURCE -Wall -g 
RM =rm 
IPC_RM =ipcrm --all
UTILS = src/util/*.c


start:
	$(CC) $(CFLAGS) startandstop.c library.h -o signal 
rcv: 
	$(CC) $(CFLAGS) delivery.c library.h -o rcv 

rm: 
	$(IPC_RM) $(RM) signal rcv

debug:
	$(CC) $(CFLAGS) -D _PRINT_TEST startandstop.c library.h -o signal 