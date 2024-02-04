CC = gcc 
CFLAGS =  -D_GNU_SOURCE 
RM =rm 
IPC_RM =ipcrm --all
COMMON_DEPS = src/header/*.h
all: master atom activator

run: ./master
master:$(COMMON_DEPS)
	@$(CC) $(CFLAGS) src/master.c -o master -lm

atom: $(COMMON_DEPS)
	@$(CC) $(CFLAGS) src/atom.c -o atom -lm
activator: $(COMMON_DEPS)
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
