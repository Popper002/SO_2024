#ifndef IPC_H_SO
#define IPC_H_SO
#define ALL 0666
#define O_READ  0400
#define O_WRITE 0020

struct message
{
    long m_type;
    char text[10];
};

#endif
