CC = gcc 
CFLAGS_EXTRA_ALL = -Wvla  -D_GNU_SOURCE
CFLAGS = -D_GNU_SOURCE
RM =rm 
IPC_RM =ipcrm 
master:
	$(CC) $(CFLAGS) src/master.c -o master -lm

atom:
	$(CC) $(CFLAGS) src/atom.c -o atom

rm: 
	$(RM) ./master ./atom

cleanup:
	$(RM) ./master ./atom
	$(IPC_RM) 