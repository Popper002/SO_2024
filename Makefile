CC = gcc 
CFLAGS =  -std=c11 -D_GNU_SOURCE  -Wall -Wextra -g
D_FLAG = -D _PRINT_TEST -D_GNU_SOURCE -Wall -g 
RM =rm 
IPC_RM =ipcrm --all
UTILS = src/util/*.c
all: master atom activator fuel 

run: 
	./bin/master

master:$(COMMON_DEPS)
	$(CC) $(CFLAGS) src/master.c $(UTILS) -o bin/master -lm

atom:$(COMMON_DEPS)
	$(CC)	$(CFLAGS) src/atom.c $(UTILS) src/header/ipc.h -o bin/atom -lm
activator:$(COMMON_DEPS)
	$(CC)	$(CFLAGS) src/activator.c $(UTILS) -o bin/activator src/header/ipc.h -lm

fuel:$(COMMON_DEPS)
	$(CC)	$(CFLAGS) src/fuel.c $(UTILS) src/header/ipc.h -o bin/fuel -lm
inebitore:$(COMMON_DEPS)
	$(CC)	$(CFLAGS) src/inebitore.c $(UTILS) src/header/ipc.h bin/inebitore -lm


debug: 
	$(CC)   $(CFLAGS) $(D_FLAG) src/master.c $(UTILS) src/header/master.h -o bin/master -lm
	$(CC)	$(CFLAGS) $(D_FLAG) src/atom.c $(UTILS) src/header/ipc.h -o bin/atom -lm
	$(CC)	$(CFLAGS) $(D_FLAG) src/activator.c $(UTILS) src/header/ipc.h -o bin/activator -lm
	$(CC)	$(CFLAGS) $(D_FLAG) src/fuel.c $(UTILS) src/header/ipc.h -o bin/fuel -lm
	$(CC)	$(CFLAGS) $(D_FLAG) src/inebitore.c $(UTILS) src/header/ipc.h -o bin/inebitore -lm

clean:
		$(IPC_RM)
		$(RM) -f bin/*

mem_check: 
	valgrind ./bin/master

docs:
	pdflatex docs/Relazione.tex 



	
