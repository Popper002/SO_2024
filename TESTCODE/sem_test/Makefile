CC = gcc
CFLAGS = -std=c11 -D_GNU_SOURCE -Wall -Wextra -g	
sem:
	$(CC) $(CFLAGS) sem.c -o sem
remove:
	ipcrm --all 
	rm sem semProc all
all:
	$(CC) $(CFLAGS) all.c -o all
exe: 
	$(CC) $(CFLAGS) exe.c -o exe 
	echo exe_creato
semProc:
	$(CC) $(CFLAGS) semProc.c -o semProc
		
semAllZero:
	$(CC) $(CFLAGS) waitzero.c -o sem
semdontWait:
	$(CC) $(CFLAGS) dontwait.c -o sem
sem2:
		$(CC) $(CFLAGS) sem2.c -o sem