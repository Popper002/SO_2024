CC = gcc 
CFLAGS =  -std=c11 -D_GNU_SOURCE  -Wall -Wextra -g
RM =rm 
IPC_RM =ipcrm --all
COMMON_DEPS = src/header/*.h
all: master atom activator

run: 
	./bin/master

master:$(COMMON_DEPS)
	$(CC) $(CFLAGS) src/master.c -o bin/master -lm

atom: $(COMMON_DEPS)
	$(CC) $(CFLAGS) src/atom.c -o bin/atom -lm
activator: $(COMMON_DEPS)
	$(CC)	$(CFLAGS) src/activator.c src/header/common.h -o bin/activator -lm

clean:
		$(IPC_RM)
		$(RM) -f bin/*

mem_check: 
	valgrind ./bin/master

docs:
	pdflatex docs/Relazione.tex 
