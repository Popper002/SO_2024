#ifndef IPC_H_SO
#define IPC_H_SO
#define ALL 0666
#define O_READ  0400
#define O_WRITE 0020

struct message
{
    char text[2]; 
    long m_type;
};
static int pipe_fd[2]; //fd[0] -read //fd[1]--write

#endif
