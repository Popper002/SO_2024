CC = gcc 
CFLAGS_EXTRA_ALL = -Wvla  -D_GNU_SOURCE
CFLAGS = -D_GNU_SOURCE
RM =rm 
IPC_RM =ipcrm --all

all: master atom activator
master:
	@$(CC) $(CFLAGS) src/master.c -o master -lm

atom:
	@$(CC) $(CFLAGS) src/atom.c -o atom
activator:
	@$(CC)	$(CFLAGS) src/activator.c src/header/common.h -o active -lm

rm: 
	@$(RM) ./master ./atom ./active
.c.o:
	@$(CC) -c $(CFLAGS) $< -o $@

cleanup:
		$(IPC_RM)
		$(RM) ./master ./atom
	$(RM) ./master ./atom
	$(IPC_RM) 


docs:
	pdflatex docs/Relazione.tex 
