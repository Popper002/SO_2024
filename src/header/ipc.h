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

#endif
