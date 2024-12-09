CC = gcc 
CFLAGS =  -std=c11 -D_GNU_SOURCE  -Wall -Wextra -g
D_FLAG = -D _PRINT_TEST -D_GNU_SOURCE -Wall -g 
RM =rm 
IPC_RM =ipcrm --all
UTILS = src/util/*.c


start:
	$(CC) $(CFLAGS) startandstop.c ../../src/util/my_sem_lib.c library.h -o signal
rcv: 
	$(CC) $(CFLAGS) delivery.c library.h -o rcv 

clean: 
	$(IPC_RM) 
	$(RM) signal rcv timeout alarm_test start stop 

alarm: 
	
debug:
	$(CC) $(CFLAGS) -D _PRINT_TEST startandstop.c library.h -o signal 
	$(CC) $(CFLAGS) -D _PRINT_TEST alarm.c -o alarm_test
	$(CC) $(CFLAGS) -D _PRINT_TEST timeout.c -o timeout
	$(CC) $(CFLAGS) -D _PRINT_TEST start.c -o start
	$(CC) $(CFLAGS) -D _PRINT_TEST stop.c -o stop
	echo compilation compleate
