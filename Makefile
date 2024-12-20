CC = gcc
CFLAGS =  -std=c11 -D_GNU_SOURCE -Werror -Wall -Wextra -Werror -Wvla -g
D_FLAG = -D _PRINT_TEST -D_GNU_SOURCE -Wall -g 
RM =rm 
IPC_RM =ipcrm --all
UTILS = src/util/*.c

all:
	$(CC)   $(CFLAGS) src/master.c $(UTILS) -o bin/master -lm
	$(CC)	$(CFLAGS) src/atom.c $(UTILS) -o bin/atom -lm
	$(CC)	$(CFLAGS) src/activator.c $(UTILS) -o bin/activator  -lm
	$(CC)	$(CFLAGS) src/psu.c $(UTILS) -o bin/psu -lm
	$(CC)	$(CFLAGS) src/inhibitor.c $(UTILS) -o bin/inhibitor -lm


debug: 
	$(CC)   $(CFLAGS) $(D_FLAG) src/master.c $(UTILS) -o bin/master -lm
	$(CC)	$(CFLAGS) $(D_FLAG) src/atom.c $(UTILS) -o bin/atom -lm
	$(CC)	$(CFLAGS) $(D_FLAG) src/activator.c $(UTILS) -o bin/activator -lm
	$(CC)	$(CFLAGS) $(D_FLAG) src/psu.c $(UTILS) -o bin/psu -lm
	$(CC)	$(CFLAGS) $(D_FLAG) src/inhibitor.c $(UTILS) -o bin/inhibitor -lm

clean:
		$(RM) -f bin/*

mem_check: 
	valgrind ./bin/master



run: 
	./bin/master

gdb:
	gdb ./bin/master

ipc_clean:
		$(IPC_RM)

docs:
	pdflatex docs/Relazione.tex 
