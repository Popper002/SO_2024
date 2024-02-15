CC = gcc 
CFLAGS =  -std=c11 -D_GNU_SOURCE  -Wall -Wextra -g
D_FLAG = -D _PRINT_TEST -D_GNU_SOURCE -Wall -g 
RM =rm 
IPC_RM =ipcrm --all
UTILS = src/util/*.c
all: master atom activator

run: 
	./bin/master

master:$(COMMON_DEPS)
	$(CC) $(CFLAGS) src/master.c $(UTILS) -o bin/master -lm

atom:$(COMMON_DEPS)
	$(CC)	$(CFLAGS) src/atom.c $(UTILS) src/header/ipc.h -o bin/atom -lm
activator:$(COMMON_DEPS)
	$(CC)	$(CFLAGS) src/activator.c $(UTILS) -o bin/activator src/header/ipc.h -lm

debug: 
	$(CC)   $(D_FLAG)  src/master.c $(UTILS) src/header/master.h -o bin/master  -lm
	$(CC)	$(CFLAGS) src/atom.c $(UTILS) src/header/ipc.h -o bin/atom -lm
	$(CC)	$(CFLAGS) src/activator.c $(UTILS) -o bin/activator src/header/ipc.h -lm
clean:
		$(IPC_RM)
		$(RM) -f bin/*

mem_check: 
	valgrind ./bin/master

docs:
	pdflatex docs/Relazione.tex 



	