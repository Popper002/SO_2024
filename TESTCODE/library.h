#include <stdio.h>
#include<sys/ipc.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#define macro_key 12


struct msg{
    long msg_type;
    char text[10]; 
};